#include "platform_wgl.h"

#include <mutex>
#include <el/debug.h>
#include <glad/glad.h>

#include <Windows.h>
#include <GL/GL.h>

#include "wgl.h"
#include "wglext.h"

#pragma comment(lib, "opengl32.lib")

namespace el {

struct PlatformCocoaGLImpl 
{
    HDC hdc;
    HWND hwnd;
    HGLRC context;
};

PlatformWGL::PlatformWGL() :
    _impl(new PlatformCocoaGLImpl)
{
    _impl->hdc = NULL;
    _impl->hwnd = NULL;
    _impl->context = nullptr;
}

PlatformWGL::~PlatformWGL()
{
    destroy();
}

bool PlatformWGL::create(void* window)
{
    PIXELFORMATDESCRIPTOR pfd = {
        sizeof(PIXELFORMATDESCRIPTOR),
        1,
        PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,    // Flags
        PFD_TYPE_RGBA,        // The kind of framebuffer. RGBA or palette.
        32,                   // Colordepth of the framebuffer.
        0, 0, 0, 0, 0, 0,
        0,
        0,
        0,
        0, 0, 0, 0,
        24,                   // Number of bits for the depthbuffer
        0,                    // Number of bits for the stencilbuffer
        0,                    // Number of Aux buffers in the framebuffer.
        PFD_MAIN_PLANE,
        0,
        0, 0, 0
    };

    _impl->hwnd = reinterpret_cast<HWND>(window);
    HDC hdc = _impl->hdc = ::GetDC(_impl->hwnd);

    int pixelFormat = ChoosePixelFormat(hdc, &pfd);
    if (!pixelFormat)
        return false;
    if (!DescribePixelFormat(hdc, pixelFormat, sizeof(PIXELFORMATDESCRIPTOR), &pfd))
        return false;
    if (!SetPixelFormat(hdc, pixelFormat, &pfd))
        return false;

    HGLRC dummyContext = wglCreateContext(hdc);
    if (!dummyContext)
        return false;

    if (!wglMakeCurrent(hdc, dummyContext))
    {
        wglDeleteContext(dummyContext);
        destroy();
        return false;
    }

    PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribs =
        (PFNWGLCREATECONTEXTATTRIBSARBPROC)wglGetProcAddress("wglCreateContextAttribsARB");

    if (wglCreateContextAttribs)
    {
        int attribs[] = {
            WGL_CONTEXT_MAJOR_VERSION_ARB, 4,
            WGL_CONTEXT_MINOR_VERSION_ARB, 1,
            WGL_CONTEXT_FLAGS_ARB, WGL_CONTEXT_DEBUG_BIT_ARB,
            WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB,
            0, 0
        };

        _impl->context = wglCreateContextAttribs(hdc, nullptr, attribs);
    }
    else
    {
        _impl->context = wglCreateContext(hdc);
    }

    wglMakeCurrent(NULL, NULL);
    wglDeleteContext(dummyContext);
    dummyContext = nullptr;

    if (!_context || !wglMakeCurrent(hdc, _impl->context)) {
        DWORD dwError = GetLastError();
        if (dwError == (0xc0070000 | ERROR_INVALID_VERSION_ARB))
            EL_TRACE("WGL: Driver does not support OpenGL version");
        else if (dwError == (0xc0070000 | ERROR_INVALID_PROFILE_ARB))
            EL_TRACE("WGL: Driver does not support the requested OpenGL profile");
        else if (dwError == (0xc0070000 | ERROR_INCOMPATIBLE_DEVICE_CONTEXTS_ARB))
            EL_TRACE("WGL: The share context is not compatible with the requested context");
        else
            EL_TRACE("WGL: Failed to create OpenGL context");
        destroy();
        return false;
    }

    static std::mutex g_library_mutex;
    static bool isLoadGL = false;

    std::lock_guard<std::mutex> lock(g_library_mutex);
    if (isLoadGL)
        return true;
    gladLoadGL();

    return true;
}

void PlatformWGL::destroy()
{
    wglMakeCurrent(NULL, NULL);
    if (_impl->context) {
        wglDeleteContext(_context);
        _impl->context = nullptr;
    }
    if (_impl->hwnd && _impl->hdc) {
        ReleaseDC(_impl->hwnd, _impl->hdc);
        _impl->hdc = NULL;
    }
    _impl->hwnd = NULL;
    delete _impl;
    _impl = nullptr;
}

void PlatformWGL::swapBuffer()
{
    SwapBuffers(_impl->hdc);
}

void PlatformWGL::makeCurrent()
{
    if (!wglMakeCurrent(_impl->hdc, _impl->context)) {
        // reportLastWindowsError();
        return;
    }
    return;
}

} // namespace el
