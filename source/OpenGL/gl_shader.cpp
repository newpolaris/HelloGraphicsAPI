#include "gl_shader.h"
#include "gl_types.h"
#include "gl.h"
#include "predefine.h"
#include "debug.h"

namespace el {
namespace shader {

	typedef GLuint Handle;

	const Handle kUninitialized = 0;

	bool isInitialized(const Handle& handle)
	{
		return handle != kUninitialized;
	}

	Handle create(GLenum type, const char* shaderCode)
	{
		GLuint id = gl::CreateShader(type);
		EL_ASSERT(id != 0);
		if (id != 0)
		{
			gl::ShaderSource(id, 1, &shaderCode, nullptr);
			gl::CompileShader(id);

			GLint compiled = 0;
			gl::GetShaderiv(id, GL_COMPILE_STATUS, &compiled);
			if (compiled == GL_FALSE)
			{
				GLint length = 0;
				gl::GetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
				std::vector<GLchar> buffer(length + 1);
				gl::GetShaderInfoLog(id, length, nullptr, buffer.data());
				el::trace("%s:%d %s\n", __FILE__, __LINE__, buffer.data());
				gl::DeleteShader(id);
				return 0;
			}
		}
		return Handle(id);
	}

	void destroy(const el::gl::program::Handle& phandle, Handle& handle)
	{
		if (isInitialized(handle))
		{
			gl::DetachShader(phandle, handle);
			gl::DeleteShader(handle);
			handle = 0;
		}
	}
} // namespace shader {
} // namespace el {

using namespace el;

GLShader::GLShader() : _id(0)
{
}

GLShader::~GLShader()
{
}

bool GLShader::create(GraphicsShaderStageFlagBits stage, const char* shaderCode)
{
	_stage = stage;
	_id = shader::create(getShaderStage(stage), shaderCode);
	if (_id == 0)
		return false;
	return true;
}

void GLShader::destroy(gl::program::Handle program)
{
	shader::destroy(program, _id);
}

GLuint GLShader::getID() const
{
	return _id;
}

const GraphicsShaderDesc& GLShader::getDesc() const noexcept
{
	return _shaderDesc;
}
