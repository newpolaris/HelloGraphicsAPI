#include <SDL.h>
#include <el/debug.h>
#include <native_window_helper.h>

int main()
{
    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window* window = SDL_CreateWindow("sample-cow",
            SDL_WINDOWPOS_UNDEFINED,
            SDL_WINDOWPOS_UNDEFINED,
            1280, 768,
            SDL_WINDOW_RESIZABLE);

    EL_ASSERT(window != nullptr);

    void *view = ::getNativeWindow(window);
    void *nativeSurface = ::setupMetalLayer(view);

    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
