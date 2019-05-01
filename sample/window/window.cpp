#include <glad/glad.h>
#include <GLFW/glfw3.h>
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

namespace el
{
    typedef BOOL (WINAPI * PFNWGLSWAPINTERVALEXTPROC)(int);
    typedef BOOL (WINAPI * PFNWGLGETPIXELFORMATATTRIBIVARBPROC)(HDC,int,int,UINT,const int*,int*);
    typedef const char* (WINAPI * PFNWGLGETEXTENSIONSSTRINGEXTPROC)(void);
    typedef const char* (WINAPI * PFNWGLGETEXTENSIONSSTRINGARBPROC)(HDC);
    typedef HGLRC (WINAPI * PFNWGLCREATECONTEXTATTRIBSARBPROC)(HDC,HGLRC,const int*);

    typedef HGLRC (WINAPI * PFN_wglCreateContext)(HDC);
    typedef BOOL (WINAPI * PFN_wglDeleteContext)(HGLRC);
    typedef PROC (WINAPI * PFN_wglGetProcAddress)(LPCSTR);
    typedef HDC (WINAPI * PFN_wglGetCurrentDC)(void);
    typedef HGLRC (WINAPI * PFN_wglGetCurrentContext)(void);
    typedef BOOL (WINAPI * PFN_wglMakeCurrent)(HDC,HGLRC);
    typedef BOOL (WINAPI * PFN_wglShareLists)(HGLRC,HGLRC);

    extern PFN_wglGetProcAddress wglGetProcAddress;
	extern PFN_wglMakeCurrent wglMakeCurrent;
	extern PFN_wglCreateContext wglCreateContext;
	extern PFN_wglDeleteContext wglDeleteContext;
    extern PFN_wglGetCurrentDC wglGetCurrentDC;
    extern PFN_wglGetCurrentContext wglGetCurrentContext;
    extern PFN_wglShareLists wglShareLists;
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

    bool initGLExtention();
}

#include <atomic>

bool el::initGLExtention()
{
    static thread_local std::atomic_bool isInitGLExtention(false);
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

    isInitGLExtention = true;
    return true;
}

#include <mutex>

namespace el
{
struct PlatformDriver
{
    HDC _hdc = 0;
    HGLRC _context = 0;

    bool createDriver(void* window)
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
        static std::once_flag flag; 
        std::call_once(flag, gladLoadGL);

        return true;
    }

    void swapBuffer()
    {
        SwapBuffers(_hdc);
    }
};
}

#endif // EL_PLAT_WINDOWS

#include <condition_variable>
#include <mutex>

namespace el
{
    class Event {
    public:

        void set()
        {
            std::lock_guard<std::mutex> guard(_mutex);
            _flag = true;
            _condition.notify_one();
        }

        void wait()
        {
            std::unique_lock<std::mutex> guard(_mutex);
            _condition.wait(guard, [this]{ return _flag;});
            _flag = false;
        }

    private:

        bool _flag = false;
        std::condition_variable _condition;
        std::mutex _mutex;
    };

}

bool TestWindow(el::Event* ev)
{
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    GLFWwindow* window = glfwCreateWindow(1024, 768, "Window Sample", nullptr, nullptr);

    void* windowHandle = nullptr;

#if defined(GLFW_EXPOSE_NATIVE_WIN32)
    windowHandle = glfwGetWin32Window(window);
#endif

    el::PlatformDriver driver;
    EL_ASSERT(driver.createDriver(windowHandle));
    EL_TRACE("%s\n%s\n%s\n%s\n",
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

void waits(el::Event* ev)
{
    ev->wait();
    TestWindow(nullptr);
}

bool TestApp()
{
    if (!glfwInit())
        return false;

    el::Event ev;
    std::thread t(waits, &ev);
    std::chrono::seconds duration(2);
    std::this_thread::sleep_for(duration);
    ev.set();
    TestWindow(nullptr);
    t.join();

    return true;
}


int main()
{
    EL_ASSERT(TestApp());

    return 0;
}