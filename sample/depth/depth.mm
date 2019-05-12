#include <el_debug.h>
#include <el_platform.h>
#include <SDL.h>
#include <SDL_syswm.h>
#include <Cocoa/Cocoa.h>
#include <Metal/Metal.h>
#include <QuartzCore/CAMetalLayer.h>
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
    
    char pixels[16 * 16];
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

namespace el
{
    namespace math
    {
    
        template <typename T>
        struct vec2
        {
            vec2() {}
            
            template <typename A>
            vec2(A v) : x(v), y(v) {} 

            template <typename A, typename B>
            vec2(A x, B y) : x(x), y(y) {}

            const T& operator[](size_t i) const {
                return v[i];
            }

            T& operator[](size_t i) {
                return v[i];
            }

            union {
                T v[2];
                struct { T x, y; };
                struct { T s, t; };
            };
        };

        template <typename T>
        struct vec4
        {
            vec4() {}
            
            template <typename A>
            vec4(A v) : x(v), y(v), z(v), w(v) {} 

            template <typename A, typename B, typename C, typename D>
            vec4(A x, B y, C z, D w) : x(x), y(y), z(z), w(w) {}

            const T& operator[](size_t i) const {
                return v[i];
            }

            T& operator[](size_t i) {
                return v[i];
            }

            union {
                float v[4];
                struct { float x, y, z, w; };
            };
        };

        typedef vec2<float> float2;
        typedef vec4<float> float4;
    }
}

namespace el
{
    typedef void* SwapchainHandle;

    class MetalContext final
    {
    public:

        MetalContext();
        ~MetalContext();

        void setDevice(const GraphicsDevicePtr& device);

        void beginFrame(SwapchainHandle handle);
        void endFrame();
        void present();
        void commit(bool isWaitComplete = false);

        mtlpp::CommandBuffer& getCommandBuffer();
        mtlpp::Drawable& getCurrentDrawable();

    private:

        MetalContext(const MetalContext&);
        MetalContext& operator=(const MetalContext&);

        GraphicsPixelFormat _depthFormat;

        MTLDevicePtr _device;
        mtlpp::Device _metalDevice;
        mtlpp::CommandBuffer _commandBuffer;
        mtlpp::Drawable _currentDrawable;
        CAMetalLayer* _metalLayer;
        
        math::float4 _clearColor;
        float _clearDepth;
        bool _isDepthWrite;
        bool _isDepthTest;
    };
}

el::MetalContext::MetalContext() :
    _depthFormat(GraphicsPixelFormatInvalid),
    _metalLayer(nullptr),
    _clearColor(0.3f),
    _clearDepth(1.f)
{
}

el::MetalContext::~MetalContext() {
    _commandBuffer = ns::Handle{};
    _metalLayer = nullptr;
    _metalDevice = ns::Handle{};
    _device = nullptr;
}

void el::MetalContext::setDevice(const el::GraphicsDevicePtr& device) {
    _device = std::static_pointer_cast<MTLDevice>(device);
    EL_ASSERT(_device);
    _metalDevice = _device->getMetalDevice();
}

mtlpp::CommandBuffer& el::MetalContext::getCommandBuffer()
{
    EL_ASSERT(_device);

    if (!_commandBuffer)
    {
        auto& queue = _device->getCommandQueue();
        EL_ASSERT(queue);
        _commandBuffer = queue.CommandBuffer();
    }
    return _commandBuffer;
}

mtlpp::Drawable& el::MetalContext::getCurrentDrawable()
{
    EL_ASSERT(_metalLayer);
    if (!_currentDrawable) {
        id<CAMetalDrawable> drawable = [_metalLayer nextDrawable];
        EL_ASSERT(drawable);
        _currentDrawable = mtlpp::Drawable(ns::Handle{(__bridge void*)drawable});
        EL_ASSERT(_currentDrawable);
    }
    return _currentDrawable;
}

void el::MetalContext::beginFrame(SwapchainHandle handle)
{
    _metalLayer = reinterpret_cast<CAMetalLayer*>(handle);
}

void el::MetalContext::endFrame()
{
    _currentDrawable = ns::Handle{};
    _commandBuffer = ns::Handle{};
    _metalLayer = nullptr;
}

void el::MetalContext::present()
{
    auto& drawable = getCurrentDrawable();
    EL_ASSERT(drawable);

    EL_ASSERT(_commandBuffer);
    _commandBuffer.Present(drawable);
}

void el::MetalContext::commit(bool isWaitComplete)
{
    EL_ASSERT(_commandBuffer);

    _commandBuffer.Commit(); 
    if (isWaitComplete)
        _commandBuffer.WaitUntilCompleted();
}

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
    textureDesc.setStream(pixels);
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

	GraphicsDepthStencilDesc _depthDesc;
    
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
            
            auto& commandBuffer = context->getCommandBuffer();

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

            context->setDepthWriteEnable();
            context->setDepthCompareOp();

            _depthDesc.setDepthWriteEnable(true);
            _depthDesc.setDepthCompareOp(GraphicsCompareOp::GraphicsCompareOpLess);
            
            static GraphicsDepthStencilDesc depthDesc;
            static GraphicsDepthStencilPtr depthStencil;

            // update status
            if (!depthStencil || (depthDesc == _depthDesc))
            {
                depthDesc = _depthDesc;
                depthStencil = g_device->createDepthStencil(_depthDesc);
            }

            EL_ASSERT(depthStencil);
			auto metalDepthStencil = std::static_pointer_cast<MTLDepthStencil>(depthStencil);
            
            auto& depthStencilState = metalDepthStencil->getMetalDepthStencilState();
            
            auto renderCommandEncoder = commandBuffer.RenderCommandEncoder(renderPassDesc);
            renderCommandEncoder.SetCullMode(mtlpp::CullMode::Back);
            renderCommandEncoder.SetDepthStencilState(depthStencilState);
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
            pixels[y * 16 + x] = rand() % 256;
    }
    
    SDL_Init(SDL_INIT_VIDEO);
    
    SDL_Window* window = SDL_CreateWindow("sdl sample",
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
