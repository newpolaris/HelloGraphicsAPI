#include <el_debug.h>
#include <el_platform.h>
#include <SDL.h>
#include <SDL_syswm.h>
#include <Cocoa/Cocoa.h>
#include <QuartzCore/CAMetalLayer.h>
#include <Metal/Metal.h>
#include <mtlpp.hpp>

#include <graphics_device.h>
#include <graphics_texture.h>
#include <graphics_shader.h>
#include <graphics_data.h>
#include <graphics_program.h>
#include <graphics_depth_stencil.h>

#include <linmath.h>

#undef EL_BUILD_METAL
#define EL_BUILD_METAL 1
#include <Metal/mtl_texture.h>
#include <Metal/mtl_shader.h>
#include <Metal/mtl_types.h>
#include <Metal/mtl_depth_stencil.h>
#include <Metal/mtl_device.h>
#include <Metal/mtl_context.h>

namespace {

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

        vertex RasterizerData Main(
            const device vertex_t* vertexArray [[buffer(0)]],
            unsigned int vID[[vertex_id]])
        {
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
    
        fragment half4 Main(
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
    
    // AAPLVertex improve;
    const float vertexData[] =
    {
         0.0f,  1.0f, 0.0f,  0.5f, 0.0f,
         1.0f, -1.0f, 0.0f,  1.0f, 1.0f,
        -1.0f, -1.0f, 0.0f,  0.0f, 1.0f,
    };
    
    char _pixels[16 * 16];
}

struct MetalSwapchain final
{
    mtlpp::Texture _color;
    mtlpp::Texture _depth;
    el::GraphicsDevicePtr _device;
    
    MetalSwapchain()
    {
    }

    ~MetalSwapchain()
    {
        _color = ns::Handle{};
        _depth = ns::Handle{};
        _device = nullptr;
    }

    void setDevice(const el::GraphicsDevicePtr& device)
    {
        _device = device;
    }
    
    bool update(const mtlpp::Texture& color)
    {
        return update(color, el::GraphicsPixelFormatInvalid);
    }

    bool update(const mtlpp::Texture& color, el::GraphicsPixelFormat depthFormat)
    {
        if (!color)
            return false;

        if (depthFormat == el::GraphicsPixelFormatInvalid)
            _depth = ns::Handle{};
            
        else if (!_depth ||
            _depth.GetWidth() != color.GetWidth() ||
            _depth.GetHeight() != color.GetHeight() ||
            _depth.GetPixelFormat() != el::asPixelFormat(depthFormat))
        {
            el::GraphicsTextureDesc textureDesc;
            textureDesc.setWidth(color.GetWidth());
            textureDesc.setHeight(color.GetHeight());
            textureDesc.setPixelFormat(depthFormat);
            textureDesc.setTextureUsage(el::GraphicsTextureUsageDepthStencilAttachmentBit);
        
            auto depth = _device->createTexture(textureDesc);
            if (!depth)
                return false;

            _depth = std::static_pointer_cast<el::MTLTexture>(depth)->getTexture();
        }
        _color = color;
        return true;
    }
};

bool execute(NSView* view)
{
    using namespace el;

    GraphicsDeviceDesc deviceDesc;
    deviceDesc.setType(GraphicsDeviceTypeMetal);
    auto g_device = createDevice(deviceDesc);

    auto device = mtlpp::Device::CreateSystemDefaultDevice();
    auto commandQueue = device.NewCommandQueue();
    
    const auto colorFormat = GraphicsPixelFormatBGRA8Unorm;
    const auto depthFormat = GraphicsPixelFormatDepth32Float;
    
    CAMetalLayer* layer = [CAMetalLayer layer];
    layer.device = (__bridge id<MTLDevice>)device.GetPtr();
    layer.opaque = true;
    layer.drawableSize = [view convertSizeToBacking:view.bounds.size];
    layer.pixelFormat = (MTLPixelFormat)asPixelFormat(colorFormat);
    
    view.wantsLayer = YES;
    view.layer = layer;

    SwapchainHandle swapchainHandle = layer;

    typedef std::shared_ptr<MetalSwapchain> MetalSwapchainPtr;
    MetalSwapchainPtr swapchain = std::make_shared<MetalSwapchain>();
    EL_ASSERT(swapchain);
    swapchain->setDevice(g_device);
    
    GraphicsShaderDesc shaderDesc;
    shaderDesc.setStageFlag(GraphicsShaderStageVertexBit);
    shaderDesc.setShaderCode(vertexShaderSrc);
    auto g_vertexShader = g_device->createShader(shaderDesc);
    EL_ASSERT(g_vertexShader);
    const mtlpp::Function& vertFunc = std::dynamic_pointer_cast<MTLShader>(g_vertexShader)->getFunction();
    shaderDesc.setStageFlag(GraphicsShaderStageFragmentBit);
    shaderDesc.setShaderCode(fragmentShaderSrc);
    auto g_fragmentShader = g_device->createShader(shaderDesc);
    EL_ASSERT(g_fragmentShader);
    const mtlpp::Function& fragFunc = std::dynamic_pointer_cast<MTLShader>(g_fragmentShader)->getFunction();

    GraphicsTextureDesc textureDesc;
    textureDesc.setWidth(16);
    textureDesc.setHeight(16);
    textureDesc.setStream(_pixels);
    textureDesc.setPixelAlignment(GraphicsPixelAlignment::GraphicsPixelAlignment1);
    textureDesc.setPixelFormat(GraphicsPixelFormatR8Unorm);
    textureDesc.setTextureUsage(GraphicsTextureUsageSampledBit | GraphicsTextureUsageUploadableBit);

    GraphicsTexturePtr g_texture = g_device->createTexture(textureDesc);
    auto texture = std::dynamic_pointer_cast<MTLTexture>(g_texture)->getTexture();

    auto vertexBuffer = device.NewBuffer(vertexData, sizeof(vertexData), mtlpp::ResourceOptions::CpuCacheModeDefaultCache);
    
    mtlpp::RenderPipelineDescriptor renderPipelineDesc;
    renderPipelineDesc.SetVertexFunction(vertFunc);
    renderPipelineDesc.SetFragmentFunction(fragFunc);
    auto colorAttachmentDesc = renderPipelineDesc.GetColorAttachments()[0];
    colorAttachmentDesc.SetPixelFormat(asPixelFormat(colorFormat));
    colorAttachmentDesc.SetBlendingEnabled(false);
    renderPipelineDesc.SetDepthAttachmentPixelFormat(el::asPixelFormat(depthFormat));
    
    mtlpp::PipelineOption pipelineOptions = mtlpp::PipelineOption(mtlpp::ArgumentInfo | mtlpp::BufferTypeInfo);
    mtlpp::RenderPipelineReflection reflection;
    ns::Error error;
    auto renderPipelineState = device.NewRenderPipelineState(renderPipelineDesc, pipelineOptions, &reflection, &error);
    EL_ASSERT(renderPipelineState);
    
    auto context = std::make_shared<MetalContext>();
    context->setDevice(g_device);

    while (true)
    {
        bool isQuit = false;
        SDL_Event e;
        while (SDL_PollEvent(&e))
            isQuit = (e.type == SDL_QUIT);
        if (isQuit) break;
        
        @autoreleasepool
        {
            context->beginFrame(swapchainHandle);

            auto& drawable = context->getCurrentDrawable();
            
            id<CAMetalDrawable> metalDrawable = (__bridge id<CAMetalDrawable>)drawable.GetPtr();
            mtlpp::Texture color(ns::Handle{(__bridge void*)metalDrawable.texture});

            EL_ASSERT(swapchain->update(color, depthFormat));
            
            auto& commandBuffer = context->getCurrentCommandBuffer();

            mtlpp::RenderPassDescriptor renderPassDesc;
            auto colorAttachment = renderPassDesc.GetColorAttachments()[0];
            colorAttachment.SetClearColor(mtlpp::ClearColor(0.5f, 0.5f, 0.5f, 1.0f));
            colorAttachment.SetLoadAction(mtlpp::LoadAction::Clear);
            colorAttachment.SetStoreAction(mtlpp::StoreAction::Store);
            colorAttachment.SetTexture(swapchain->_color);
            colorAttachment.SetLevel(0);
            colorAttachment.SetSlice(0);
            if (swapchain->_depth)
            {
                auto depthAttachment = renderPassDesc.GetDepthAttachment();
                depthAttachment.SetTexture(swapchain->_depth);
                depthAttachment.SetClearDepth(1.0);
                depthAttachment.SetLevel(0);
                depthAttachment.SetSlice(0);
                depthAttachment.SetLoadAction(mtlpp::LoadAction::Clear);
                depthAttachment.SetStoreAction(mtlpp::StoreAction::DontCare);
            }
            renderPassDesc.setRenderTargetArrayLength(1);

            context->setDepthTestEnable(true);
            context->setDepthWriteEnable(true);
            context->setDepthCompareOp(GraphicsCompareOp::GraphicsCompareOpLess);

            auto renderCommandEncoder = commandBuffer.RenderCommandEncoder(renderPassDesc);
            renderCommandEncoder.SetCullMode(mtlpp::CullMode::Back);
            // renderCommandEncoder.SetDepthStencilState(depthStencilState);
            renderCommandEncoder.SetRenderPipelineState(renderPipelineState);
            renderCommandEncoder.SetVertexBuffer(vertexBuffer, 0, 0);
            renderCommandEncoder.SetFragmentTexture(texture, 0);
            renderCommandEncoder.Draw(mtlpp::PrimitiveType::Triangle, 0, 3);
            renderCommandEncoder.EndEncoding();

            context->present();
            context->commit();
            context->endFrame();
        }
    }

    swapchain = nullptr;
    device = ns::Handle{};

    return true;
}

int main()
{
    int y, x;
    for (y = 0;  y < 16;  y++)
    {
        for (x = 0;  x < 16;  x++)
            _pixels[y * 16 + x] = rand() % 256;
    }
    
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

    EL_ASSERT(execute(view));

    SDL_DestroyWindow(window);
    SDL_Quit();
    
    return 0;
}
