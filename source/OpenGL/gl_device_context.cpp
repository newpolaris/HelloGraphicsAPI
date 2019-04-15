#include "gl_device_context.h"

#if EL_BUILD_OPENGL

#include "gl.h"
#include "gl_program.h"

using namespace el;

GLDeviceContext::GLDeviceContext()
{
}

GLDeviceContext::~GLDeviceContext()
{
}

bool GLDeviceContext::create()
{
    // TODO: Better
#if 0
    if (GLEW_ARB_vertex_array_object)
    {
        glGenVertexArrays(1, &_globalVao);
        glBindVertexArray(_globalVao);
    }
    else if (GLEW_APPLE_vertex_array_object)
    {
        glGenVertexArraysAPPLE(1, &_globalVao);
        glBindVertexArrayAPPLE(_globalVao);
    }
#endif
    return true;
}

void GLDeviceContext::destory()
{
}

void GLDeviceContext::setViewport(int32_t x, int32_t y, uint32_t width, uint32_t height)
{
    GL_CHECK(glViewport(x, y, width, height));
}

void GLDeviceContext::setProgram(const GraphicsProgramPtr& ptr)
{
    GLProgramPtr program = std::static_pointer_cast<GLProgram>(ptr);
    if (_program != program)
    {
        _program = program;
        _program->apply();
    }
}

void GLDeviceContext::setTexture(const std::string& name, const GraphicsTexturePtr& texture)
{
    EL_ASSERT(_program);
    _program->setUniform(name, texture);
}

#endif // #if EL_BUILD_OPENGL
