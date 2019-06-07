#include "platform_driver_wgl.h"

#include <el/debug.h>
#include "wgl_context.h"

#define ERROR_INVALID_VERSION_ARB 0x2095
#define ERROR_INVALID_PROFILE_ARB 0x2096
#define ERROR_INCOMPATIBLE_DEVICE_CONTEXTS_ARB 0x2054

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

el::PlatformDriverWGL::PlatformDriverWGL() :
    _hdc(NULL),
    _context(NULL),
    _hwnd(NULL)
{
}

bool el::PlatformDriverWGL::create(void* window)
{
    EL_ASSERT(el::initGLExtention());

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

    _hwnd = reinterpret_cast<HWND>(window);
    _hdc = ::GetDC(_hwnd);

    int pixelFormat = ChoosePixelFormat(_hdc, &pfd);
    if (!pixelFormat)
        return false;
    if (!DescribePixelFormat(_hdc, pixelFormat, sizeof(PIXELFORMATDESCRIPTOR), &pfd))
        return false;
    if (!SetPixelFormat(_hdc, pixelFormat, &pfd))
        return false;

    HGLRC dummyContext = wglCreateContext(_hdc);
    if (!dummyContext)
        return false;

    if (!wglMakeCurrent(_hdc, dummyContext))
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

        _context = wglCreateContextAttribs(_hdc, nullptr, attribs);
    }
    else
    {
        _context = wglCreateContext(_hdc);
    }

    wglMakeCurrent(NULL, NULL);
    wglDeleteContext(dummyContext);
    dummyContext = nullptr;

    if (!_context || !wglMakeCurrent(_hdc, _context)) {
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
    EL_ASSERT(initGL());
    return true;
}

void el::PlatformDriverWGL::destroy()
{
    // NOTE: 
    // if you make two consecutive wlgMakeCurrent(NULL, NULL) calls,
    // An invalid handle error may occur.
    // But it seems to work well.
    wglMakeCurrent(NULL, NULL);
    if (_context) {
        wglDeleteContext(_context);
        _context = NULL;
    }
    if (_hwnd && _hdc) {
        ReleaseDC(_hwnd, _hdc);
        _hdc = NULL;
    }
    _hwnd = NULL;
}

void el::PlatformDriverWGL::swapBuffer()
{
    SwapBuffers(_hdc);
}

bool el::PlatformDriverWGL::makeCurrent()
{
    if (wglMakeCurrent(_hdc, _context))
        return true;
    reportLastWindowsError();
    return false;
}
