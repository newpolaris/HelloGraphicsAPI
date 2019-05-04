#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <debug.h>
#include <graphics_device.h>
#include <graphics_program.h>
#include <graphics_shader.h>

#pragma comment(lib, "Opengl32.lib")

#define GLFW_EXPOSE_NATIVE_WIN32
// #define GLFW_EXPOSE_NATIVE_COCOA

#define GLFW_EXPOSE_NATIVE_WGL
// #define GLFW_EXPOSE_NATIVE_NSGL

#include <GLFW/glfw3native.h>

using namespace el;

namespace el
{
    typedef std::shared_ptr<class PlatformSwapchain> PlatformSwapchainPtr;

    class PlatformSwapchain
    {
    public:

        PlatformSwapchain();
        virtual ~PlatformSwapchain();

        bool setup(void* nativeWindow, void* context);
        void close();

        bool activate();
        void deactivate();

        void swapbuffer();

    private:

        HDC _hdc;
        HWND _hwnd;
        HGLRC _context;
    };

    PlatformSwapchain::PlatformSwapchain() :
        _hdc(NULL),
        _hwnd(NULL),
        _context(NULL)
    {
    }

    PlatformSwapchain::~PlatformSwapchain()
    {
        close();
    }

    bool PlatformSwapchain::setup(void* nativeWindow, void* context)
    {
        _hwnd = reinterpret_cast<HWND>(nativeWindow);
        _context = reinterpret_cast<HGLRC>(context);
        _hdc = ::GetDC(_hwnd);
        if (!_hdc)
            return false;

        PIXELFORMATDESCRIPTOR pfd = {
            sizeof(PIXELFORMATDESCRIPTOR),
            1,
            PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,    // Flags
            PFD_TYPE_RGBA,        // The kind of framebuffer. RGBA or palette.
            32,                   // Colordepth of the framebuffer.
            8, 16, 8, 8, 8, 0,
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

        // GFLW's default format
        PIXELFORMATDESCRIPTOR pfd2; 
        DescribePixelFormat(_hdc, 12, sizeof(PIXELFORMATDESCRIPTOR), &pfd2);

        int pixelFormat = ChoosePixelFormat(_hdc, &pfd2);
        if (!pixelFormat)
            return false;
        if (!DescribePixelFormat(_hdc, pixelFormat, sizeof(PIXELFORMATDESCRIPTOR), &pfd2))
            return false;
        if (!SetPixelFormat(_hdc, pixelFormat, &pfd2))
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
        if (!wglMakeCurrent(_hdc, _context))
            return false;
        return true;
    }

    void PlatformSwapchain::deactivate()
    {
        wglMakeCurrent(NULL, NULL);
    }

    void PlatformSwapchain::swapbuffer()
    {
        SwapBuffers(_hdc);
    }

    class PlatformWGL final
    {
    public:

        PlatformWGL();
        ~PlatformWGL();

        GraphicsDevicePtr createDevice(GraphicsDeviceType deviceType);
        PlatformSwapchainPtr createSwapchain(void* nativeWindow);


    private:

        GLFWwindow* _window;

        HWND _hwnd;
        HDC _hdc;
        HGLRC _context;
        std::vector<GraphicsDevicePtr> _devices;
    };

    PlatformWGL::PlatformWGL() :
        _window(nullptr)
    {
    }

    PlatformWGL::~PlatformWGL()
    {
        glfwDestroyWindow(_window);
        _window = nullptr;
    }

    GraphicsDevicePtr PlatformWGL::createDevice(GraphicsDeviceType deviceType)
    {
        glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
        glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, 1);

        _window = glfwCreateWindow(1, 1, "Hiddden", nullptr, nullptr);
        _hwnd = glfwGetWin32Window(_window);
        _hdc = ::GetDC(_hwnd);
        _context = glfwGetWGLContext(_window);

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
        if (!swapchain->setup(nativeWindow, _context))
            return nullptr;
        return swapchain;
    }
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
        swapchain->deactivate();

        glfwPollEvents();
    }

    return 0;
}
