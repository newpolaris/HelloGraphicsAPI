/*
 * multicontext
 * 
 * one window switches two opengl context
 */
#if defined(_WIN32)
#include <windows.h>
#endif
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

#if EL_PLAT_WINDOWS
#include "platform_wgl.h"
#define GLFW_EXPOSE_NATIVE_WIN32
#elif EL_PLAT_OSX
#include "platform_nsgl.h"
#define GLFW_EXPOSE_NATIVE_COCOA
#endif

#include <GLFW/glfw3native.h>
#include "platform.h"

#include <stdio.h>
#include <memory>

namespace el {

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
#elif defined(GLFW_EXPOSE_NATIVE_COCOA)
    windowHandle = glfwGetCocoaWindow(window); // NSWindow
#endif

    el::PlatformNSGL driver[2];
    EL_ASSERT(driver[0].create(windowHandle));
    EL_TRACE("%s\n%s\n%s\n%s\n",
        glGetString(GL_RENDERER),  // e.g. Intel HD Graphics 3000 OpenGL Engine
        glGetString(GL_VERSION),   // e.g. 3.2 INTEL-8.0.61
        glGetString(GL_VENDOR),    // e.g. NVIDIA Corporation
        glGetString(GL_SHADING_LANGUAGE_VERSION)  // e.g. 4.60 NVIDIA or 1.50 NVIDIA via Cg compiler
    );

    EL_ASSERT(driver[1].create(windowHandle));
    EL_TRACE("%s\n%s\n%s\n%s\n",
        glGetString(GL_RENDERER),  // e.g. Intel HD Graphics 3000 OpenGL Engine
        glGetString(GL_VERSION),   // e.g. 3.2 INTEL-8.0.61
        glGetString(GL_VENDOR),    // e.g. NVIDIA Corporation
        glGetString(GL_SHADING_LANGUAGE_VERSION)  // e.g. 4.60 NVIDIA or 1.50 NVIDIA via Cg compiler
    );

    while (!glfwWindowShouldClose(window))
    {
        driver[0].makeCurrent();
        glClearColor(0.5f, 1.0f, 0.5f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        driver[0].swapBuffer();
        driver[1].makeCurrent();
        glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        driver[1].swapBuffer();
        glfwPollEvents();
    }
    glfwDestroyWindow(window);
    return true;
}

void waits(el::Event* ev)
{
    ev->wait();
    std::chrono::seconds duration(1);
    std::this_thread::sleep_for(duration);
    TestWindow(nullptr);
}

bool TestApp()
{
    if (!glfwInit())
        return false;

    el::Event ev;
    std::thread t(waits, &ev);
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
