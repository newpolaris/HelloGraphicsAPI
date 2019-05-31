#include "gl_program.h"

#include <el/debug.h>
#include "gl_headers.h"
#include "gl_shader.h"
#include "gl_texture.h"
#include "gl_buffer.h"
#include "gl_input_layout.h"

#include <algorithm>

namespace el {

    static const bool g_isQuiet = false;

} // namespace el {

using namespace el;

bool GLVertexAttribute::isMatch(const GraphicsInputAttribute& layout) const
{
    return name == layout.getName();
}

GLProgram::GLProgram() :
    _programID(0)
{
}

GLProgram::~GLProgram()
{
    destroy();
}

bool GLProgram::create(const GraphicsProgramDesc& desc)
{
    std::vector<GLuint> shaderIDs;
    for (auto& s : desc.getShaders())
    {
        auto shader = std::static_pointer_cast<GLShader>(s);
        shaderIDs.push_back(shader->getID());
    }
    
    if (shaderIDs.size() <= 0)
        return false;

    _programID = GL_CHECK_RET(glCreateProgram());
    if (_programID == 0)
        return false;

    GLint status = 0;
    for (auto& shader : shaderIDs)
        GL_CHECK(glAttachShader(_programID, shader));
    GL_CHECK(glLinkProgram(_programID));

    GL_CHECK(glGetProgramiv(_programID, GL_LINK_STATUS, &status));
    if (status == GL_FALSE || EL_CONFIG_DEBUG)
    {
        const uint32_t kBufferSize = 512u;
        char log[kBufferSize];
        GL_CHECK(glGetProgramInfoLog(_programID, sizeof(log), nullptr, log));
        if (log[0] != '\0')
            EL_TRACE("%d: %s", status, log);
    }
    if (status == GL_FALSE)
    {
        GL_CHECK(glDeleteProgram(_programID));
        _programID = 0;
    }
    _programDesc = desc;

    // NOTE:
    // glVertexAttribPointer's maximum index value
    // minimum values in OpenGL 2.1 = 16, ES 2.0 = 8
    // GL_CHECK(glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, (GLint*)&_limits.maxVertexAttributes));

    setupActiveUniform();
    setupActiveAttribute();

    return true;
}

void GLProgram::destroy()
{
    GL_CHECK(glDeleteProgram(_programID));
    _programID = 0;

}

GLuint GLProgram::getProgramID() const
{
    return _programID;
}

void GLProgram::apply() const
{
    GL_CHECK(glUseProgram(_programID));
}

const GraphicsProgramDesc& GLProgram::getProgramDesc() const
{
    return _programDesc;
}

const GraphicsVertexAttributes& GLProgram::getVertexAttributes() const
{
    return _activeAttributeList;
}

void GLProgram::setupActiveUniform()
{
    EL_ASSERT(_programID != 0);

    GLint uniforms;
    GL_CHECK(glGetProgramiv(_programID, GL_ACTIVE_UNIFORMS, &uniforms));
    GLint uniformMaxLength;
    GL_CHECK(glGetProgramiv(_programID, GL_ACTIVE_UNIFORM_MAX_LENGTH, &uniformMaxLength));

    uint32_t textureUnit = 0;
    
    for (GLint i = 0; i < uniforms; i++)
    {
        GLsizei length;
        GLenum type;
        GLint size;
        std::string name(uniformMaxLength, 0);
        GL_CHECK(glGetActiveUniform(_programID, i, uniformMaxLength, &length, &size, &type, (GLchar*)name.data()));
        name = name.substr(0, length);

        const GLuint location = glGetUniformLocation(_programID, name.c_str());

        GLUniform uniform;
        uniform.name = name;
        uniform.location = location;
        uniform.unit = 0;
        uniform.type = asVariableComponentType(type);
        uniform.count = asVariableComponentCount(type);;

        if (type == GL_SAMPLER_1D ||
            type == GL_SAMPLER_2D ||
            type == GL_SAMPLER_3D ||
            type == GL_SAMPLER_CUBE)
        {
            uniform.unit = textureUnit++;
        }

        _activeUniforms.emplace(name, std::move(uniform));
    }
}

void GLProgram::setupActiveAttribute()
{
    EL_ASSERT(_programID != 0);

    GLint attributes;
    GL_CHECK(glGetProgramiv(_programID, GL_ACTIVE_ATTRIBUTES, &attributes));
    GLint attributeMaxLength;
    GL_CHECK(glGetProgramiv(_programID, GL_ACTIVE_ATTRIBUTE_MAX_LENGTH, &attributeMaxLength));

    for (GLint i = 0; i < attributes; i++)
    {
        GLsizei length;
        GLint size;
        GLenum type;
        GLVertexAttribute attribute;
        std::string name(attributeMaxLength, 0);
        GL_CHECK(glGetActiveAttrib(_programID, i, attributeMaxLength, &length, &size, &type, (GLchar*)name.data()));
        name = name.substr(0, length);

        const GLint location = GL_CHECK_RET(glGetAttribLocation(_programID, name.c_str()));
        EL_WARN_IF(location >= _limits.maxVertexAttributes, "!!Exceeded the limits maximum vertex attributes");

        attribute.name = name;
        attribute.location = location;
        attribute.count = asVariableComponentCount(type);
        attribute.type = asVariableComponentType(type);

        _activeAttributes.emplace(name, attribute);

        auto attrib = std::make_shared<GLVertexAttribute>(attribute);
        _activeAttributeList.push_back(attrib);
    }
}

void GLProgram::setUniform(GLint location, const GLint& v0)
{
    GL_CHECK(glUniform1i(location, v0));
}

void GLProgram::setUniform(GLint location, const vec3& v0)
{
    GL_CHECK(glUniform3fv(location, 1, v0));
}

void GLProgram::setUniform(GLint location, const mat4x4& m0)
{
    GL_CHECK(glUniformMatrix4fv(location, 1, GL_FALSE, (const GLfloat*)m0));
}

void GLProgram::setUniform(const std::string& name, float v0)
{
    auto it = _activeUniforms.find(name);
    if (it == _activeUniforms.end())
    {
        EL_WARN_IF(!g_isQuiet, "Can't find uniform '%s'\n", name.c_str());
        return;
    }
    auto& uniform = it->second;
    GL_CHECK(glUniform1f(uniform.location, v0));
}

void GLProgram::setUniform(const std::string& name, const vec3& v0)
{
    auto it = _activeUniforms.find(name);
    if (it == _activeUniforms.end())
    {
        EL_WARN_IF(!g_isQuiet, "Can't find uniform '%s'\n", name.c_str());
        return;
    }
    auto& uniform = it->second;
    GL_CHECK(glUniform3fv(uniform.location, 1, v0));
}

void GLProgram::setUniform(const std::string& name, const vec4& v0)
{
    auto it = _activeUniforms.find(name);
    if (it == _activeUniforms.end())
    {
        EL_WARN_IF(!g_isQuiet, "Can't find uniform '%s'\n", name.c_str());
        return;
    }
    auto& uniform = it->second;
    GL_CHECK(glUniform4fv(uniform.location, 1, v0));
}

void GLProgram::setUniform(const std::string& name, const mat4x4& m0)
{
    auto it = _activeUniforms.find(name);
    if (it == _activeUniforms.end())
    {
        EL_WARN_IF(!g_isQuiet, "Can't find uniform '%s'\n", name.c_str());
        return;
    }
    auto& uniform = it->second;
    GL_CHECK(glUniformMatrix4fv(uniform.location, 1, GL_FALSE, (const GLfloat*)m0));
}

void GLProgram::setUniform(const std::string& name, const GraphicsTexturePtr& texture)
{
    auto it = _activeUniforms.find(name);
    if (it == _activeUniforms.end())
    {
        EL_WARN_IF(!g_isQuiet, "Can't find uniform '%s'\n", name.c_str());
        return;
    }
    auto& uniform = it->second;

    const auto& glTexture = std::static_pointer_cast<GLTexture>(texture);
    if (glTexture != nullptr)
        glTexture->bind(uniform.unit);
}

void GLProgram::setVertexBuffer(const GraphicsDataPtr& buffer)
{
#if 0
    auto& bindings = _inputLayout->getDesc().getBindings();

    auto glBuffer = std::static_pointer_cast<GLBuffer>(buffer);
    if (glBuffer != nullptr)
        glBuffer->bind();


    auto& desc = _inputLayout->getDesc();
    auto& bindings = desc.getBindings()[binding];
    for (auto& location : desc.getAttributes()[binding])
    {
        auto it = _activeAttribute.find(location.name);
        if (it == _activeAttribute.end()) {
            EL_ASSERT(false);
            continue;
        }
        EL_ASSERT(asSize(it->type) == asSize(location.getFormat()));
        const uint32_t stride = bindings.getStride();
        uint32_t offset = 0;

        const GLvoid* pointer = reinterpret_cast<GLvoid*>(offset);
        GL_CHECK(glEnableVertexAttribArray(it->location));
        GL_CHECK(glVertexAttribPointer(it->location, it->size, it->type, GL_FALSE, stride, pointer));
    }
#endif
}

void GLProgram::setVertexBuffer(const std::string& name, const GraphicsDataPtr& buffer, uint32_t stride, uint32_t offset)
{
    auto it = _activeAttributes.find(name);
    if (it == _activeAttributes.end())
    {
        EL_WARN_IF(!g_isQuiet, "Can't find attributes '%s'\n", name.c_str());
        return;
    }

    auto glBuffer = std::static_pointer_cast<GLBuffer>(buffer);
    if (glBuffer != nullptr)
        glBuffer->bind();
    EL_ASSERT(glBuffer != nullptr);

    auto& attrib = it->second;
    const GLvoid* pointer = reinterpret_cast<GLvoid*>(offset);
    GL_CHECK(glEnableVertexAttribArray(attrib.location));
    GL_CHECK(glVertexAttribPointer(attrib.location, attrib.count, attrib.type, GL_FALSE, stride, pointer));
}

void GLProgram::setVertexBuffer(GLint location, GLint size, GLenum type, GLsizei stride, const void* pointer)
{
    GL_CHECK(glEnableVertexAttribArray(location));
    GL_CHECK(glVertexAttribPointer(location, size, type, GL_FALSE, stride, (void*)pointer));
}

void GLProgram::setVertexBuffer(GLint location, const GraphicsDataPtr& buffer, GLint size, GLenum type, GLsizei stride, GLsizei offset)
{
    auto glBuffer = std::static_pointer_cast<GLBuffer>(buffer);
    if (glBuffer != nullptr)
        glBuffer->bind();
    EL_ASSERT(glBuffer != nullptr);
    const GLvoid* pointer = reinterpret_cast<GLvoid*>(offset);
    GL_CHECK(glEnableVertexAttribArray(location));
    GL_CHECK(glVertexAttribPointer(location, size, type, GL_FALSE, stride, pointer));
}

void GLProgram::setIndexBuffer(const GraphicsDataPtr& buffer)
{
    auto glBuffer = std::static_pointer_cast<GLBuffer>(buffer);
    if (glBuffer != nullptr)
        glBuffer->bind();
    EL_ASSERT(glBuffer != nullptr);
}

void GLProgram::setTexture(GLint location, const GraphicsTexturePtr& texture, GLenum unit)
{
    auto glTexture = std::static_pointer_cast<GLTexture>(texture);
    if (glTexture != nullptr)
    {
        glTexture->bind(unit);
        setUniform(location, unit);
    }
    EL_ASSERT(glTexture != nullptr);
}

void GLProgram::setInputLayout(const GraphicsInputLayoutPtr& layout)
{
    _inputLayout = layout;
}
