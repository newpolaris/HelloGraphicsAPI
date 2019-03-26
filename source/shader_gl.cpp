#include "Shader_gl.h"
#include "gl.h"
#include "predefine.h"
#include "debug.h"
#include <vector>
#include <cassert>

namespace el {
namespace gl {
namespace shader {

	bool isInitialized(const Handle& handle)
	{
		return handle != kUninitialized;
	}

	Handle create(GLenum type, const char* shaderCode)
	{
		GLuint id = gl::CreateShader(type);
		// TODO: possible ?
		assert(id != 0 && "Failed to create shader");
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

	void destroy(const program::Handle& phandle, Handle& handle)
	{
		if (isInitialized(handle))
		{
			gl::DetachShader(phandle, handle);
			gl::DeleteShader(handle);
			handle = 0;
		}
	}
}

} // namespace el {
} // namespace gl {
