#include <cmath>
#include <SDL.h>
#include <el_debug.h>
#include <el_utility.h>
#include <el_math.h>
#include <graphics_data.h>
#include <graphics_texture.h>

#include <graphics_platform.h>
#include <graphics_driver.h>
#include <graphics_pipeline.h>
#include <native_window_helper.h>

namespace el {
    
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
    
    vertex RasterizerData main0(
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
    
    // AAPLVertex improve;
    const float vertexData[] =
    {
        0.0f,  1.0f, 0.0f,  0.5f, 0.0f,
        1.0f, -1.0f, 0.0f,  1.0f, 1.0f,
        -1.0f, -1.0f, 0.0f,  0.0f, 1.0f,
    };
    
    char _pixels[16 * 16];
}

int main()
{
    int y, x;
    for (y = 0;  y < 16;  y++)
    {
        for (x = 0;  x < 16;  x++)
            el::_pixels[y * 16 + x] = rand() % 256;
    }
    
    SDL_Init(SDL_INIT_VIDEO);
    
    SDL_Window* window = SDL_CreateWindow("triangle-sample",
                                          SDL_WINDOWPOS_UNDEFINED,
                                          SDL_WINDOWPOS_UNDEFINED,
                                          1280, 768,
                                          SDL_WINDOW_RESIZABLE);
    
    EL_ASSERT(window != nullptr);
    
    void *view = ::getNativeWindow(window);
    void *nativeSurface = setupMetalLayer(view);
    
    auto platform = el::DefaultPlatform::create(el::GraphicsDeviceTypeMetal);
    auto driver = platform->createDriver(nullptr);
    
    driver->setup(nativeSurface);
    
    el::RenderPassParms params {};
    params.flags.clear = el::GraphicsTargetBufferFlagBitColor;
    params.clearColor = el::math::float4(0.2f, 0.4f, 0.6f, 1.0f);
    
    auto vertexBuffer = driver->createVertexBuffer(el::vertexData, 3, sizeof(float)*5);
    auto program = driver->createProgram(el::vertexShaderSrc, el::fragmentShaderSrc);
    
    el::GraphicsTextureDesc textureDesc;
    textureDesc.setWidth(16);
    textureDesc.setHeight(16);
    textureDesc.setStream((el::stream_t*)el::_pixels);
    textureDesc.setStreamSize(16*16);
    textureDesc.setPixelFormat(el::GraphicsPixelFormatR8Unorm);
    textureDesc.setTextureUsage(el::GraphicsTextureUsageSampledBit | el::GraphicsTextureUsageUploadableBit);
    auto texture = driver->createTexture(textureDesc);
    EL_ASSERT(texture);
    
    auto defaultTarget = driver->createDefaultRenderTarget();
    
    el::PipelineState pipelineState {};
    pipelineState.program = program;
    
    struct DrawCommands
    {
        uint32_t vertexCount;
        uint32_t vertexOffset;
    };
    
    while (true)
    {
        bool isQuit = false;
        SDL_Event e;
        while (SDL_PollEvent(&e))
            isQuit = (e.type == SDL_QUIT);
        if (isQuit) break;
        
#if __has_feature(objc_arc)
        @autoreleasepool {
#endif
            driver->beginFrame();
            driver->beginRenderPass(defaultTarget, params);
            driver->setVertexBuffer(vertexBuffer, 0);
            driver->setFragmentTexture(texture, 0);
            driver->setPipelineState(pipelineState);
            driver->draw(el::GraphicsPrimitiveTypeTriangle, 3, 0);
            driver->endRenderPass();
            driver->commit();
            
#if __has_feature(objc_arc)
        }
#endif
    }
    program = nullptr;
    vertexBuffer = nullptr;
    texture = nullptr;
    driver->cleanup();
    
    el::DefaultPlatform::destroy(&platform);
    
    SDL_DestroyWindow(window);
    SDL_Quit();
    
    return 0;
}
