#include <windows.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include <atomic>
#include <mutex>
#include <thread>

#include <condition_variable>
#include <mutex>

#include <el/debug.h>
#include <el/platform.h>
#include "platform_driver_wgl.h"

#pragma comment(lib, "opengl32.lib")

#if EL_PLAT_WINDOWS
#define GLFW_EXPOSE_NATIVE_WIN32
#elif EL_PLAT_OSX
#define GLFW_EXPOSE_NATIVE_COCOA
#endif

#include <GLFW/glfw3native.h>

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

bool TestWindow()
{
    return true;
}

void waits(GLFWwindow* window, el::PlatformDriverWGL* driver, el::Event* ev)
{
    ev->wait();
    
    {
        (driver->makeCurrent());
        glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        driver->swapBuffer();
        wglMakeCurrent(0, nullptr);
    }
}

bool TestApp()
{
    if (!glfwInit())
        return false;

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    GLFWwindow* window[2];
    window[0] = glfwCreateWindow(1024, 768, "Window Sample0", nullptr, nullptr);
    window[1] = glfwCreateWindow(1024, 768, "Window Sample1", nullptr, nullptr);

    void* windowHandle[2];

#if defined(GLFW_EXPOSE_NATIVE_WIN32)
    windowHandle[0] = glfwGetWin32Window(window[0]);
    windowHandle[1] = glfwGetWin32Window(window[1]);
#endif

    el::PlatformDriverWGL driver[2];
    EL_ASSERT(driver[0].create(windowHandle[0]));
    EL_ASSERT(driver[1].create(windowHandle[1]));
    EL_TRACE("%s\n%s\n%s\n%s\n",
        glGetString(GL_RENDERER),  // e.g. Intel HD Graphics 3000 OpenGL Engine
        glGetString(GL_VERSION),   // e.g. 3.2 INTEL-8.0.61
        glGetString(GL_VENDOR),    // e.g. NVIDIA Corporation
        glGetString(GL_SHADING_LANGUAGE_VERSION)  // e.g. 4.60 NVIDIA or 1.50 NVIDIA via Cg compiler
    );

    driver[0].makeCurrent();

    el::Event ev;

    while (!glfwWindowShouldClose(window[0]))
    {
        std::thread t(waits, window[1], &(driver[1]), &ev);
        ev.set();

        EL_ASSERT(driver[0].makeCurrent());
        glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        driver[0].swapBuffer();
        wglMakeCurrent(0, nullptr);
        t.join();
        glfwPollEvents();
    }
    glfwDestroyWindow(window[0]);

    return true;
}


int main()
{
    EL_ASSERT(TestApp());
    return 0;
}
