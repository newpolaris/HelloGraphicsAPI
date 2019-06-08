#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <el/debug.h>

#if EL_PLAT_WINDOWS
#define GLFW_EXPOSE_NATIVE_WIN32
#elif EL_PLAT_OSX
#define GLFW_EXPOSE_NATIVE_COCOA
#endif

#define GLFW_EXPOSE_NATIVE_COCOA
#include <GLFW/glfw3native.h>

#import <Cocoa/Cocoa.h>

struct PlatformCocoaGLImpl {
    NSOpenGLContext* mGLContext = nullptr;
    NSView* mCurrentView = nullptr;
};

bool executeSample()
{
    NSOpenGLPixelFormatAttribute pixelFormatAttribues[] = {
        NSOpenGLPFAColorSize,     24,
        NSOpenGLPFAAlphaSize,     8,
        NSOpenGLPFADepthSize,     24,
        NSOpenGLPFAStencilSize,   8,
        NSOpenGLPFADoubleBuffer,  true,
        NSOpenGLPFAAccelerated,   true,
        NSOpenGLPFANoRecovery,    true,
        NSOpenGLPFAOpenGLProfile, NSOpenGLProfileVersion3_2Core,
        0, 0
    };

    NSOpenGLContext* sharedContext = nullptr;
    NSOpenGLPixelFormat* pixelFormat = [[NSOpenGLPixelFormat alloc] initWithAttributes:pixelFormatAttribues];
    NSOpenGLContext* nsOpenGLContext = [[NSOpenGLContext alloc] initWithFormat:pixelFormat shareContext:sharedContext];
    [pixelFormat release];
    pixelFormat = nullptr;

    GLint interval = 0;
    [nsOpenGLContext makeCurrentContext];
    [nsOpenGLContext setValues:&interval forParameter:NSOpenGLCPSwapInterval];

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    GLFWwindow* window = glfwCreateWindow(1280, 1024, "window test", nullptr, nullptr); 
    if (!window)
        return false;
    
    NSWindow* nsWindow = glfwGetCocoaWindow(window);
    NSView* view = [nsWindow contentView];
    
    [nsOpenGLContext setView:view];
    [nsOpenGLContext makeCurrentContext];
    
    gladLoadGL();
    while (!glfwWindowShouldClose(window))
    {
        glClearColor(0.0f, 0.0f, 0.5f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT);

        [nsOpenGLContext flushBuffer];
        glfwPollEvents();
    }
    
    [NSOpenGLContext clearCurrentContext];
    [nsOpenGLContext release];
    nsOpenGLContext = nullptr;
    
    return true;
}

int main()
{
    if (!glfwInit())
        return -1;
    EL_ASSERT(executeSample());
    return 0;
}
