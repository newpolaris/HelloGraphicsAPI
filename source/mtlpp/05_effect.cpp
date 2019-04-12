#include "../mtlpp.hpp"
#include "window.hpp"
#include <string>
#include "debug.h"

#if 1

mtlpp::Device              g_device;
mtlpp::CommandQueue        g_commandQueue;
mtlpp::Buffer              g_vertexBuffer;
mtlpp::Texture             g_texture;
mtlpp::RenderPipelineState g_renderPipelineState;

void Render(const Window& win)
{
    mtlpp::CommandBuffer commandBuffer = g_commandQueue.CommandBuffer();
    
    mtlpp::RenderPassDescriptor renderPassDesc = win.GetRenderPassDescriptor();
    if (renderPassDesc)
    {
        mtlpp::RenderCommandEncoder renderCommandEncoder = commandBuffer.RenderCommandEncoder(renderPassDesc);
        renderCommandEncoder.SetCullMode(mtlpp::CullMode::Back);
        renderCommandEncoder.SetRenderPipelineState(g_renderPipelineState);
        renderCommandEncoder.SetVertexBuffer(g_vertexBuffer, 0, 0);
        renderCommandEncoder.SetFragmentTexture(g_texture, 0);
        renderCommandEncoder.Draw(mtlpp::PrimitiveType::Triangle, 0, 3);
        renderCommandEncoder.EndEncoding();
        commandBuffer.Present(win.GetDrawable());
    }

    commandBuffer.Commit();
    commandBuffer.WaitUntilCompleted();
}

int main()
{
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
    g_device = mtlpp::Device::CreateSystemDefaultDevice();
    g_commandQueue = g_device.NewCommandQueue();

    ns::Error error;
    mtlpp::Library library = g_device.NewLibrary(shadersSrc, mtlpp::CompileOptions(), &error);
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

    g_texture = g_device.NewTexture(descriptor);
    assert(g_texture);
    mtlpp::Region region = { 0, 0, descriptor.GetWidth(), descriptor.GetHeight() };
    g_texture.Replace(region, 0, pixels, 16);
    g_vertexBuffer = g_device.NewBuffer(vertexData, sizeof(vertexData), mtlpp::ResourceOptions::CpuCacheModeDefaultCache);

    mtlpp::RenderPipelineDescriptor renderPipelineDesc;
    renderPipelineDesc.SetVertexFunction(vertFunc);
    renderPipelineDesc.SetFragmentFunction(fragFunc);
    renderPipelineDesc.GetColorAttachments()[0].SetPixelFormat(mtlpp::PixelFormat::BGRA8Unorm);
    g_renderPipelineState = g_device.NewRenderPipelineState(renderPipelineDesc, nullptr);

    Window win(g_device, &Render, 320, 240);
    Window::Run();

    return 0;
}

#endif
