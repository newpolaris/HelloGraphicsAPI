#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <el/debug.h>
#include <el/graphics_device.h>
#include <el/graphics_program.h>
#include <el/graphics_shader.h>

#pragma comment(lib, "Opengl32.lib")

#define GLFW_EXPOSE_NATIVE_WIN32
// #define GLFW_EXPOSE_NATIVE_COCOA

#include <GLFW/glfw3native.h>

using namespace el;

#define ERROR_INVALID_VERSION_ARB 0x2095
#define ERROR_INVALID_PROFILE_ARB 0x2096
#define ERROR_INCOMPATIBLE_DEVICE_CONTEXTS_ARB 0x2054

#define WGL_CONTEXT_MAJOR_VERSION_ARB 0x2091
#define WGL_CONTEXT_MINOR_VERSION_ARB 0x2092
#define WGL_CONTEXT_FLAGS_ARB 0x2094
#define WGL_CONTEXT_PROFILE_MASK_ARB 0x9126
#define WGL_CONTEXT_CORE_PROFILE_BIT_ARB 0x00000001
#define WGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB 0x00000002
#define WGL_CONTEXT_DEBUG_BIT_ARB 0x00000001
#define WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB 0x00000002

namespace el
{
    typedef BOOL (WINAPI * PFNWGLSWAPINTERVALEXTPROC)(int);
    typedef BOOL (WINAPI * PFNWGLGETPIXELFORMATATTRIBIVARBPROC)(HDC,int,int,UINT,const int*,int*);
    typedef const char* (WINAPI * PFNWGLGETEXTENSIONSSTRINGEXTPROC)(void);
    typedef const char* (WINAPI * PFNWGLGETEXTENSIONSSTRINGARBPROC)(HDC);
    typedef HGLRC (WINAPI * PFNWGLCREATECONTEXTATTRIBSARBPROC)(HDC,HGLRC,const int*);
}

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

    typedef std::shared_ptr<class PlatformSwapchain> PlatformSwapchainPtr;

    class PlatformSwapchain
    {
    public:

        PlatformSwapchain();
        virtual ~PlatformSwapchain();

        bool setup(void* nativeWindow, void* context, const PIXELFORMATDESCRIPTOR& pfd);
        void close();

        bool activate();
        void swapbuffer();

    private:

        HDC _hdc;
        HWND _hwnd;
        HGLRC _context;
    };
    
    class PlatformWGL final
    {
    public:

        PlatformWGL();
        ~PlatformWGL();

        void destroy();

        GraphicsDevicePtr createDevice(GraphicsDeviceType deviceType);
        PlatformSwapchainPtr createSwapchain(void* nativeWindow);

    private:

        HWND _hwnd;
        HDC _hdc;
        HGLRC _context;
        PIXELFORMATDESCRIPTOR _pfd;
        std::vector<GraphicsDevicePtr> _devices;
    };

}

PlatformSwapchain::PlatformSwapchain():
    _hdc(NULL),
    _hwnd(NULL),
    _context(NULL)
{
}

PlatformSwapchain::~PlatformSwapchain()
{
    close();
}

bool PlatformSwapchain::setup(void* nativeWindow, void* context, const PIXELFORMATDESCRIPTOR& pfd)
{
    _hwnd = reinterpret_cast<HWND>(nativeWindow);
    _context = reinterpret_cast<HGLRC>(context);
    _hdc = ::GetDC(_hwnd);
    if (!_hdc)
        return false;

    int pixelFormat = ChoosePixelFormat(_hdc, &pfd);
    if (!pixelFormat)
        return false;
    if (!SetPixelFormat(_hdc, pixelFormat, &pfd))
        return false;

    return true;
}

void PlatformSwapchain::close()
{
    wglMakeCurrent(NULL, NULL);
    _context = NULL;
    if (_hdc) {
        ReleaseDC(_hwnd, _hdc);
        _hdc = NULL;
    }
    _hwnd = NULL;
}

bool PlatformSwapchain::activate()
{
    if (!wglMakeCurrent(_hdc, _context)) {
        reportLastWindowsError();
        return false;
    }
    return true;
}

void PlatformSwapchain::swapbuffer()
{
    SwapBuffers(_hdc);
}

PlatformWGL::PlatformWGL():
    _hwnd(NULL),
    _hdc(NULL),
    _context(NULL)
{
}

PlatformWGL::~PlatformWGL()
{
    destroy();
}

GraphicsDevicePtr PlatformWGL::createDevice(GraphicsDeviceType deviceType)
{
    PIXELFORMATDESCRIPTOR pfd = {
        sizeof(PIXELFORMATDESCRIPTOR),
        1,
        PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,    // Flags
        PFD_TYPE_RGBA,        // The kind of framebuffer. RGBA or palette.
        32,                   // Colordepth of the framebuffer.
        0, 0, 0, 0, 0, 0,
        8,
        24,
        0,
        0, 0, 0, 0,
        24,                   // Number of bits for the depthbuffer
        0,                    // Number of bits for the stencilbuffer
        0,                    // Number of Aux buffers in the framebuffer.
        PFD_MAIN_PLANE,
        0,
        0, 0, 0
    };

    _hwnd = CreateWindowA("STATIC", "dummy", 0, 0, 0, 1, 1, NULL, NULL, NULL, NULL);
    _hdc = ::GetDC(_hwnd);

    int pixelFormat = ChoosePixelFormat(_hdc, &pfd);
    if (!pixelFormat)
        return nullptr;
    if (!DescribePixelFormat(_hdc, pixelFormat, sizeof(PIXELFORMATDESCRIPTOR), &_pfd))
        return nullptr;
    if (!SetPixelFormat(_hdc, pixelFormat, &_pfd))
        return nullptr;

    HGLRC dummyContext = wglCreateContext(_hdc);
    if (!dummyContext)
        return nullptr;

    if (!wglMakeCurrent(_hdc, dummyContext))
    {
        wglDeleteContext(dummyContext);
        destroy();
        return nullptr;
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
        return nullptr;
    }

    GraphicsDeviceDesc desc;
    desc.setType(deviceType);
    auto device = el::createDevice(desc);
    _devices.push_back(device);

    if (!wglMakeCurrent(_hdc, _context))
        return nullptr;

    gladLoadGL();

    return device;
}

PlatformSwapchainPtr PlatformWGL::createSwapchain(void* nativeWindow)
{
    auto swapchain = std::make_shared<PlatformSwapchain>();
    if (!swapchain->setup(nativeWindow, _context, _pfd))
        return nullptr;
    return swapchain;
}

void PlatformWGL::destroy()
{
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


int main()
{
    if (!glfwInit())
        return -1;

    PlatformWGL platform;
    auto device = platform.createDevice(GraphicsDeviceTypeOpenGL);
    EL_ASSERT(device);

    glfwWindowHint(GLFW_VISIBLE, GLFW_TRUE);
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    GLFWwindow* window = glfwCreateWindow(1280, 1024, "Window", nullptr, nullptr);
    if (!window)
        return -1;

    HWND nativeWindow = glfwGetWin32Window(window);
    auto swapchain = platform.createSwapchain(nativeWindow);
    EL_ASSERT(swapchain);

    DWORD error = GetLastError();

    while (!glfwWindowShouldClose(window))
    {
        EL_ASSERT(swapchain->activate());

        glClearColor(0.0f, 0.0f, 0.5f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT);

        swapchain->swapbuffer();

        glfwPollEvents();
    }

    return 0;
}
