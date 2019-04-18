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
    _program.reset();
}

void GLContext::beginRendering()
{
}

void GLContext::endRendering()
{
}

void GLContext::setViewport(const Viewport& viewport)
{
    if (_viewport != viewport)
    {
        _viewport = viewport;
        GL_CHECK(glViewport(_viewport.x, _viewport.y, _viewport.width, _viewport.height));
    }
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

void GLContext::setIndexBuffer(const GraphicsBufferPtr& index_buffer)
{
    EL_ASSERT(_program);
    _program->setIndexBuffer(index_buffer);
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

void GLContext::draw(GraphicsPrimitiveType primitive, uint32_t vertexCount, int32_t vertexStartOffset)
{
    EL_ASSERT(_program);
    GLenum mode = asPrimitiveType(primitive);
    GL_CHECK(glDrawArrays(mode, vertexStartOffset, vertexCount));
}

void GLContext::drawIndexed(GraphicsPrimitiveType primitive, uint32_t indexCount, uint32_t startIndexLocation)
{
    EL_ASSERT(_program);
    GLenum mode = asPrimitiveType(primitive);
    const GLvoid* indices = reinterpret_cast<GLvoid*>(startIndexLocation);
    GL_CHECK(glDrawElements(mode, indexCount, GL_UNSIGNED_INT, indices));
}

void GLContext::drawIndexed(GraphicsPrimitiveType primitive, uint32_t indexCount, uint32_t startIndexLocation, int32_t baseVertexLocation)
{
    EL_ASSERT(_program);
    GLenum mode = asPrimitiveType(primitive);
    const GLvoid* indices = reinterpret_cast<GLvoid*>(startIndexLocation);

    // since 3.2 - ARB_draw_elements_base_vertex
    GL_CHECK(glDrawElementsBaseVertex(mode, indexCount, GL_UNSIGNED_INT, indices, baseVertexLocation));
}

void GLContext::drawInstanced(GraphicsPrimitiveType primitive, uint32_t vertexCountPerInstance, uint32_t instanceCount,
                              uint32_t startVertexLocation, uint32_t startInstanceLocation)
{
}

void GLContext::drawIndexedInstanced(GraphicsPrimitiveType primitive, uint32_t indexCountPerInstance, uint32_t instanceCount,
                                     uint32_t startIndexLocation, int32_t vaseVertexLocation, uint32_t startInstanceLocation)
{
}

#endif // #if EL_BUILD_OPENGL
