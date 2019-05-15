#include <SDL.h>
#include <SDL_syswm.h>
#include <Cocoa/Cocoa.h>
#include <QuartzCore/CAMetalLayer.h>
#include <Metal/Metal.h>
#include <mtlpp.hpp>
#include <el_debug.h>

#include <Metal/mtl_types.h>
#include <Metal/mtl_texture.h>
#include <Metal/mtl_shader.h>
#include <Metal/mtl_depth_stencil.h>
#include <Metal/mtl_device.h>
#include <Metal/mtl_context.h>

#include "renderer.h"
#include "metal_driver.h"

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
