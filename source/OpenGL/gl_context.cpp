#include "gl_context.h"
#if EL_BUILD_OPENGL

#include "graphics_data.h"
#include "gl.h"
#include "gl_types.h"
#include "gl_buffer.h"
#include "gl_program.h"
#include "gl_input_layout.h"

using namespace el;

GLContext::GLContext() :
    _indexSize(0u),
    _indexCount(0u),
    _vertexSize(0u),
    _vertexCount(0u)
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

    // enough to store maximum vertex attributes
    const uint32_t maxVertexBufferBindings = 16; 
    _vertexBuffers.resize(maxVertexBufferBindings);

    // TODO: GL_MAX_DRAW_BUFFERS - glDrawBuffers

    return true;
}

void GLContext::destory()
{
    // _attributes.clear();
    _vertexBuffers.clear();

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
    // TODO;
    // if (_viewport != viewport)
    {
        _viewport = viewport;
        GL_CHECK(glViewport(_viewport.x, _viewport.y, _viewport.width, _viewport.height));
    }
}

void GLContext::setProgram(const GraphicsProgramPtr& ptr)
{
    GLProgramPtr program = std::static_pointer_cast<GLProgram>(ptr);
    // TODO:
    // if (_program != program)
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

void GLContext::setVertexBuffer(const std::string& name, const GraphicsDataPtr& vertexData, uint32_t stride, uint32_t offset)
{
    EL_ASSERT(_program);
    _vertexSize = vertexData->getDesc().getElementSize();
    _vertexCount = vertexData->getDesc().getElementCount();
    _program->setVertexBuffer(name, vertexData, stride, offset);
}

void GLContext::setVertexBuffer(uint32_t binding, const GraphicsDataPtr& vertexData, uint32_t offset)
{
    EL_ASSERT(vertexData);
    EL_ASSERT(_vertexBuffers.size() > binding);
    _vertexSize = vertexData->getDesc().getElementSize();
    _vertexCount = vertexData->getDesc().getElementCount();
    _vertexBuffers[binding] = GLVertexBuffer(vertexData, offset);
}

void GLContext::setIndexBuffer(const GraphicsDataPtr& indexData)
{
    EL_ASSERT(_program);
    EL_ASSERT(indexData);

    _indexSize = indexData->getDesc().getElementSize();
    _indexCount = indexData->getDesc().getElementCount();
    EL_ASSERT(_indexCount >= 0);
    EL_ASSERT(_indexSize > 0);
    _program->setIndexBuffer(indexData);
}

void GLContext::setUniform(const std::string& name, float v0)
{
    EL_ASSERT(_program);
    _program->setUniform(name, v0);
}

void GLContext::setUniform(const std::string& name, const vec3& v0)
{
    EL_ASSERT(_program);
    _program->setUniform(name, v0);
}

void GLContext::setUniform(const std::string& name, const quat& q0)
{
    EL_ASSERT(_program);
    _program->setUniform(name, q0);
}

void GLContext::setUniform(const std::string& name, const mat4x4& m0)
{
    EL_ASSERT(_program);
    _program->setUniform(name, m0);
}

void GLContext::setInputLayout(const GraphicsInputLayoutPtr& inputLayout)
{
    EL_ASSERT(_program);
    EL_ASSERT(inputLayout);

    _inputLayout = inputLayout;

    // TODO:
    auto& inputDesc = _inputLayout->getDesc();
    auto& activeAttribs = _program->getVertexAttributes();
    for (auto& activeAttrib : activeAttribs)
    {
        auto attrib = std::static_pointer_cast<GLVertexAttribute>(activeAttrib);

        GLuint attribProgramLocation = GL_INVALID_VALUE;
        for (auto& inputAttrib : inputDesc.getAttributes())
        {
            if (activeAttrib->isMatch(inputAttrib))
            {
                const uint32_t attribSize = asVariableComponentSize(attrib->type) * attrib->count;
                const uint32_t vertexAttrbSize = asVertexFormatSize(inputAttrib.getFormat());
                EL_ASSERT(attribSize == vertexAttrbSize);

                attribProgramLocation = attrib->location;
                uint32_t binding = inputAttrib.getBinding();
                break;
            }
        }
        if (attribProgramLocation != GL_INVALID_VALUE)
        {
            attrib->count;
            attrib->location;
            attrib->type;
        }
    }

    for (auto& inputBinding : inputDesc.getBindings())
    {
        inputBinding.getStride();
        inputBinding.getInputRate();
        inputBinding.getBinding();
    }
    // _attributes
}

void GLContext::bindVertexBuffers(const GLVertexBuffers& buffers)
{
}

void GLContext::draw(GraphicsPrimitiveType primitive, uint32_t vertexCount, int32_t vertexStartOffset)
{
    EL_ASSERT(_program);
    bindVertexBuffers(_vertexBuffers);

    GLenum mode = asPrimitiveType(primitive);
    GL_CHECK(glDrawArrays(mode, vertexStartOffset, vertexCount));
}

void GLContext::drawIndexed(GraphicsPrimitiveType primitive, uint32_t indexCount, uint32_t startIndexLocation)
{
    EL_ASSERT(_program);
    bindVertexBuffers(_vertexBuffers);

    const GLenum indexType = getIndexType(_indexSize);
    EL_ASSERT(indexCount + startIndexLocation <= _indexCount); 

    GLenum mode = asPrimitiveType(primitive);
    const GLvoid* offset = reinterpret_cast<GLvoid*>(startIndexLocation);
    GL_CHECK(glDrawElements(mode, indexCount, indexType, offset));
}

void GLContext::drawIndexed(GraphicsPrimitiveType primitive, uint32_t indexCount, uint32_t startIndexLocation, int32_t baseVertexLocation)
{
    EL_ASSERT(_program);
    bindVertexBuffers(_vertexBuffers);

    const GLenum indexType = getIndexType(_indexSize);
    EL_ASSERT(indexCount + startIndexLocation <= _indexCount); 
    EL_ASSERT(baseVertexLocation <= int32_t(_vertexCount));

    GLenum mode = asPrimitiveType(primitive);
    const GLvoid* offset = reinterpret_cast<GLvoid*>(startIndexLocation);

    // since Core 3.2 or requires ARB_draw_elements_base_vertex
    GL_CHECK(glDrawElementsBaseVertex(mode, indexCount, indexType, offset, baseVertexLocation));
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
