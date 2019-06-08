#include "wgl_context.h"

#include <mutex>
#include <glad/glad.h>
#include <el/debug.h>

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
    PFNWGLGETPROCADDRESSPROC wglGetProcAddress;
    PFNWGLMAKECURRENTPROC wglMakeCurrent;
    PFNWGLCREATECONTEXTPROC wglCreateContext;
    PFNWGLDELETECONTEXTPROC wglDeleteContext;
    PFNWGLGETCURRENTDCPROC wglGetCurrentDC;
    PFNWGLGETCURRENTCONTEXTPROC wglGetCurrentContext;
    PFNWGLSHARELISTSPROC wglShareLists;

    // This mutex protect g_library_refcount below.
    static std::mutex g_library_mutex;
}

bool el::initializeWGL()
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

    wglGetProcAddress = (PFNWGLGETPROCADDRESSPROC)GetProcAddress(instance, "wglGetProcAddress");
    wglMakeCurrent = (PFNWGLMAKECURRENTPROC)GetProcAddress(instance, "wglMakeCurrent");
    wglCreateContext = (PFNWGLCREATECONTEXTPROC)GetProcAddress(instance, "wglCreateContext");
    wglDeleteContext = (PFNWGLDELETECONTEXTPROC)GetProcAddress(instance, "wglDeleteContext");
    wglGetCurrentDC = (PFNWGLGETCURRENTDCPROC)GetProcAddress(instance, "wglGetCurrentDC");
    wglGetCurrentContext = (PFNWGLGETCURRENTCONTEXTPROC)GetProcAddress(instance, "wglGetCurrentContext");
    wglShareLists = (PFNWGLSHARELISTSPROC)GetProcAddress(instance, "wglShareLists");

    if (instance)
        FreeLibrary(instance);
    
    isInitGLExtention = true;
    return true;
}

bool el::loadGLFunctions()
{
    std::lock_guard<std::mutex> lock(g_library_mutex);
    static bool isInitGL = false;
    if (isInitGL)
        return true;
    gladLoadGL();
    return true;
}
