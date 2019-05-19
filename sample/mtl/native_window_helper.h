#ifndef __NATIVE_WINDOW_HELPER_H__
#define __NATIVE_WINDOW_HELPER_H__

#include <SDL.h>

void *setupMetalLayer(void *nativeView);
void *getNativeWindow(SDL_Window *window);

#endif // #define __NATIVE_WINDOW_HELPER_H__
