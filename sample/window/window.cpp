#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cstdio>
#include <iostream>
#include <debug.h>
#include <platform.h>

#if EL_PLAT_WINDOWS
#define GLFW_EXPOSE_NATIVE_WIN32
#elif EL_PLAT_OSX
#define GLFW_EXPOSE_NATIVE_COCOA
#endif

#include <GLFW/glfw3native.h>

#if EL_PLAT_WINDOWS

#define WGL_CONTEXT_MAJOR_VERSION_ARB 0x2091
#define WGL_CONTEXT_MINOR_VERSION_ARB 0x2092
#define WGL_CONTEXT_FLAGS_ARB 0x2094
#define WGL_CONTEXT_PROFILE_MASK_ARB 0x9126

typedef HGLRC (WINAPI * WGLCREATECONTEXT_T)(HDC);
typedef BOOL (WINAPI * WGLDELETECONTEXT_T)(HGLRC);
typedef PROC (WINAPI * WGLGETPROCADDRESS_T)(LPCSTR);
typedef HDC (WINAPI * WGLGETCURRENTDC_T)(void);
typedef BOOL (WINAPI * WGLMAKECURRENT_T)(HDC, HGLRC);
typedef BOOL (WINAPI * WGLSHARELISTS_T)(HGLRC, HGLRC);
typedef HGLRC (WINAPI * PFNWGLCREATECONTEXTATTRIBSARBPROC)(HDC, HGLRC, const int*);

struct WglLibrary
{
    bool Load();

    HINSTANCE _instance;
    WGLCREATECONTEXT_T CreateContext;
    WGLDELETECONTEXT_T DeleteContext;
    WGLGETPROCADDRESS_T GetProcAddress;
    WGLGETCURRENTDC_T GetCurrentDC;
    WGLMAKECURRENT_T MakeCurrent;
    WGLSHARELISTS_T ShareLists;
};

bool WglLibrary::Load()
{
    _instance = LoadLibraryA("opengl32.dll");
    if (!_instance)
    {
        EL_TRACE("WGL: Failed to load opengl32.dll");
        return false;
    }

    CreateContext = (WGLCREATECONTEXT_T)
        ::GetProcAddress(_instance, "wglCreateContext");
    DeleteContext = (WGLDELETECONTEXT_T)
        ::GetProcAddress(_instance, "wglDeleteContext");
    GetProcAddress = (WGLGETPROCADDRESS_T)
        ::GetProcAddress(_instance, "wglGetProcAddress");
    GetCurrentDC = (WGLGETCURRENTDC_T)
        ::GetProcAddress(_instance, "wglGetCurrentDC");
    MakeCurrent = (WGLMAKECURRENT_T)
        ::GetProcAddress(_instance, "wglMakeCurrent");
    ShareLists = (WGLSHARELISTS_T)
        ::GetProcAddress(_instance, "wglShareLists");

    return true;
}

WglLibrary instance; 

#define wglCreateContext    instance.CreateContext
#define wglMakeCurrent      instance.MakeCurrent 
#define wglDeleteContext    instance.DeleteContext
#define wglGetProcAddress   instance.GetProcAddress

struct PlatformDriver
{
    HDC _hdc = 0;
    HGLRC _context = 0;

    bool createDriver(void* window)
    {
        EL_ASSERT(instance.Load());

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

        HWND hwnd = reinterpret_cast<HWND>(window);
        _hdc = ::GetDC(hwnd);

        int pixelFormat = ChoosePixelFormat(_hdc, &pfd);
        SetPixelFormat(_hdc, pixelFormat, &pfd);

        HGLRC dummyContext = wglCreateContext(_hdc);
        if (!wglMakeCurrent(_hdc, dummyContext))
            return false;

        PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribs =
                (PFNWGLCREATECONTEXTATTRIBSARBPROC)wglGetProcAddress("wglCreateContextAttribsARB");

        if (wglCreateContextAttribs)
        {
            int attribs[] = {
                WGL_CONTEXT_MAJOR_VERSION_ARB, 4,
                WGL_CONTEXT_MINOR_VERSION_ARB, 1,
                WGL_CONTEXT_FLAGS_ARB, 
                WGL_CONTEXT_PROFILE_MASK_ARB,
                0
            };

            _context = wglCreateContextAttribs(_hdc, nullptr, attribs);
            if (!_context)
                return false;
            wglMakeCurrent(0, 0);
            wglDeleteContext(dummyContext);
            dummyContext = nullptr;
            if (!wglMakeCurrent(_hdc, _context))
                return false;
        }
        gladLoadGL();

        return true;
    }

    void swapBuffer()
    {
        SwapBuffers(_hdc);
    }
};

#endif // EL_PLAT_WINDOWS

bool TestApp()
{
    if (!glfwInit())
        return false;

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

    GLFWwindow* window = glfwCreateWindow(1024, 768, "Window Sample", nullptr, nullptr);

    void* windowHandle = nullptr;

#if defined(GLFW_EXPOSE_NATIVE_WIN32)
    windowHandle = glfwGetWin32Window(window);
#endif

    PlatformDriver driver;
    EL_ASSERT(driver.createDriver(windowHandle));

    std::printf("%s\n%s\n%s\n%s\n",
        glGetString(GL_RENDERER),  // e.g. Intel HD Graphics 3000 OpenGL Engine
        glGetString(GL_VERSION),   // e.g. 3.2 INTEL-8.0.61
        glGetString(GL_VENDOR),    // e.g. NVIDIA Corporation
        glGetString(GL_SHADING_LANGUAGE_VERSION)  // e.g. 4.60 NVIDIA or 1.50 NVIDIA via Cg compiler
    );

    while (!glfwWindowShouldClose(window))
    {
        glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        driver.swapBuffer();
        glfwPollEvents();
    }
    glfwDestroyWindow(window);
    return true;
}

int main()
{
    EL_ASSERT(TestApp());

    return 0;
}