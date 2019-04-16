#include "gl_context.h"

#if EL_BUILD_OPENGL

#include "gl.h"
#include "gl_program.h"

using namespace el;

GLContext::GLContext()
{
}

GLContext::~GLContext()
{
}

bool GLContext::create()
{
    // TODO: Better?
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

void GLContext::destory()
{
}

void GLContext::setViewport(int32_t x, int32_t y, uint32_t width, uint32_t height)
{
    // TODO: viewport comparing;

    GL_CHECK(glViewport(x, y, width, height));
}

void GLContext::setProgram(const GraphicsProgramPtr& ptr)
{
    GLProgramPtr program = std::static_pointer_cast<GLProgram>(ptr);
    if (_program != program)
    {
        _program = program;
        _program->apply();
    }
}

void GLContext::setTexture(const std::string& name, const GraphicsTexturePtr& texture)
{
    EL_ASSERT(_program);
    _program->setUniform(name, texture);
}

void GLContext::setVertexBuffer(const std::string& name, const GraphicsBufferPtr& vertex_buffer, uint32_t stride, uint32_t offset)
{
    EL_ASSERT(_program);
    _program->setVertexBuffer(name, vertex_buffer, stride, offset);
}

void GLContext::setUniform(const std::string& name, const vec3& v0)
{
    EL_ASSERT(_program);
    _program->setUniform(name, v0);
}

void GLContext::setUniform(const std::string& name, const mat4x4& m0)
{
    EL_ASSERT(_program);
    _program->setUniform(name, m0);
}

#endif // #if EL_BUILD_OPENGL
