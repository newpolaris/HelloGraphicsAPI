#include "platform_nsgl.h"
#include <mutex>
#include <Cocoa/Cocoa.h>
#include <glad/glad.h>
#include <el/debug.h>

namespace el {

CFBundleRef framework;

bool initGLExtention()
{
    static std::mutex g_library_mutex;
    static bool isInitGLExtention = false;

    std::lock_guard<std::mutex> lock(g_library_mutex);
    if (isInitGLExtention)
        return true;

    EL_ASSERT(framework == NULL);
    
    framework = CFBundleGetBundleWithIdentifier(CFSTR("com.apple.opengl"));
    if (framework == NULL)
        return false;
    return true;
}

struct PlatformNSGLImpl
{
    NSOpenGLContext* context;
};
    
PlatformNSGL::PlatformNSGL() : 
    _impl(new PlatformNSGLImpl)
{
    _impl->context = nullptr;
}

bool PlatformNSGL::create(void* window)
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

    NSOpenGLContext* shareContext = nullptr;
    NSOpenGLPixelFormat* pixelFormat = [[NSOpenGLPixelFormat alloc] initWithAttributes:pixelFormatAttribues];
    NSOpenGLContext* context = _impl->context = [[NSOpenGLContext alloc] initWithFormat:pixelFormat 
                                                                           shareContext:shareContext];
    [pixelFormat release];
    if (context == nil)
        return false;
    
    GLint interval = 0;
    [context makeCurrentContext];
    [context setValues:&interval forParameter:NSOpenGLCPSwapInterval];

    NSWindow* nsWindow = (NSWindow*)window;
    NSView* view = [nsWindow contentView];

    [context setView:view];
    [context makeCurrentContext];

    gladLoadGL();
    return true;
}

void PlatformNSGL::destroy()
{
    [NSOpenGLContext clearCurrentContext];
    [_impl->context release];
    _impl->context = nullptr;
    delete _impl;
    _impl = nullptr;
}

void PlatformNSGL::swapBuffer()
{
    [_impl->context flushBuffer];
}

void PlatformNSGL::makeCurrent()
{
    [NSOpenGLContext makeCurrentContext];
}

} // namespace el
