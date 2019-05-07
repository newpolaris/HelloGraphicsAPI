#include "wglcontext.h"

#if EL_PLAT_WINDOWS

#define ERROR_INVALID_VERSION_ARB 0x2095
#define ERROR_INVALID_PROFILE_ARB 0x2096
#define ERROR_INCOMPATIBLE_DEVICE_CONTEXTS_ARB 0x2054

namespace {

    void reportLastWindowsError() {
        LPSTR lpMessageBuffer = nullptr;
        DWORD dwError = GetLastError();

        if (dwError == 0) {
            return;
        }
        FormatMessage(
            FORMAT_MESSAGE_ALLOCATE_BUFFER |
            FORMAT_MESSAGE_FROM_SYSTEM |
            FORMAT_MESSAGE_IGNORE_INSERTS,
            nullptr,
            dwError,
            MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
            lpMessageBuffer,
            0, nullptr
        );

        EL_TRACE("Windows error code: %d . %s\n", dwError, lpMessageBuffer);
        LocalFree(lpMessageBuffer);
    }

}

namespace el
{
    PFN_wglGetProcAddress wglGetProcAddress;
    PFN_wglMakeCurrent wglMakeCurrent;
    PFN_wglCreateContext wglCreateContext;
    PFN_wglDeleteContext wglDeleteContext;
    PFN_wglGetCurrentDC wglGetCurrentDC;
    PFN_wglGetCurrentContext wglGetCurrentContext;
    PFN_wglShareLists wglShareLists;

    // This mutex protect g_library_refcount below.
    static std::mutex g_library_mutex;

    bool initGLExtention();
    bool initGL();
}

bool el::initGLExtention()
{
    std::lock_guard<std::mutex> lock(g_library_mutex);
    static bool isInitGLExtention = false;
    if (isInitGLExtention)
        return true;

    HINSTANCE instance;
    instance = LoadLibraryA("opengl32.dll");
    if (!instance)
    {
        EL_TRACE("WGL: Failed to load opengl32.dll");
        return false;
    }

    wglGetProcAddress = (PFN_wglGetProcAddress)
        GetProcAddress(instance, "wglGetProcAddress");
    wglMakeCurrent = (PFN_wglMakeCurrent)
        GetProcAddress(instance, "wglMakeCurrent");
    wglCreateContext = (PFN_wglCreateContext)
        GetProcAddress(instance, "wglCreateContext");
    wglDeleteContext = (PFN_wglDeleteContext)
        GetProcAddress(instance, "wglDeleteContext");
    wglGetCurrentDC = (PFN_wglGetCurrentDC)
        GetProcAddress(instance, "wglGetCurrentDC");
    wglGetCurrentContext = (PFN_wglGetCurrentContext)
        GetProcAddress(instance, "wglGetCurrentContext");
    wglShareLists = (PFN_wglShareLists)
        GetProcAddress(instance, "wglShareLists");

    if (instance)
        FreeLibrary(instance);
    
    isInitGLExtention = true;
    return true;
}

bool el::initGL()
{
    std::lock_guard<std::mutex> lock(g_library_mutex);
    static bool isInitGL = false;
    if (isInitGL)
        return true;
    gladLoadGL();
    return true;
}

#endif // EL_PLAT_WINDOWS
