#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <debug.h>
#include <graphics_device.h>

#define GLFW_EXPOSE_NATIVE_WIN32
// #define GLFW_EXPOSE_NATIVE_COCOA

#define GLFW_EXPOSE_NATIVE_WGL
// #define GLFW_EXPOSE_NATIVE_NSGL

#include <GLFW/glfw3native.h>

using namespace el;

namespace el
{
    class PlatformSwapchain
    {
    public:

        PlatformSwapchain();
        virtual ~PlatformSwapchain();

        bool setup(void* context, void* nativeWindow);
        void close();

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

    bool PlatformSwapchain::setup(void* context, void* nativeWindow)
    {
        _hwnd = reinterpret_cast<HWND>(nativeWindow);
        _context = reinterpret_cast<HGLRC>(context);

        _hdc = ::GetDC(_hwnd);
        if (!_hdc)
            return false;

        if (!wglMakeCurrent(_hdc, _context))
            return false;
        return true;
    }

    void PlatformSwapchain::close()
    {
        if (_hdc) {
            ReleaseDC(_hwnd, _hdc);
            _hdc = NULL;
        }
        _hwnd = NULL;
        _context = NULL;
    }

    class PlatformWGL final
    {
    public:

        PlatformWGL();
        ~PlatformWGL();

        GraphicsDevicePtr createDevice(GraphicsDeviceType deviceType);
        PlatformSwapchain* createSwapchain(void* nativeWindow);

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
        glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, 1);
        glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);

        _window = glfwCreateWindow(128, 128, "Hiddden", nullptr, nullptr);
        _hwnd = glfwGetWin32Window(_window);
        _hdc = ::GetDC(_hwnd);
        _context = glfwGetWGLContext(_window);

        GraphicsDeviceDesc desc;
        desc.setType(deviceType);
        auto device = el::createDevice(desc);
        _devices.push_back(device);

        if (!wglMakeCurrent(_hdc, _context))
            return nullptr;
        return device;
    }

    PlatformSwapchain* PlatformWGL::createSwapchain(void* nativeWindow)
    {
        auto swapchain = std::unique_ptr<PlatformSwapchain>(new PlatformSwapchain());
        if (!swapchain->setup(_context, nativeWindow))
            return nullptr;
        return swapchain.release();
    }

    GraphicsSwapchainPtr PlatformWGL::destroySwapchain(* swapChain)
    {
        glMakeCurrent(_hdc, _context);
    }
}

int main()
{
    if (!glfwInit())
        return -1;

    PlatformWGL platform;
    auto device = platform.createDevice(GraphicsDeviceTypeOpenGL);

    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);
    GLFWwindow* window = glfwCreateWindow(1280, 1024, "Window", nullptr, nullptr);
    if (!window)
        return -1;

    HWND nativeWindow = glfwGetWin32Window(window);

    auto swapchain = platform.createSwapchain(nativeWindow);

    return 0;
}
