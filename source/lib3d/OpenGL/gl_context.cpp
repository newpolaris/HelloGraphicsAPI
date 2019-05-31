#include "gl_context.h"
#if EL_BUILD_OPENGL

#include <el/graphics_data.h>
#include "gl_headers.h"
#include "gl_types.h"
#include "gl_buffer.h"
#include "gl_program.h"
#include "gl_input_layout.h"

#include <sstream>

using namespace el;

bool isInitGLExtension = false;
bool initGLExtension()
{
	if (isInitGLExtension)
		return true;

    // The contexts are created with the same APIs so the function
    // pointers should be re-usable between them
    gladLoadGL();

    // TODO:
    // how to handle glad_glGenFramebuffersEXT?
    // GL_IMPORT_EXT__(true, PFNGLBINDFRAMEBUFFERPROC, glBindFramebuffer);
    if (glGenFramebuffers == 0)
    {
        EL_TRACE("Require GL_ARB_framebuffer_object");
        return false;
    }

    isInitGLExtension = true;

    return true;
}

GLContext::GLContext() :
    _isDepthTest(true),
    _isCullface(true),
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
    if (!initGLExtension())
        return false;

    // TODO:
    glClearDepth(1.0f);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClearStencil(0);

    glViewport(0, 0, 0, 0);

    glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);

    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);
    glDepthFunc(GL_LEQUAL);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CW);

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

void GLContext::destroy()
{
    // _attributes.clear();
    _vertexBuffers.clear();

    _program.reset();
}

void GLContext::startDebugControl()
{
#if EL_CONFIG_DEBUG
    if (glDebugMessageCallback) {
        GL_CHECK(glEnable(GL_DEBUG_OUTPUT));
        GL_CHECK(glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS));
        GL_CHECK(glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, NULL, GL_TRUE));
        GL_CHECK(glDebugMessageCallback(debugCallback, nullptr));
    }

    std::printf("%s\n%s\n%s\n%s\n",
        glGetString(GL_RENDERER),  // e.g. Intel HD Graphics 3000 OpenGL Engine
        glGetString(GL_VERSION),   // e.g. 3.2 INTEL-8.0.61
        glGetString(GL_VENDOR),    // e.g. NVIDIA Corporation
        glGetString(GL_SHADING_LANGUAGE_VERSION)  // e.g. 4.60 NVIDIA or 1.50 NVIDIA via Cg compiler
    );
#endif
}


void GLContext::setDepthTestEnable(bool enable)
{
    // TODO:
    // if (_isDepthTest != enable)
    {
        _isDepthTest = enable;
        if (_isDepthTest)
            glEnable(GL_DEPTH_TEST);
        else
            glDisable(GL_DEPTH_TEST);
    }
}

void GLContext::setCullFaceEnable(bool enable)
{
    // TODO:
    // if (_isCullface != enable)
    {
        _isCullface = enable;
        if (_isCullface)
            glEnable(GL_CULL_FACE);
        else
            glDisable(GL_CULL_FACE);
    }
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
                // uint32_t binding = inputAttrib.getBinding();
                break;
            }
        }
        if (attribProgramLocation != GL_INVALID_VALUE)
        {
            //attrib->count;
            //attrib->location;
            //attrib->type;
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

    const GLenum indexType = asIndexType(_indexSize);
    EL_ASSERT(indexCount + startIndexLocation <= _indexCount); 

    GLenum mode = asPrimitiveType(primitive);
    const GLvoid* offsetInByte = reinterpret_cast<GLvoid*>(startIndexLocation * _indexSize);
    GL_CHECK(glDrawElements(mode, indexCount, indexType, offsetInByte));
}

void GLContext::drawIndexed(GraphicsPrimitiveType primitive, uint32_t indexCount, uint32_t startIndexLocation, int32_t baseVertexLocation)
{
    EL_ASSERT(_program);
    bindVertexBuffers(_vertexBuffers);

    const GLenum indexType = asIndexType(_indexSize);
    EL_ASSERT(indexCount + startIndexLocation <= _indexCount); 
    EL_ASSERT(baseVertexLocation <= int32_t(_vertexCount));

    GLenum mode = asPrimitiveType(primitive);
    const GLvoid* offsetInByte = reinterpret_cast<GLvoid*>(startIndexLocation * _indexSize);

    // since Core 3.2 or requires ARB_draw_elements_base_vertex
    GL_CHECK(glDrawElementsBaseVertex(mode, indexCount, indexType, offsetInByte, baseVertexLocation));
}

void GLContext::drawInstanced(GraphicsPrimitiveType primitive, uint32_t vertexCountPerInstance, uint32_t instanceCount,
                              uint32_t startVertexLocation, uint32_t startInstanceLocation)
{
}

void GLContext::drawIndexedInstanced(GraphicsPrimitiveType primitive, uint32_t indexCountPerInstance, uint32_t instanceCount,
                                     uint32_t startIndexLocation, int32_t vaseVertexLocation, uint32_t startInstanceLocation)
{
}

void APIENTRY GLContext::debugCallback(GLenum source, GLenum type, GLuint id, GLenum severity,
                                              GLsizei length, const GLchar* message, const void* userParam)
{
    // ignore these non-significant error codes
    if (id == 131169 || id == 131185 || id == 131218 || id == 131204 || id == 131184)
        return;

    std::stringstream output;
    output << "---------- OPENGL CALLBACK -----------" << std::endl;
    output << "SOURCE: ";
    switch (source) {
    case GL_DEBUG_SOURCE_API:
        output << "WINDOW_SYSTEM";
        break;
    case GL_DEBUG_SOURCE_SHADER_COMPILER:
        output << "SHADER_COMPILER";
        break;
    case GL_DEBUG_SOURCE_THIRD_PARTY:
        output << "THIRD_PARTY";
        break;
    case GL_DEBUG_SOURCE_APPLICATION:
        output << "APPLICATION";
        break;
    case GL_DEBUG_SOURCE_OTHER:
        output << "OTHER";
        break;
    }
    output << std::endl;

    output << "TYPE: ";
    switch (type) {
    case GL_DEBUG_TYPE_ERROR:
        output << "ERROR";
        break;
    case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
        output << "DEPRECATED_BEHAVIOR";
        break;
    case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
        output << "UNDEFINED_BEHAVIOR";
        break;
    case GL_DEBUG_TYPE_PORTABILITY:
        output << "PORTABILITY";
        break;
    case GL_DEBUG_TYPE_PERFORMANCE:
        output << "PERFORMANCE";
        break;
    case GL_DEBUG_TYPE_OTHER:
        output << "OTHER";
        break;
    }
    output << std::endl;

    output << "SEVERITY : ";
    switch (severity) {
    case GL_DEBUG_SEVERITY_LOW:
        output << "LOW";
        break;
    case GL_DEBUG_SEVERITY_MEDIUM:
        output << "MEDIUM";
        break;
    case GL_DEBUG_SEVERITY_HIGH:
        output << "HIGH";
        break;
    }
    output << std::endl;
    output << message << std::endl;

    EL_TRACE("\n%s", output.str().c_str());
    el::debug_break();
}

#endif // #if EL_BUILD_OPENGL
