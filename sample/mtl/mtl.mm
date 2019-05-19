#include <SDL.h>
#include <SDL_syswm.h>
#include <Cocoa/Cocoa.h>
#include <mtlpp.hpp>
#include <el_debug.h>

#include <Metal/mtl_types.h>
#include <Metal/mtl_texture.h>
#include <Metal/mtl_shader.h>
#include <Metal/mtl_device.h>
#include <Metal/mtl_context.h>
#include <Metal/Metal.h>

#include <el_utility.h>
#include <math_types.h>

#include "metal_driver.h"
#include "metal_resources.h"
#include "metal_states.h"
#include "native_window_helper.h"

const char vertexShaderSrc[] = R"""(
#include <metal_stdlib>
using namespace metal;

vertex float4 main0(
                    const device packed_float3* vertexArray [[buffer(0)]],
                    unsigned int vID[[vertex_id]])
{
    return float4(vertexArray[vID], 1.0);
}
)""";

const char fragmentShaderSrc[] = R"""(
#include <metal_stdlib>
using namespace metal;
fragment half4 fragFunc()
{
    return half4(1.0, 0.0, 0.0, 1.0);
}
)""";

int main()
{
    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window* window = SDL_CreateWindow("sdl sample",
            SDL_WINDOWPOS_UNDEFINED,
            SDL_WINDOWPOS_UNDEFINED,
            1280, 768,
            SDL_WINDOW_RESIZABLE);

    EL_ASSERT(window != nullptr);

    void *view = ::getNativeWindow(window);

    const auto colorFormat = el::GraphicsPixelFormatBGRA8Unorm;
    const auto depthFormat = el::GraphicsPixelFormatDepth32Float;
    void *nativeSurface = setupMetalLayer(view);
    
    el::MetalDriver driver;
    driver.setup(nativeSurface);
    
    el::setupRenderPasses();
    
    auto clearSurface = el::renderPassColor;
    clearSurface.clearColor = el::math::float4(0.2f, 0.4f, 0.6f, 1.0f);
    
    auto program = driver.createProgram(vertexShaderSrc, fragmentShaderSrc);
    
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
            driver.beginFrame();
            driver.beginRenderPass(clearSurface);
            driver.drawFrame(program);
            driver.endRenderPass();
            driver.commit();
            
#if __has_feature(objc_arc)
        }
#endif
    }

    el::cleanupPipeline();
    program.destroy();
    driver.cleanup();

    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
