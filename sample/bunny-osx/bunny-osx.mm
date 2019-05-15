#include <SDL.h>
#include <SDL_syswm.h>
#include <Cocoa/Cocoa.h>
#include <QuartzCore/CAMetalLayer.h>
#include <Metal/Metal.h>
#include <mtlpp.hpp>
#include <el_debug.h>

#undef EL_BUILD_METAL
#define EL_BUILD_METAL 1

#include <Metal/mtl_types.h>
#include <Metal/mtl_texture.h>
#include <Metal/mtl_shader.h>
#include <Metal/mtl_depth_stencil.h>
#include <Metal/mtl_device.h>
#include <Metal/mtl_context.h>

typedef void* SwapchainHandle;

class Renderer
{
public:
    
    Renderer();
    
    mtlpp::Device& getDevice();
    mtlpp::CommandQueue& getCommandQueue();
    
    void setup();
    void setupBlitRenderer();
    void setupMeshRenderer();
    
    void makeCurrent(SwapchainHandle handle);
    void draw();
    
    mtlpp::Device _device;
    mtlpp::CommandQueue _commandQueue;
    mtlpp::Function _blitVertexFunction;
    mtlpp::Function _blitFragmentFunction;
    mtlpp::RenderPipelineState _blitPipelineState;
    mtlpp::Texture _color;
    mtlpp::Texture _depth;
    
    el::GraphicsPixelFormat _surfaceFormat;
    SwapchainHandle _handle;
    const el::GraphicsPixelFormat _colorFormat = el::GraphicsPixelFormatRG11B10Float;
    const el::GraphicsPixelFormat _depthFormat = el::GraphicsPixelFormatDepth32Float;
};

Renderer::Renderer()
{
    _surfaceFormat = el::GraphicsPixelFormatInvalid;
}

mtlpp::Device& Renderer::getDevice()
{
    return _device;;
}

mtlpp::CommandQueue& Renderer::getCommandQueue()
{
    return _commandQueue;
}

void Renderer::setup()
{
    _device = mtlpp::Device::CreateSystemDefaultDevice();
    _commandQueue = _device.NewCommandQueue();
    
    setupBlitRenderer();
    setupMeshRenderer();
}

void Renderer::setupBlitRenderer()
{
    const char vertexShaderSrc[] = R"""(
#include <metal_stdlib>
using namespace metal;

typedef struct
{
    packed_float3 position;
    packed_float2 texcoord;
} vertex_t;

typedef struct
{
    float4 clipSpacePosition [[position]];
    float2 textureCoordinate;
} RasterizerData;

vertex RasterizerData main0(unsigned int vID[[vertex_id]])
{
    const vertex_t vertexArray[3] = {
        { {-1.0,  3.0, 0.0}, {0.0, 2.0} },
        { { 3.0, -1.0, 0.0}, {2.0, 0.0} },
        { {-1.0, -1.0, 0.0}, {0.0, 0.0} }
    };
    
    RasterizerData data;
    data.clipSpacePosition = float4(vertexArray[vID].position, 1.0);
    data.textureCoordinate = vertexArray[vID].texcoord;
    return data;
}
    )""";
    
    const char fragmentShaderSrc[] = R"""(
#include <metal_stdlib>
using namespace metal;

typedef struct
{
    float4 clipSpacePosition [[position]];
    float2 textureCoordinate;
} RasterizerData;

fragment half4 main0(
                     RasterizerData in [[stage_in]],
                     texture2d<half> colorTexture [[texture(0)]])
{
    constexpr sampler textureSampler (mag_filter::nearest,
                                      min_filter::nearest);
    // Sample the texture to obtain a color
    const half4 colorSample = colorTexture.sample(textureSampler, in.textureCoordinate);
    
    // We return the color of the texture
    return colorSample;
}
    )""";

    ns::Error error;
    auto vertexLibrary = _device.NewLibrary(vertexShaderSrc, mtlpp::CompileOptions(), &error);
    if (!vertexLibrary)
        EL_TRACE("Failed to created pipeline state, error %s", error.GetLocalizedDescription().GetCStr());
    EL_ASSERT(vertexLibrary);
    auto fragmentLibrary = _device.NewLibrary(fragmentShaderSrc, mtlpp::CompileOptions(), &error);
    if (!fragmentLibrary)
        EL_TRACE("Failed to created pipeline state, error %s", error.GetLocalizedDescription().GetCStr());
    EL_ASSERT(fragmentLibrary);
    
    EL_ASSERT(vertexLibrary && fragmentLibrary);
    _blitVertexFunction = vertexLibrary.NewFunction("main0");
    EL_ASSERT(_blitVertexFunction);
    _blitFragmentFunction = fragmentLibrary.NewFunction("main0");
    EL_ASSERT(_blitFragmentFunction);
}

void Renderer::setupMeshRenderer()
{
}

void Renderer::makeCurrent(SwapchainHandle handle)
{
    _handle = handle;
}

void Renderer::draw()
{
    CAMetalLayer* layer = reinterpret_cast<CAMetalLayer*>(_handle);
    
    ns::Error error;
    
    auto surfaceFormat = el::asGraphicsPixelFormat((mtlpp::PixelFormat)layer.pixelFormat);
    if (_surfaceFormat != surfaceFormat)
    {
        mtlpp::RenderPipelineDescriptor blitPipelineDesc;
        blitPipelineDesc.SetLabel("blitPipeline");
        blitPipelineDesc.SetVertexFunction(_blitVertexFunction);
        blitPipelineDesc.SetFragmentFunction(_blitFragmentFunction);
        {
            auto colorAttachment = blitPipelineDesc.GetColorAttachments()[0];
            colorAttachment.SetPixelFormat(el::asPixelFormat(surfaceFormat));
        }
        _blitPipelineState = _device.NewRenderPipelineState(blitPipelineDesc, &error);
        _surfaceFormat = surfaceFormat;
    }
    
    auto drawable = [layer nextDrawable];
    
    mtlpp::Texture swapchainImage(ns::Handle{(__bridge void*)drawable.texture});
    
    if (!_color ||
        swapchainImage.GetWidth() != _color.GetWidth() ||
        swapchainImage.GetHeight() != _color.GetHeight())
    {
        mtlpp::TextureUsage textureUsage = (mtlpp::TextureUsage)(mtlpp::TextureUsage::ShaderRead | mtlpp::TextureUsage::RenderTarget);
        auto colorTextureDesc = mtlpp::TextureDescriptor::Texture2DDescriptor(el::asPixelFormat(_colorFormat), 0, 0, false);
        colorTextureDesc.SetUsage(textureUsage);
        colorTextureDesc.SetStorageMode(mtlpp::StorageMode::Private);
        colorTextureDesc.SetWidth(swapchainImage.GetWidth());
        colorTextureDesc.SetHeight(swapchainImage.GetHeight());
        _color = _device.NewTexture(colorTextureDesc);
        
        auto depthTextureDesc = mtlpp::TextureDescriptor::Texture2DDescriptor(el::asPixelFormat(_depthFormat), 0, 0, false);
        depthTextureDesc.SetUsage(mtlpp::TextureUsage::RenderTarget);
        depthTextureDesc.SetStorageMode(mtlpp::StorageMode::Private);
        depthTextureDesc.SetWidth(swapchainImage.GetWidth());
        depthTextureDesc.SetHeight(swapchainImage.GetHeight());
        _depth = _device.NewTexture(depthTextureDesc);
    }
    
    auto commandBuffer = _commandQueue.CommandBuffer();
    mtlpp::RenderPassDescriptor renderPassDesc;
    {
        auto colorAttachment = renderPassDesc.GetColorAttachments()[0];
        colorAttachment.SetClearColor(mtlpp::ClearColor(0.2f, 0.5f, 0.7f, 1.0f));
        colorAttachment.SetLoadAction(mtlpp::LoadAction::Clear);
        colorAttachment.SetStoreAction(mtlpp::StoreAction::Store);
        colorAttachment.SetTexture(_color);
        colorAttachment.SetLevel(0);
        colorAttachment.SetSlice(0);
        renderPassDesc.SetRenderTargetArrayLength(1);
        
        auto depthAttachment = renderPassDesc.GetDepthAttachment();
        depthAttachment.SetClearDepth(1.0);
        depthAttachment.SetTexture(_depth);
        depthAttachment.SetLoadAction(mtlpp::LoadAction::Clear);
        depthAttachment.SetStoreAction(mtlpp::StoreAction::DontCare);
    }
    
    auto renderEncoder = commandBuffer.RenderCommandEncoder(renderPassDesc);
    renderEncoder.SetLabel("renderEncdoer");
    // renderEncoder.SetVertexData(const void *bytes, uint32_t length, uint32_t index);
    // renderEncoder.SetRenderPipelineState(renderPipelineState);
    // renderEncoder.DrawIndexed((mtlpp::PrimitiveType::Triangle, uint32_t indexCount, IndexType indexType, const Buffer &indexBuffer, uint32_t indexBufferOffset);
    renderEncoder.EndEncoding();
    
    mtlpp::RenderPassDescriptor blitPassDesc;
    {
        auto colorAttachment = blitPassDesc.GetColorAttachments()[0];
        colorAttachment.SetLoadAction(mtlpp::LoadAction::DontCare);
        colorAttachment.SetStoreAction(mtlpp::StoreAction::DontCare);
        colorAttachment.SetTexture(swapchainImage);
        colorAttachment.SetLevel(0);
        colorAttachment.SetSlice(0);
        blitPassDesc.SetRenderTargetArrayLength(1);
    }
    auto blitEncoder = commandBuffer.RenderCommandEncoder(blitPassDesc);
    blitEncoder.SetLabel("blitEncoder");
    blitEncoder.SetRenderPipelineState(_blitPipelineState);
    blitEncoder.SetFragmentTexture(_color, 0);
    blitEncoder.Draw(mtlpp::PrimitiveType::Triangle, 0, 3);
    blitEncoder.EndEncoding();
    
    commandBuffer.Present(ns::Handle{(__bridge void*)drawable});
    commandBuffer.Commit();
    commandBuffer.WaitUntilCompleted();
}

int main()
{
    SDL_Init(SDL_INIT_VIDEO);
    
    SDL_Window* window = SDL_CreateWindow("dpeth sample",
                                          SDL_WINDOWPOS_UNDEFINED,
                                          SDL_WINDOWPOS_UNDEFINED,
                                          1280, 768,
                                          SDL_WINDOW_RESIZABLE);
    
    EL_ASSERT(window != nullptr);

    SDL_SysWMinfo info;
    SDL_GetVersion(&info.version);
    SDL_GetWindowWMInfo(window, &info);
    
    NSView* view = [info.info.cocoa.window contentView];
    
    Renderer renderer;
    renderer.setup();
    
    CAMetalLayer* layer = [CAMetalLayer layer];
    layer.device = (__bridge id<MTLDevice>)renderer.getDevice().GetPtr();
    layer.opaque = true;
    layer.drawableSize = [view convertSizeToBacking:view.bounds.size];
    layer.pixelFormat = MTLPixelFormatBGRA8Unorm;
    layer.framebufferOnly = YES;
    
    view.wantsLayer = YES;
    view.layer = layer;

    while (true)
    {
        bool isQuit = false;
        SDL_Event e;
        while (SDL_PollEvent(&e))
            isQuit = (e.type == SDL_QUIT);
        if (isQuit) break;
        
        renderer.makeCurrent(layer);
        renderer.draw();
    }

    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
