#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>

void* createOpenGLDriver()
{
}

int createWindow()
{
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    GLFWwindow* window = glfwCreateWindow("window test", 1024, 768); 
    if (!window)
        return false;
    void* nativeWindow = glfwGetWin32Window(window);
    HDC hdc = ::GetDC(nativeWindow);
    ::ReleaseDC(nativeWindow, hdc);
    return true;
}

int main()
{
    return 0;
}
