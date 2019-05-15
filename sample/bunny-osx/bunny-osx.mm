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
    
    const auto surfaceFormat = el::GraphicsPixelFormatBGRA8Unorm;
    const auto colorFormat = el::GraphicsPixelFormatRG11B10Float;
    const auto depthFormat = el::GraphicsPixelFormatDepth32Float;
    
    auto device = mtlpp::Device::CreateSystemDefaultDevice();
    auto commandQueue = device.NewCommandQueue();
    
    CAMetalLayer* layer = [CAMetalLayer layer];
    layer.device = (__bridge id<MTLDevice>)device.GetPtr();
    layer.opaque = true;
    layer.drawableSize = [view convertSizeToBacking:view.bounds.size];
    layer.pixelFormat = (MTLPixelFormat)el::asPixelFormat(surfaceFormat);
    layer.framebufferOnly = YES;
    
    view.wantsLayer = YES;
    view.layer = layer;
    
    ns::Error error;
    auto vertexLibrary = device.NewLibrary(vertexShaderSrc, mtlpp::CompileOptions(), &error);
    if (!vertexLibrary)
        EL_TRACE("Failed to created pipeline state, error %s", error.GetLocalizedDescription().GetCStr());
    auto fragmentLibrary = device.NewLibrary(fragmentShaderSrc, mtlpp::CompileOptions(), &error);
    if (!fragmentLibrary)
        EL_TRACE("Failed to created pipeline state, error %s", error.GetLocalizedDescription().GetCStr());
    
    EL_ASSERT(vertexLibrary && fragmentLibrary);
    auto vertexFunction = vertexLibrary.NewFunction("main0");
    
    auto fragmentFunction = fragmentLibrary.NewFunction("main0");

    mtlpp::RenderPipelineDescriptor blitPipelineDesc;
    blitPipelineDesc.SetLabel("blitPipeline");
    blitPipelineDesc.SetVertexFunction(vertexFunction);
    blitPipelineDesc.SetFragmentFunction(fragmentFunction);
    {
        auto colorAttachment = blitPipelineDesc.GetColorAttachments()[0];
        colorAttachment.SetPixelFormat(el::asPixelFormat(surfaceFormat));
    }
    auto blitPipelineState = device.NewRenderPipelineState(blitPipelineDesc, &error);
    
    mtlpp::TextureUsage textureUsage = (mtlpp::TextureUsage)(mtlpp::TextureUsage::ShaderRead | mtlpp::TextureUsage::RenderTarget);
    auto colorTextureDesc = mtlpp::TextureDescriptor::Texture2DDescriptor(el::asPixelFormat(colorFormat), layer.drawableSize.width, layer.drawableSize.height, false);
    colorTextureDesc.SetUsage(textureUsage);
    colorTextureDesc.SetStorageMode(mtlpp::StorageMode::Private);
    auto color = device.NewTexture(colorTextureDesc);
    auto depthTextureDesc = mtlpp::TextureDescriptor::Texture2DDescriptor(el::asPixelFormat(depthFormat), layer.drawableSize.width, layer.drawableSize.height, false);
    depthTextureDesc.SetUsage(mtlpp::TextureUsage::RenderTarget);
    depthTextureDesc.SetStorageMode(mtlpp::StorageMode::Private);
    auto depth = device.NewTexture(depthTextureDesc);
    
    while (true)
    {
        bool isQuit = false;
        SDL_Event e;
        while (SDL_PollEvent(&e))
            isQuit = (e.type == SDL_QUIT);
        if (isQuit) break;
        
        auto drawable = [layer nextDrawable];

        auto commandBuffer = commandQueue.CommandBuffer();

        mtlpp::Texture swapchainImage(ns::Handle{(__bridge void*)drawable.texture});
        
        if (swapchainImage.GetWidth() != color.GetWidth() ||
            swapchainImage.GetHeight() != color.GetHeight())
        {
            colorTextureDesc.SetWidth(swapchainImage.GetWidth());
            colorTextureDesc.SetHeight(swapchainImage.GetHeight());
            color = device.NewTexture(colorTextureDesc);
            depthTextureDesc.SetWidth(swapchainImage.GetWidth());
            depthTextureDesc.SetHeight(swapchainImage.GetHeight());
            depth = device.NewTexture(colorTextureDesc);
        }

        mtlpp::RenderPassDescriptor renderPassDesc;
        {
            auto colorAttachment = renderPassDesc.GetColorAttachments()[0];
            colorAttachment.SetClearColor(mtlpp::ClearColor(0.2f, 0.5f, 0.7f, 1.0f));
            colorAttachment.SetLoadAction(mtlpp::LoadAction::Clear);
            colorAttachment.SetStoreAction(mtlpp::StoreAction::Store);
            colorAttachment.SetTexture(color);
            colorAttachment.SetLevel(0);
            colorAttachment.SetSlice(0);
            renderPassDesc.SetRenderTargetArrayLength(1);
            auto depthAttachment = renderPassDesc.GetDepthAttachment();
            depthAttachment.SetClearDepth(1.0);
            depthAttachment.SetTexture(depth);
            depthAttachment.SetLoadAction(mtlpp::LoadAction::Clear);
            depthAttachment.SetStoreAction(mtlpp::StoreAction::DontCare);
        }

        auto renderEncoder = commandBuffer.RenderCommandEncoder(renderPassDesc);
        renderEncoder.SetLabel("renderEncdoer");
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
        blitEncoder.SetRenderPipelineState(blitPipelineState);
        blitEncoder.SetFragmentTexture(color, 0);
        blitEncoder.Draw(mtlpp::PrimitiveType::Triangle, 0, 3);
        blitEncoder.EndEncoding();
        
        commandBuffer.Present(ns::Handle{(__bridge void*)drawable});
        commandBuffer.Commit();
        commandBuffer.WaitUntilCompleted();
    }

    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
