#include <SDL.h>
#include <SDL_syswm.h>
#include <Cocoa/Cocoa.h>
#include <QuartzCore/CAMetalLayer.h>

void *getNativeWindow(SDL_Window *window)
{
    SDL_SysWMinfo info;
    SDL_GetVersion(&info.version);
    SDL_GetWindowWMInfo(window, &info);
    NSView* view = [info.info.cocoa.window contentView];
    return (__bridge void*)view;
}

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
