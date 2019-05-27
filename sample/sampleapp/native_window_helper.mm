#include <SDL.h>
#include <SDL_syswm.h>
#include <QuartzCore/CAMetalLayer.h>
#if EL_PLAT_OSX
#include <Cocoa/Cocoa.h>
#else
#import <UIKit/UIKit.h>
#endif

void *getNativeWindow(SDL_Window *window)
{
    SDL_SysWMinfo info;
    SDL_GetVersion(&info.version);
    SDL_GetWindowWMInfo(window, &info);
#if EL_PLAT_OSX
    NSView* view = [info.info.cocoa.window contentView];
#else
    UIView *view = info.info.uikit.window.rootViewController.view;
#endif
    return (__bridge void*)view;
}

// In case iOS follow
// https://gist.github.com/gcatlin/b89e0efed78dd91364609ca4095da346
//
//   SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC);
//   const CAMetalLayer *swapchain = (__bridge CAMetalLayer *)SDL_RenderGetMetalLayer(renderer);
//   const id<MTLDevice> gpu = swapchain.device;
//   SDL_DestroyRenderer(renderer);

#if EL_PLAT_OSX
void *setupMetalLayer(void *nativeView) 
{
    NSView *view = (__bridge NSView *)nativeView;
    CAMetalLayer* layer = [CAMetalLayer layer];
    layer.opaque = true;
    layer.drawableSize = [view convertSizeToBacking:view.bounds.size];
    layer.pixelFormat = MTLPixelFormatBGRA8Unorm;
    layer.framebufferOnly = YES;
    layer.bounds = view.bounds;
    [view setLayer:layer];
    return (__bridge void*)layer;
}
#endif
