#include <graphics_window.h>
#include <debug.h>

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

GraphicsWindowType el::GraphicsWindowDesc::getWindowType() const
{
    return _windowType;
}

void el::GraphicsWindowDesc::setWindowTitle(const std::string& title)
{
    _windowTitle = title;
}

const std::string& el::GraphicsWindowDesc::getWindowTitle() const
{
    return _windowTitle;
}

void el::GraphicsWindowDesc::setWidth(uint32_t width)
{
    _width = width;
}

uint32_t el::GraphicsWindowDesc::getWidth() const
{
    return _width;
}

void el::GraphicsWindowDesc::setHeight(uint32_t height)
{
    _height = height;
}

uint32_t el::GraphicsWindowDesc::getHeight() const
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
    glfwDestroyWindow(_window);
    _window = nullptr;
}

bool GraphicsWindowGLFW::run()
{
    return true;
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
    GLFWwindow* sharedWindowContext = nullptr;
    _window = glfwCreateWindow(1024, 768, "First", NULL, sharedWindowContext);
    if (!_window)
        return false;

    glfwSetKeyCallback(_window, keyCallback);

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
