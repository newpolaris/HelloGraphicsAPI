#include <el_debug.h>
#include <el_platform.h>
#include <SDL.h>
#include <SDL_syswm.h>
#include <Cocoa/Cocoa.h>
#include <Metal/Metal.h>
#include <Metal/MTLLibrary.h>
#include <QuartzCore/CAMetalLayer.h>
#include <mtlpp.hpp>

#include <graphics_device.h>
#include <graphics_texture.h>
#include <graphics_shader.h>
#include <graphics_data.h>
#include <graphics_program.h>

#undef EL_BUILD_METAL
#define EL_BUILD_METAL 1
#include <Metal/mtl_texture.h>
#include <Metal/mtl_shader.h>

namespace {

    const char vertexShaderSrc[] = R"""(
        #include <metal_stdlib>
        #include <simd/simd.h>
        using namespace metal;
    
        namespace AAPL
        {
            struct constants_t
            {
                simd::float4x4 modelview_projection_matrix;
                simd::float4x4 normal_matrix;
                simd::float4   ambient_color;
                simd::float4   diffuse_color;
                int            multiplier;
            } __attribute__ ((aligned (256)));
        }

        typedef struct
        {
            packed_float3 position;
            packed_float2 texcoord;
        } vertex_t;
        struct VertexInput {
            float3 position [[attribute(0)]];
            float3 normal   [[attribute(1)]];
            half2  texcoord [[attribute(2)]];
        };

        typedef struct
        {
            float4 clipSpacePosition [[position]];
            float2 textureCoordinate;
        } RasterizerData;

        vertex RasterizerData Main(
            VertexInput in [[stage_in]],
            const device vertex_t* vertexArray [[buffer(2)]],
            constant AAPL::constants_t& constants [[buffer(1)]],
            constant float& light [[buffer(0)]],
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

enum class ArgumentAccess : uint8_t
{
    ReadOnly  = 0,
    ReadWrite = 1,
    WriteOnly = 2,
};

struct MTLUniform
{
    bool isActive;
    std::string name;
    uint32_t location;
    uint32_t arraySize;
    uint32_t alignment;
    uint32_t size;
    ArgumentAccess access;
};

void setupArgument(const mtlpp::Argument& arg)
{
    MTLUniform uniform;
    uniform.name = std::string(arg.GetName().GetCStr());
    uniform.isActive = arg.IsActive();
    uniform.access = (ArgumentAccess)arg.GetAccess();
    uniform.location = arg.GetIndex();
    
    @autoreleasepool {
        MTLArgument* argument = (__bridge MTLArgument*)arg.GetPtr();
        
        MTLDataType dataType = [argument bufferDataType];
        MTLStructType* structType = [argument bufferStructType];
    }

    auto type = arg.GetType();
    if (type == mtlpp::ArgumentType::Buffer)
    {
        uint32_t arraySize = 1;
        uniform.alignment = arg.GetBufferAlignment();
        uniform.size = arg.GetBufferDataSize();
        auto bufferType = arg.GetBufferDataType();
        if (bufferType == mtlpp::DataType::Array)
            arraySize = arg.GetArrayLength();
        else if (bufferType == mtlpp::DataType::Struct)
        {
            // uniform.type = bufferType;
        }
        // uniform.elementSize = asElementSize(bufferType);
    }
    else if (type == mtlpp::ArgumentType::Sampler)
    {
        
    }
    else if (type == mtlpp::ArgumentType::Texture)
    {
    
    }
}

bool execute(NSView* view)
{
    using namespace el;

    GraphicsDeviceDesc deviceDesc;
    deviceDesc.setType(GraphicsDeviceTypeMetal);
    auto g_device = createDevice(deviceDesc);

    auto device = mtlpp::Device::CreateSystemDefaultDevice();
    auto commandQueue = device.NewCommandQueue();
    
    CAMetalLayer* layer = [CAMetalLayer layer];
    layer.device = (__bridge id<MTLDevice>)device.GetPtr();
    layer.opaque = true;
    layer.drawableSize = [view convertSizeToBacking:view.bounds.size];

    view.wantsLayer = YES;
    view.layer = layer;

    GraphicsShaderDesc shaderDesc;
    shaderDesc.setStageFlag(GraphicsShaderStageVertexBit);
    shaderDesc.setShaderCode(vertexShaderSrc);
    auto g_vertexShader = g_device->createShader(shaderDesc);
    EL_ASSERT(g_vertexShader);
    const mtlpp::Function& vertFunc = std::dynamic_pointer_cast<el::MTLShader>(g_vertexShader)->getFunction();
    shaderDesc.setStageFlag(GraphicsShaderStageFragmentBit);
    shaderDesc.setShaderCode(fragmentShaderSrc);
    auto g_fragmentShader = g_device->createShader(shaderDesc);
    EL_ASSERT(g_fragmentShader);
    const mtlpp::Function& fragFunc = std::dynamic_pointer_cast<el::MTLShader>(g_fragmentShader)->getFunction();

    auto qt = vertFunc.GetVertexAttributes();
    
    GraphicsTextureDesc textureDesc;
    textureDesc.setWidth(16);
    textureDesc.setHeight(16);
    textureDesc.setStream(pixels);
    textureDesc.setPixelAlignment(GraphicsPixelAlignment::GraphicsPixelAlignment1);
    textureDesc.setPixelFormat(GraphicsPixelFormat::GraphicsPixelFormatR8Unorm);
    textureDesc.setTextureUsage(GraphicsTextureUsageFlagBitSampledBit);

    GraphicsTexturePtr g_texture = g_device->createTexture(textureDesc);
    auto texture = std::dynamic_pointer_cast<el::MTLTexture>(g_texture)->getTexture();

    auto vertexBuffer = device.NewBuffer(vertexData, sizeof(vertexData), mtlpp::ResourceOptions::CpuCacheModeDefaultCache);
    
    MTLVertexDescriptor *mtlVertexDescriptor = [[MTLVertexDescriptor alloc] init];
    
    // Positions.
    mtlVertexDescriptor.attributes[0].format = MTLVertexFormatFloat3;
    mtlVertexDescriptor.attributes[0].offset = 0;
    mtlVertexDescriptor.attributes[0].bufferIndex = 0;
    
    // Normals.
    mtlVertexDescriptor.attributes[1].format = MTLVertexFormatFloat3;
    mtlVertexDescriptor.attributes[1].offset = 12;
    mtlVertexDescriptor.attributes[1].bufferIndex = 0;
    
    // Texture coordinates.
    mtlVertexDescriptor.attributes[2].format = MTLVertexFormatHalf2;
    mtlVertexDescriptor.attributes[2].offset = 24;
    mtlVertexDescriptor.attributes[2].bufferIndex = 0;
    
    // Single interleaved buffer.
    mtlVertexDescriptor.layouts[0].stride = 28;
    mtlVertexDescriptor.layouts[0].stepRate = 1;
    mtlVertexDescriptor.layouts[0].stepFunction = MTLVertexStepFunctionPerVertex;

    MTLVertexBufferLayoutDescriptor;
    
    mtlpp::VertexDescriptor vertexDescriptor = ns::Handle{(__bridge mtlpp::VertexDescriptor*)mtlVertexDescriptor};
    mtlpp::RenderPipelineDescriptor renderPipelineDesc;
    renderPipelineDesc.SetVertexDescriptor(vertexDescriptor);
    renderPipelineDesc.SetVertexFunction(vertFunc);
    renderPipelineDesc.SetFragmentFunction(fragFunc);
    auto colorAttachmentDesc = renderPipelineDesc.GetColorAttachments()[0];
    colorAttachmentDesc.SetPixelFormat(mtlpp::PixelFormat::BGRA8Unorm);
    mtlpp::PipelineOption pipelineOptions = mtlpp::PipelineOption(mtlpp::ArgumentInfo | mtlpp::BufferTypeInfo);
    mtlpp::RenderPipelineReflection reflection;
    ns::Error error;
    auto renderPipelineState = device.NewRenderPipelineState(renderPipelineDesc, pipelineOptions, &reflection, &error);
    EL_ASSERT(renderPipelineState);

    auto vertexArgs = reflection.GetVertexArguments();
    for (int i = 0; i < vertexArgs.GetSize(); i++)
    {
        setupArgument(vertexArgs[i]);
    }
    
    mtlpp::RenderPassDescriptor renderPassDesc;
    auto colorAttachment = renderPassDesc.GetColorAttachments()[0];
    colorAttachment.SetClearColor(mtlpp::ClearColor{0.5f, 0.5f, 0.5f, 1.0f});
    colorAttachment.SetLoadAction(mtlpp::LoadAction::Clear);
    colorAttachment.SetStoreAction(mtlpp::StoreAction::Store);
    
    while (true)
    {
        bool isQuit = false;
        SDL_Event e;
        while (SDL_PollEvent(&e))
            isQuit = (e.type == SDL_QUIT);
        if (isQuit) break;

        @autoreleasepool
        {
            id<CAMetalDrawable> drawable = [layer nextDrawable];
            
            mtlpp::CommandBuffer commandBuffer = commandQueue.CommandBuffer();

            if (drawable)
            {
                colorAttachment.SetTexture(mtlpp::Texture(ns::Handle{(__bridge void*)drawable.texture}));

                auto renderCommandEncoder = commandBuffer.RenderCommandEncoder(renderPassDesc);
                renderCommandEncoder.SetCullMode(mtlpp::CullMode::Back);
                renderCommandEncoder.SetRenderPipelineState(renderPipelineState);
                renderCommandEncoder.SetVertexBuffer(vertexBuffer, 0, 0);
                renderCommandEncoder.SetFragmentTexture(texture, 0);
                renderCommandEncoder.Draw(mtlpp::PrimitiveType::Triangle, 0, 3);
                renderCommandEncoder.EndEncoding();
                commandBuffer.Present(ns::Handle{(__bridge void*)drawable});
            }

            commandBuffer.Commit();
            commandBuffer.WaitUntilCompleted();
        }
    }

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
