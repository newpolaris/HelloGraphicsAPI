#include "native_window_helper.h"

#include <el/debug.h>
#include <SDL_syswm.h>

void* getNativeWindow(SDL_Window* window)
{
    SDL_SysWMinfo info;
    SDL_GetVersion(&info.version);
    SDL_GetWindowWMInfo(window, &info);
    HWND win = (HWND)info.info.win.window;
    return (void*)win;
}
