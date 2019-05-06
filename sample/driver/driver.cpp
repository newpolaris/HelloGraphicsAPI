#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <el_debug.h>

#if EL_PLAT_WINDOWS
#define GLFW_EXPOSE_NATIVE_WIN32
#elif EL_PLAT_OSX
#define GLFW_EXPOSE_NATIVE_COCOA
#endif

#include <GLFW/glfw3native.h>

bool executeSample()
{
    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);
    GLFWwindow* window = glfwCreateWindow(1280, 1024, "window test", nullptr, nullptr); 
    if (!window)
        return false;
    glfwMakeContextCurrent(window);
    gladLoadGL();
    while (!glfwWindowShouldClose(window))
    {
        glClearColor(0.0f, 0.0f, 0.5f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwMakeContextCurrent(nullptr);
    return true;
}

int main()
{
    if (!glfwInit())
        return -1;
    EL_ASSERT(executeSample());
    return 0;
}
