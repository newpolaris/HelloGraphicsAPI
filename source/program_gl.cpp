#include "program_gl.h"

#include "gl.h"
#include "debug.h"

namespace el {
namespace gl {
namespace program {

	bool isInitialized(const Handle& h)
	{
		return h != kUninitialized;
	}

	Handle create(const shader::Handle& vertex, const shader::Handle& fragment)
	{
		GLuint id = gl::CreateProgram();

		GLint status = 0;
		if (vertex != 0)
		{
			gl::AttachShader(id, vertex);
			if (fragment != 0)
				gl::AttachShader(id, fragment);
			gl::LinkProgram(id);
			GL_CHECK(glGetProgramiv(id, GL_LINK_STATUS, &status));

			if (status == GL_FALSE || EL_CONFIG_DEBUG)
			{
				const uint32_t kBufferSize = 512u;
				char log[kBufferSize];
				GL_CHECK(glGetProgramInfoLog(id, sizeof(log), nullptr, log));
				EL_TRACE("%d: %s", status, log);
			}
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
} // namespace gl {
} // namespace el {
