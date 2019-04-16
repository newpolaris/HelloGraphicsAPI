#include "gl_program.h"

#include "debug.h"
#include "gl.h"
#include "gl_shader.h"
#include "gl_texture.h"
#include "gl_buffer.h"

#include <string>

namespace el {
    namespace gl {
        namespace program {
            typedef GLuint Handle;
            const Handle kUninitialized = Handle(0);

            bool isInitialized(const Handle& h)
            {
                return h != kUninitialized;
            }

            Handle create(const std::vector<GLuint>& shaderIDs)
            {
                // In opengl it is valid, but check logic error
                EL_ASSERT(shaderIDs.size() > 0);

                GLuint id = gl::CreateProgram();

                GLint status = 0;

                for (auto& shader : shaderIDs)
                    gl::AttachShader(id, shader);
                gl::LinkProgram(id);

                GL_CHECK(glGetProgramiv(id, GL_LINK_STATUS, &status));
                if (status == GL_FALSE || EL_CONFIG_DEBUG)
                {
                    const uint32_t kBufferSize = 512u;
                    char log[kBufferSize];
                    GL_CHECK(glGetProgramInfoLog(id, sizeof(log), nullptr, log));
                    if (log[0] != '\0')
                        EL_TRACE("%d: %s", status, log);
                }
                if (status == GL_FALSE)
                {
                    gl::DeleteProgram(id);
                    id = 0;
                }
                return Handle(id);
            }

            void destroy(Handle& handle)
            {
                if (isInitialized(handle))
                {
                    gl::DeleteProgram(handle);
                    handle = 0;
                }
            }

        } // namespace program {
    }
    // namespace gl {
} // namespace el {

using namespace el;

GLProgram::GLProgram() :
    _programID(0)
{
}

GLProgram::~GLProgram()
{
}

bool GLProgram::create(GraphicsProgramDesc desc)
{
    std::vector<GLuint> shaderIDs;
    for (auto& s : desc.getShaders())
    {
        auto shader = std::static_pointer_cast<GLShader>(s);
        shaderIDs.push_back(shader->getID());
    }
    GLuint id = gl::program::create(shaderIDs);
    if (id == 0)
        return false;

    _programID = id;
    _programDesc = std::move(desc);

    setupActiveUniform();
    setupActiveAttribute();

    return true;
}

void GLProgram::destory()
{
    gl::program::destroy(_programID);
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

struct SymbolicConstant
{
    std::string name;
    size_t size;
};

void GLProgram::setupActiveUniform()
{
    EL_ASSERT(_programID != 0);

    GLint count;
    GL_CHECK(glGetProgramiv(_programID, GL_ACTIVE_UNIFORMS, &count));

    uint32_t textureUnit = 0;

    const GLsizei bufSize = 16;
    for (GLint i = 0; i < count; i++)
    {
        GLchar nameBuf[bufSize] = { 0 };
        GLsizei length;
        GLenum type;
        GLint size;
        GL_CHECK(glGetActiveUniform(_programID, i, bufSize, &length, &size, &type, nameBuf));
        std::string name(nameBuf, length);

        EL_ASSERT(i == glGetUniformLocation(_programID, name.c_str()));

        GLUniform uniform;
        uniform.name = name;
        uniform.index = i;
        uniform.unit = 0;
        uniform.type = type;
        uniform.size = size;

        if (type == GL_SAMPLER_1D ||
            type == GL_SAMPLER_2D ||
            type == GL_SAMPLER_3D ||
            type == GL_SAMPLER_CUBE)
        {
            uniform.unit = textureUnit++;
        }
        _activeUniform[name] = std::move(uniform);
    }
}

void GLProgram::setupActiveAttribute()
{
    EL_ASSERT(_programID != 0);

    GLint count;
    GL_CHECK(glGetProgramiv(_programID, GL_ACTIVE_ATTRIBUTES, &count));

    const GLsizei bufSize = 16;
    for (GLint i = 0; i < count; i++)
    {
        GLchar nameBuf[bufSize] = { 0 };
        GLsizei length;
        GLint size;
        GLenum type;
        GLAttribute attribute;
        GL_CHECK(glGetActiveAttrib(_programID, i, bufSize, &length, &size, &type, nameBuf));
        std::string name(nameBuf, length);

        EL_ASSERT(i == glGetAttribLocation(_programID, name.c_str()));

        attribute.name = name;
        attribute.index = i;
        attribute.size = asVariableComponentCount(type);
        attribute.type = asVariableComponentType(type);
        _activeAttribute[name] = std::move(attribute);
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

void GLProgram::setUniform(const std::string& name, const vec3& v0)
{
    auto it = _activeUniform.find(name);
    if (it == _activeUniform.end())
    {
        EL_ASSERT(false);
        return;
    }
    auto& uniform = it->second;
    setUniform(uniform.index, v0);
}

void GLProgram::setUniform(const std::string& name, const mat4x4& m0)
{
    auto it = _activeUniform.find(name);
    if (it == _activeUniform.end())
    {
        EL_ASSERT(false);
        return;
    }
    auto& uniform = it->second;
    setUniform(uniform.index, m0);
}

void GLProgram::setUniform(const std::string& name, const GraphicsTexturePtr& texture)
{
    auto it = _activeUniform.find(name);
    if (it == _activeUniform.end())
    {
        EL_ASSERT(false);
        return;
    }
    auto& uniform = it->second;

    const auto& glTexture = std::static_pointer_cast<GLTexture>(texture);
    if (glTexture != nullptr)
        glTexture->bind(uniform.unit);
}

void GLProgram::setVertexBuffer(const std::string& name, const GraphicsBufferPtr& buffer, uint32_t stride, uint32_t offset)
{
    auto glBuffer = std::static_pointer_cast<GLBuffer>(buffer);
    if (glBuffer != nullptr)
        glBuffer->bind();
    EL_ASSERT(glBuffer != nullptr);

    auto it = _activeAttribute.find(name);
    if (it == _activeAttribute.end())
    {
        EL_ASSERT(false);
        return;
    }
    auto& attrib = it->second;
    setVertexBuffer(attrib.index, buffer, 2, GL_FLOAT, stride, offset);
}

void GLProgram::setVertexBuffer(GLint location, GLint size, GLenum type, GLsizei stride, const void * pointer)
{
    GL_CHECK(glEnableVertexAttribArray(location));
    GL_CHECK(glVertexAttribPointer(location, size, type, GL_FALSE, stride, (void*)pointer));
}

void GLProgram::setVertexBuffer(GLint location, const GraphicsBufferPtr& buffer, GLint size, GLenum type, GLsizei stride, GLsizei offset)
{
    auto glBuffer = std::static_pointer_cast<GLBuffer>(buffer);
    if (glBuffer != nullptr)
        glBuffer->bind();
    EL_ASSERT(glBuffer != nullptr);
    const GLvoid* pointer = reinterpret_cast<GLvoid*>(offset);
    GL_CHECK(glEnableVertexAttribArray(location));
    GL_CHECK(glVertexAttribPointer(location, size, type, GL_FALSE, stride, pointer));
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
