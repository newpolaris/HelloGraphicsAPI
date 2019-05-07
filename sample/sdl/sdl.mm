#include <el_debug.h>
#include <el_platform.h>
#include <SDL.h>
#include <SDL_syswm.h>
#include <Cocoa/Cocoa.h>
#include <Metal/Metal.h>
#include <QuartzCore/CAMetalLayer.h>
#include <mtlpp.hpp>

namespace {

    const char shadersSrc[] = R"""(
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

        vertex RasterizerData vertFunc(
            const device vertex_t* vertexArray [[buffer(0)]],
            unsigned int vID[[vertex_id]])
        {
            RasterizerData data;
            data.clipSpacePosition = float4(vertexArray[vID].position, 1.0);
            data.textureCoordinate = vertexArray[vID].texcoord;
            return data;
        }

        fragment half4 fragFunc(
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

}

bool execute(NSView* view)
{
    auto device = mtlpp::Device::CreateSystemDefaultDevice();
    
    CAMetalLayer* layer = [CAMetalLayer layer];
    layer.device = (__bridge id<MTLDevice>)device.GetPtr();
    layer.opaque = true;
    layer.drawableSize = [view convertSizeToBacking:view.bounds.size];

    view.wantsLayer = YES;
    view.layer = layer;

    auto commandQueue = device.NewCommandQueue();

    ns::Error error;
    mtlpp::Library library = device.NewLibrary(shadersSrc, mtlpp::CompileOptions(), &error);
    if (!library)
        EL_TRACE("Failed to created pipeline state, error %s", error.GetLocalizedDescription().GetCStr());

    mtlpp::Function vertFunc = library.NewFunction("vertFunc");
    mtlpp::Function fragFunc = library.NewFunction("fragFunc");
    
    char pixels[16 * 16];
    int y, x;
    for (y = 0;  y < 16;  y++)
    {
        for (x = 0;  x < 16;  x++)
            pixels[y * 16 + x] = rand() % 256;
    }

    mtlpp::TextureDescriptor descriptor;
    descriptor.SetWidth(16);
    descriptor.SetHeight(16);
    descriptor.SetPixelFormat(mtlpp::PixelFormat::R8Unorm);

    auto texture = device.NewTexture(descriptor);
    EL_ASSERT(texture);
    mtlpp::Region region = { 0, 0, descriptor.GetWidth(), descriptor.GetHeight() };
    texture.Replace(region, 0, pixels, 16);

    auto vertexBuffer = device.NewBuffer(vertexData, sizeof(vertexData), mtlpp::ResourceOptions::CpuCacheModeDefaultCache);

    mtlpp::RenderPipelineDescriptor renderPipelineDesc;
    renderPipelineDesc.SetVertexFunction(vertFunc);
    renderPipelineDesc.SetFragmentFunction(fragFunc);
    renderPipelineDesc.GetColorAttachments()[0].SetPixelFormat(mtlpp::PixelFormat::BGRA8Unorm);
    auto renderPipelineState = device.NewRenderPipelineState(renderPipelineDesc, nullptr);

    
    mtlpp::RenderPassDescriptor renderPassDesc;
    mtlpp::RenderPassColorAttachmentDescriptor colorAttachment = renderPassDesc.GetColorAttachments()[0];
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
        
        MTLViewport viewport = (MTLViewport){
            0.0, 0.0,
            layer.drawableSize.width,
            layer.drawableSize.height,
            0.0, 1.0};
        
        @autoreleasepool
        {
            id<CAMetalDrawable> drawable = [layer nextDrawable];
            
            mtlpp::CommandBuffer commandBuffer = commandQueue.CommandBuffer();

            if (drawable)
            {
                colorAttachment.SetTexture(mtlpp::Texture(ns::Handle{(__bridge void*)drawable.texture}));

                mtlpp::RenderCommandEncoder renderCommandEncoder = commandBuffer.RenderCommandEncoder(renderPassDesc);
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
