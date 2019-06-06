#ifndef __NATIVE_WINDOW_HELPER_H__
#define __NATIVE_WINDOW_HELPER_H__

struct SDL_Window;

extern "C" void* getNativeWindow(SDL_Window *window);

#if defined(__APPLE__)
// Add a backing CAMetalLayer to the NSView and return the layer.
extern "C" void* setupMetalLayer(void* nativeView);
// Resize the backing CAMetalLayer's drawable to match the new view's size. Returns the layer.
extern "C" void* resizeMetalLayer(void* natviewView);
#endif 

#endif // __NATIVE_WINDOW_HELPER_H__
