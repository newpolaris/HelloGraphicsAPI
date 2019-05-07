#include <OpenGL/gl_shader.h>

#include <OpenGL/gl_types.h>
#include <OpenGL/gl_headers.h>
#include <el_predefine.h>
#include <el_debug.h>

using namespace el;

GLShader::GLShader() : 
    _id(0),
    _stage(GL_VERTEX_SHADER)
{
}

GLShader::~GLShader()
{
    destroy();
}

bool GLShader::create(const GraphicsShaderDesc& desc)
{
    _stage = asShaderStage(desc.getStageFlag());

    _id = GL_CHECK_RET(glCreateShader(_stage));
    if (_id == 0)
        return false;

    const GLchar* shaderCode = desc.getShaderCode();
    if (shaderCode == nullptr)
        return false;
    
    GL_CHECK(glShaderSource(_id, 1, &shaderCode, nullptr));
    GL_CHECK(glCompileShader(_id));

    GLint compiled = 0;
    GL_CHECK(glGetShaderiv(_id, GL_COMPILE_STATUS, &compiled));
    if (compiled == GL_FALSE)
    {
        GLint length = 0;
        GL_CHECK(glGetShaderiv(_id, GL_INFO_LOG_LENGTH, &length));
        std::vector<GLchar> buffer(length + 1);
        GL_CHECK(glGetShaderInfoLog(_id, length, nullptr, buffer.data()));
        GL_CHECK(glDeleteShader(_id));
        _id = 0;
        EL_TRACE("%s", buffer.data());
        debug_break();
        return false;
    }
    return true;
}

void GLShader::destroy()
{
    GL_CHECK(glDeleteShader(_id));
    _id = 0;
}

GLuint GLShader::getID() const
{
    return _id;
}

const GraphicsShaderDesc& GLShader::getDesc() const
{
    return _shaderDesc;
}
