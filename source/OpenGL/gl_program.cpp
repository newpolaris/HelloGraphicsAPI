#include "gl_program.h"

#include "debug.h"
#include "gl.h"
#include "gl_shader.h"
#include "gl_texture.h"
#include "gl_buffer.h"

#include <algorithm>

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

        const GLuint locaation = glGetUniformLocation(_programID, name.c_str());

        GLUniform uniform;
        uniform.name = name;
        uniform.location = locaation;
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

    GLint attributes;
    GL_CHECK(glGetProgramiv(_programID, GL_ACTIVE_ATTRIBUTES, &attributes));
    GLint attributeMaxLength;
    GL_CHECK(glGetProgramiv(_programID, GL_ACTIVE_ATTRIBUTE_MAX_LENGTH, &attributeMaxLength));

    for (GLint i = 0; i < attributes; i++)
    {
        GLsizei length;
        GLint size;
        GLenum type;
        GLAttribute attribute;

        std::string name(attributeMaxLength, 0);
        GL_CHECK(glGetActiveAttrib(_programID, i, attributeMaxLength, &length, &size, &type, (GLchar*)name.data()));
        name = name.substr(0, length);

        const GLint location = glGetAttribLocation(_programID, name.c_str());

        attribute.name = name;
        attribute.location = location;
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
        // EL_ASSERT(false);
        return;
    }
    auto& uniform = it->second;
    setUniform(uniform.location, v0);
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
    setUniform(uniform.location, m0);
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

#if 0
D3D11_INPUT_ELEMENT_DESC layout[] = {
    { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
};
UINT numElements = ARRAYSIZE(layout);

auto bindingDescription = Vertex::getBindingDescription();
auto attributeDescriptions = Vertex::getAttributeDescription();

VkPipelineVertexInputStateCreateInfo vertexInputInfo = {};
vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
// vertexInputInfo.vertexBindingDescriptionCount = 1;
// vertexInputInfo.pVertexBindingDescriptions = &bindingDescription;
// vertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(attributeDescriptions.size());;
// vertexInputInfo.pVertexAttributeDescriptions = attributeDescriptions.data();

struct Vertex {
    glm::vec2 pos;
    glm::vec3 color;

    static VkVertexInputBindingDescription getBindingDescription() {
        VkVertexInputBindingDescription bindingDescription = {};
        bindingDescription.binding = 0;
        bindingDescription.stride = sizeof(Vertex);
        bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

        return bindingDescription;
    }

    static std::array<VkVertexInputAttributeDescription, 2> getAttributeDescription() {
        std::array<VkVertexInputAttributeDescription, 2> attributeDescriptions = {};

        attributeDescriptions[0].binding = 0;
        attributeDescriptions[0].location = 0;
        attributeDescriptions[0].format = VK_FORMAT_R32G32_SFLOAT;
        attributeDescriptions[0].offset = offsetof(Vertex, pos);

        attributeDescriptions[1].binding = 0;
        attributeDescriptions[1].location = 1;
        attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
        attributeDescriptions[1].offset = offsetof(Vertex, color);

        return attributeDescriptions;
    }
};
#endif

void GLProgram::setVertexBuffer(const GraphicsBufferPtr& buffer)
{
    auto glBuffer = std::static_pointer_cast<GLBuffer>(buffer);
    if (glBuffer != nullptr)
        glBuffer->bind();

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
    const GLvoid* pointer = reinterpret_cast<GLvoid*>(offset);
    GL_CHECK(glEnableVertexAttribArray(attrib.location));
    GL_CHECK(glVertexAttribPointer(attrib.location, attrib.size, attrib.type, GL_FALSE, stride, pointer));
}

void GLProgram::setVertexBuffer(GLint location, GLint size, GLenum type, GLsizei stride, const void* pointer)
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

void GLProgram::setIndexBuffer(const GraphicsBufferPtr& buffer)
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
