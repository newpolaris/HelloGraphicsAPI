/*
 * Context
 *
 * lunches two window runs on separate thread and wgl-context
 */
#include <windows.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <sstream>

#include <atomic>
#include <mutex>
#include <thread>

#include <condition_variable>
#include <mutex>

#include <el/debug.h>
#include <el/platform.h>
#include "platform_driver_wgl.h"

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

bool TestWindow(el::Event* ev)
{
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    GLFWwindow* window = glfwCreateWindow(1024, 768, "Window Sample", nullptr, nullptr);

    void* windowHandle = nullptr;

#if defined(GLFW_EXPOSE_NATIVE_WIN32)
    windowHandle = glfwGetWin32Window(window);
#endif

    el::PlatformDriverWGL driver;
    EL_ASSERT(driver.create(windowHandle));
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
        std::stringstream stream;
        stream << "PID : " << std::this_thread::get_id();
        glfwSetWindowTitle(window, stream.str().c_str());
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
