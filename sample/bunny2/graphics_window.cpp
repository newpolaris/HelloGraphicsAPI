#include "graphics_window.h"

#include <el/debug.h>
 
using namespace el;

GraphicsWindowPtr el::createWindow(const GraphicsWindowDesc& desc)
{
    GraphicsWindowType windowType = desc.getWindowType();
    if (windowType == GraphicsWindowTypeGLFW)
    {
        auto window = std::make_shared<GraphicsWindowGLFW>();
        if (window->setup(desc))
            return window;
        return nullptr;
    }
    return nullptr;
}

void GraphicsWindowDesc::setDeviceType(GraphicsDeviceType type)
{
    _deviceType = type;
}

GraphicsDeviceType GraphicsWindowDesc::getDeviceType() const
{
    return _deviceType;
}

void GraphicsWindowDesc::setWindowType(GraphicsWindowType type)
{
    _windowType = type;
}

GraphicsWindowType GraphicsWindowDesc::getWindowType() const
{
    return _windowType;
}

void GraphicsWindowDesc::setWindowTitle(const std::string& title)
{
    _windowTitle = title;
}

const std::string& GraphicsWindowDesc::getWindowTitle() const
{
    return _windowTitle;
}

void GraphicsWindowDesc::setWidth(uint32_t width)
{
    _width = width;
}

uint32_t GraphicsWindowDesc::getWidth() const
{
    return _width;
}

void GraphicsWindowDesc::setHeight(uint32_t height)
{
    _height = height;
}

uint32_t GraphicsWindowDesc::getHeight() const
{
    return _height;
}

GraphicsWindow::GraphicsWindow()
{
}

GraphicsWindow::~GraphicsWindow()
{
}

GraphicsWindowGLFW::GraphicsWindowGLFW()
{
}

GraphicsWindowGLFW::~GraphicsWindowGLFW()
{
}

bool GraphicsWindowGLFW::setup(const GraphicsWindowDesc& desc)
{
    _windowDesc = desc;

    if (!setupGLFW())
    {
        EL_TRACE("GLFW initialize fail");
        return false;
    }

    if (!setupWindow())
    {
        EL_TRACE("create window fail");
        return false;
    }
    return true;
}

void GraphicsWindowGLFW::close()
{
    glfwSetWindowUserPointer(_window, nullptr);
    glfwDestroyWindow(_window);
    _window = nullptr;
}

void GraphicsWindowGLFW::setWidth(uint32_t width)
{
    _width = width;
}

uint32_t GraphicsWindowGLFW::getWidth() const
{
    return _width;
}

void GraphicsWindowGLFW::setHeight(uint32_t height)
{
    _height = height;
}

uint32_t GraphicsWindowGLFW::getHeight() const
{
    return _height;
}

void GraphicsWindowGLFW::makeContextCurrent()
{
    glfwMakeContextCurrent(_window);
}

bool GraphicsWindowGLFW::setupGLFW()
{
    glfwSetErrorCallback(errorCallback);
    if (!glfwInit())
        return false;

    GraphicsDeviceType deviceType = _windowDesc.getDeviceType();
    if (deviceType == GraphicsDeviceTypeOpenGL)
    {
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
        glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
    }
    else if (deviceType == GraphicsDeviceTypeOpenGLCore)
    {
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
        glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
    #if EL_PLAT_APPLE
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    #else
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);
    #endif
    }
#ifdef GLFW_INCLUDE_ES3
    else if (deviceType == GraphicsDeviceTypeOpenGLES3)
    {
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
        glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
    }
#endif
    else if (deviceType == GraphicsDeviceTypeMetal ||
             deviceType == GraphicsDeviceTypeVulkan)
    {
    }
    return true;
}

bool GraphicsWindowGLFW::setupWindow()
{
    int width = (int)_windowDesc.getWidth();
    int height = (int)_windowDesc.getHeight();
    const char* title = _windowDesc.getWindowTitle().c_str();

    GLFWwindow* sharedWindowContext = nullptr;
    _window = glfwCreateWindow(width, height, title, NULL, sharedWindowContext);
    if (!_window)
        return false;

    glfwSetWindowUserPointer(_window, this);
    glfwSetKeyCallback(_window, keyCallback);
    glfwSetWindowSizeCallback(_window, sizeCallback);

    glfwGetWindowSize(_window, &width, &height);
    _width = uint32_t(width);
    _height = uint32_t(height);

    return true;
}

void GraphicsWindowGLFW::errorCallback(int error, const char* description)
{
    fprintf(stderr, "Error: %s\n", description);
}

void GraphicsWindowGLFW::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (action == GLFW_PRESS && key == GLFW_KEY_ESCAPE)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
}

void GraphicsWindowGLFW::sizeCallback(GLFWwindow* window, int width, int height)
{
    auto glfw = static_cast<GraphicsWindowGLFW*>(glfwGetWindowUserPointer(window));
    EL_ASSERT(glfw);
    glfw->setWidth(uint32_t(width));
    glfw->setHeight(uint32_t(width));
}
