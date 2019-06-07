
#if EL_PLAT_OSX

#include <Cocoa/Cocoa.h>

namespace el
{
    CFBundleRef framework;
    
    bool initGLExtention();

    struct PlatformDriver
    {
        bool create(void* window);
        void swapBuffer();
    };

} // namespace el

using namespace el;

bool el::initGLExtention()
{
    // std::lock_guard<std::mutex> lock(g_library_mutex);
    static bool isInitGLExtention = false;
    if (isInitGLExtention)
        return true;

    EL_ASSERT(framework == NULL);
    
    framework = CFBundleGetBundleWithIdentifier(CFSTR("com.apple.opengl"));
    if (framework == NULL)
        return false;
    
    NSOpenGLPixelFormatAttribute pixelFormatAttributes[] =
    {
        NSOpenGLPFADoubleBuffer,
        NSOpenGLPFAAccelerated,
        NSOpenGLPFAOpenGLProfile, NSOpenGLProfileVersion3_2Core,
        0, 0,
    };
    NSOpenGLContext* shareContext = (NSOpenGLContext*)nullptr;
    NSOpenGLPixelFormat* pixelFormat = [[NSOpenGLPixelFormat alloc] initWithAttributes:pixelFormatAttributes];
    NSOpenGLContext* nsOpenGLContext = [[NSOpenGLContext alloc] initWithFormat:pixelFormat shareContext:shareContext];
    [pixelFormat release];
    
    GLint interval = 0;
    [nsopenglcontext makeCurrentContext];
    [nsOpenGLContext setValues:&interval forParameter:NSOpenGLCPSwapInterval];

    return true;
}

bool PlatformDriver::create(void* window)
{
    
    // id glfwGetCocoaWindow(GLFWwindow* window);
    initGLExtention();
}

void PlatformDriver::swapBuffer()
{
}

#endif // EL_PLAT_OSX

