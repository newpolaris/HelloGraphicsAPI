#include "gl_device_context.h"

#if EL_BUILD_OPENGL

#include "gl.h"

using namespace el;

GLDeviceContext::GLDeviceContext()
{
}

GLDeviceContext::~GLDeviceContext()
{
}

bool GLDeviceContext::create()
{
    return true;
}

void GLDeviceContext::destory()
{
}

#endif // #if EL_BUILD_OPENGL
