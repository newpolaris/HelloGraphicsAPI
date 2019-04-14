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

void GLProgram::use() const
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

size_t el::asTypeSize(GLenum type)
{
    switch(type)
    {
    case GL_FLOAT: return 4*1;
    case GL_FLOAT_VEC2: return 4*2;
    case GL_FLOAT_VEC3: return 4*3;
    case GL_FLOAT_VEC4: return 4*4;
    case GL_INT: return 4*1;
    case GL_INT_VEC2: return 4*2;
    case GL_INT_VEC3: return 4*3;
    case GL_INT_VEC4: return 4*4;
    case GL_UNSIGNED_INT: return 4*1;
    case GL_UNSIGNED_INT_VEC2: return 4*2;
    case GL_UNSIGNED_INT_VEC3: return 4*3;
    case GL_UNSIGNED_INT_VEC4: return 4*4;
    case GL_BOOL: return 4*1;
    case GL_BOOL_VEC2: return 4*2;
    case GL_BOOL_VEC3: return 4*3;
    case GL_BOOL_VEC4: return 4*4;
    case GL_FLOAT_MAT2: return 4*2;
    case GL_FLOAT_MAT3: return 4*3;
    case GL_FLOAT_MAT4: return 4*4;
    case GL_FLOAT_MAT2x3: return 4*2*3;
    case GL_FLOAT_MAT2x4: return 4*2*4;
    case GL_FLOAT_MAT3x2: return 4*3*2;
    case GL_FLOAT_MAT3x4: return 4*3*4;
    case GL_FLOAT_MAT4x2: return 4*4*2;
    case GL_FLOAT_MAT4x3: return 4*4*3;
    case GL_SAMPLER_2D: return 4*1;
    case GL_SAMPLER_3D: return 4*1;
    case GL_SAMPLER_CUBE: return 4*1;
    default: EL_ASSERT(false); return 0;
    }
}

std::string asType(GLenum type)
{
    switch(type)
    {
    case GL_FLOAT: return "float";
    case GL_FLOAT_VEC2: return "vec2";
    case GL_FLOAT_VEC3: return "vec3";
    case GL_FLOAT_VEC4: return "vec4";
    case GL_INT: return "int";
    case GL_INT_VEC2: return "ivec2";
    case GL_INT_VEC3: return "ivec3";
    case GL_INT_VEC4: return "ivec4";
    case GL_UNSIGNED_INT: return "unsigned int";
    case GL_UNSIGNED_INT_VEC2: return "uvec2";
    case GL_UNSIGNED_INT_VEC3: return "uvec3";
    case GL_UNSIGNED_INT_VEC4: return "uvec4";
    case GL_BOOL: return "bool";
    case GL_BOOL_VEC2: return "bvec2";
    case GL_BOOL_VEC3: return "bvec3";
    case GL_BOOL_VEC4: return "bvec4";
    case GL_FLOAT_MAT2: return "mat2";
    case GL_FLOAT_MAT3: return "mat3";
    case GL_FLOAT_MAT4: return "mat4";
    case GL_FLOAT_MAT2x3: return "mat2x3";
    case GL_FLOAT_MAT2x4: return "mat2x4";
    case GL_FLOAT_MAT3x2: return "mat3x2";
    case GL_FLOAT_MAT3x4: return "mat3x4";
    case GL_FLOAT_MAT4x2: return "mat4x2";
    case GL_FLOAT_MAT4x3: return "mat4x3";
    case GL_SAMPLER_2D: return "sampler2D";
    case GL_SAMPLER_3D: return "sampler3D";
    case GL_SAMPLER_CUBE: return "samplerCube";
    default: EL_ASSERT(false); return "";
    }
}

void GLProgram::setupActiveUniform()
{
    EL_ASSERT(_programID != 0);
    
    GLint count;
    GL_CHECK(glGetProgramiv(_programID, GL_ACTIVE_UNIFORMS, &count));
    
    const GLsizei bufSize = 16;
    for (GLint i = 0; i < count; i++)
    {
        GLchar nameBuf[bufSize] = { 0 };
        GLsizei length;
        GLUniform uniform;
        GL_CHECK(glGetActiveUniform(_programID, i, bufSize, &length, &uniform.size, &uniform.type, nameBuf));
        std::string name(nameBuf, length);
        uniform.name = name;
        uniform.index = i;
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
        GLAttribute attribute;
        GL_CHECK(glGetActiveAttrib(_programID, i, bufSize, &length, &attribute.size, &attribute.type, nameBuf));
        std::string name(nameBuf, length);
        attribute.name = name;
        attribute.index = i;
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
