#include "gl.h"
#include <string>
#include "debug.h"

namespace el {
	bool gl::error_handler(const char* file, int line) 
	{
		GLenum error = glGetError();
		if (error == GL_NO_ERROR)
			return true;
		do {
			std::string message;
			switch (error)
			{
			case GL_INVALID_ENUM:					message = "GL_INVALID_ENUM"; break;
			case GL_INVALID_VALUE:					message = "GL_INVALID_VALUE"; break;
			case GL_INVALID_OPERATION:				message = "GL_INVALID_OPERATION"; break;
			case GL_OUT_OF_MEMORY:					message = "GL_OUT_OF_MEMORY"; break;
			case GL_INVALID_FRAMEBUFFER_OPERATION:  message = "GL_INVALID_FRAMEBUFFER_OPERATION"; break;
			case GL_STACK_OVERFLOW:					message = "GL_STACK_OVERFLOW"; break;
			case GL_STACK_UNDERFLOW:				message = "GL_STACK_UNDERFLOW"; break;
			case GL_CONTEXT_LOST:					message = "GL_CONTEXT_LOST"; break;
			}

			EL_TRACE("GL_ERROR: %d %s %s:%d\n", error, message.c_str(), file, line);
			debug_break();

		} while (GL_NO_ERROR != (error = glGetError()));
		return false;
	}

#if _EMPTY_OUT_ 

	const GLubyte* gl::GetString(GLenum name) { return nullptr; }
	GLint gl::GetAttribLocation(GLuint program, const GLchar* name) { return 0; }
	GLint gl::GetUniformLocation(GLuint program, const GLchar* name) { return 0; }
	void gl::GetShaderiv(GLuint shader, GLenum pname, GLint* param) {}
	void gl::GetShaderInfoLog(GLuint shader, GLsizei bufSize, GLsizei* length, GLchar* infoLog) {}
	GLuint gl::CreateShader(GLenum type) { return 0; }
	void gl::DeleteShader(GLuint shader) {}
	void gl::ShaderSource(GLuint shader, GLsizei count, const GLchar *const* string, const GLint* length) {}
	void gl::CompileShader(GLuint shader) {}
	GLuint gl::CreateProgram(void) { return 0; }
	void gl::DeleteProgram(GLuint program) {}
	void gl::AttachShader(GLuint program, GLuint shader) {}
	void gl::DetachShader(GLuint program, GLuint shader) {}
	void gl::UseProgram(GLuint program) {}
	void gl::LinkProgram(GLuint program) {}
	void gl::Viewport(GLint x, GLint y, GLsizei width, GLsizei height) {}
	void gl::GenTextures(GLsizei n, GLuint *textures) {}
	void gl::DeleteTextures(GLsizei n, const GLuint *textures) {}
	void gl::GenFramebuffers(GLsizei n, GLuint* framebuffers) {}
	void gl::DeleteFramebuffers(GLsizei n, const GLuint* framebuffers) {}
	void gl::GenRenderbuffers(GLsizei n, GLuint* renderbuffers) { }
	void gl::DeleteRenderbuffers(GLsizei n, const GLuint* renderbuffers) {}
	void gl::BindTexture(GLenum target, GLuint texture) {}
	void gl::BindFramebuffer(GLenum target, GLuint framebuffer) {}
	void gl::BindRenderbuffer(GLenum target, GLuint renderbuffer) {}
	void gl::FramebufferRenderbuffer(GLenum target, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer) {}
	void gl::TexParameteri(GLuint texture, GLenum pname, GLint param) {}
	void gl::TexImage2D(GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const void *pixels) {}

	void gl::PixelStorei(GLenum pname, GLint param)
	{
	}


	void gl::EnableVertexAttribArray(GLuint index)
	{
	}

	void gl::DisableVertexAttribArray(GLuint index)
	{
	}

	void gl::Enable(GLenum cap) { }

	void gl::Disable(GLenum cap) { }

	void gl::VertexAttribPointer(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void* pointer) { }
	void gl::ReadPixels(GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, void *pixels) { }
	void gl::DrawArrays(GLenum mode, GLint first, GLsizei count) { }
	void gl::DrawElements(GLenum mode, GLsizei count, GLenum type, const void *indices) { }

	void gl::CullFace(GLenum mode) { }

	void gl::BlendFunc(GLenum sfactor, GLenum dfactor)
	{
	}

	void gl::Clear(GLbitfield mask)
	{
	}

	void gl::ClearColor(GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha)
	{
	}

#ifndef OPENGL_ES
	void gl::ClearDepth(GLclampd depth)
	{
	}
#else
	void gl::ClearDepthf(GLclampf depth)
	{
	}
#endif

	void gl::Uniform1f(GLint location, GLfloat v0)
	{
	}

	void gl::Uniform2f(GLint location, GLfloat v0, GLfloat v1)
	{
	}

	void gl::Uniform3f(GLint location, GLfloat v0, GLfloat v1, GLfloat v2)
	{
	}

	void gl::Uniform4f(GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3)
	{
	}

	void gl::Uniform4fv(GLint location, GLsizei count, const GLfloat* value)
	{
	}

	void gl::UniformMatrix4fv(GLint location, GLsizei count, const GLfloat* value)
	{
	}

	void gl::Uniform1i(GLint location, GLint v0)
	{
	}

#else // _EMPTY_OUT

	const GLubyte* gl::GetString(GLenum name)
	{
		const GLubyte* str = glGetString(name);
		GL_CHECK(str);
		return str;
	}

	GLint gl::GetAttribLocation(GLuint program, const GLchar* name)
	{
		return glGetAttribLocation(program, name);
	}

	GLint gl::GetUniformLocation(GLuint program, const GLchar* name)
	{
		return glGetUniformLocation(program, name);;
	}

	void gl::GetShaderiv(GLuint shader, GLenum pname, GLint* param)
	{
		GL_CHECK(glGetShaderiv(shader, pname, param));
	}

	void gl::GetShaderInfoLog(GLuint shader, GLsizei bufSize, GLsizei* length, GLchar* infoLog)
	{
		GL_CHECK(glGetShaderInfoLog(shader, bufSize, length, infoLog));
	}

	GLuint gl::CreateShader(GLenum type)
	{
		return glCreateShader(type);
	}

	void gl::DeleteShader(GLuint shader)
	{
		GL_CHECK(glDeleteShader(shader));
	}

	void gl::ShaderSource(GLuint shader, GLsizei count, const GLchar *const* string, const GLint* length)
	{
		GL_CHECK(glShaderSource(shader, count, string, length));
	}

	void gl::CompileShader(GLuint shader)
	{
		GL_CHECK(glCompileShader(shader));
	}

	GLuint gl::CreateProgram()
	{
		return glCreateProgram();
	}

	void gl::DeleteProgram(GLuint program)
	{
		GL_CHECK(glDeleteProgram(program));
	}

	void gl::AttachShader(GLuint program, GLuint shader)
	{
		GL_CHECK(glAttachShader(program, shader));
	}

	void gl::DetachShader(GLuint program, GLuint shader)
	{
		GL_CHECK(glDetachShader(program, shader));
	}

	void gl::UseProgram(GLuint program)
	{
		GL_CHECK(glUseProgram(program));
	}

	void gl::LinkProgram(GLuint program)
	{
		GL_CHECK(glLinkProgram(program));
	}

	void gl::Viewport(GLint x, GLint y, GLsizei width, GLsizei height)
	{
		GL_CHECK(glViewport(x, y, width, height));
	}

	void gl::GenTextures(GLsizei n, GLuint *textures)
	{
		GL_CHECK(glGenTextures(n, textures));
	}

	void gl::DeleteTextures(GLsizei n, const GLuint *textures)
	{
		GL_CHECK(glDeleteTextures(n, textures));
	}

	void gl::GenFramebuffers(GLsizei n, GLuint* framebuffers)
	{
		GL_CHECK(glGenFramebuffers(n, framebuffers));
	}

	void gl::DeleteFramebuffers(GLsizei n, const GLuint* framebuffers)
	{
		GL_CHECK(glDeleteFramebuffers(n, framebuffers));
	}

	void gl::GenRenderbuffers(GLsizei n, GLuint* renderbuffers)
	{
		GL_CHECK(glGenRenderbuffers(n, renderbuffers));
	}

	void gl::DeleteRenderbuffers(GLsizei n, const GLuint* renderbuffers)
	{
		GL_CHECK(glDeleteRenderbuffers(n, renderbuffers));
	}

	void gl::BindTexture(GLenum target, GLuint texture)
	{
		GL_CHECK(glBindTexture(target, texture));
	}

	void gl::BindFramebuffer(GLenum target, GLuint framebuffer)
	{
		GL_CHECK(glBindFramebuffer(target, framebuffer));
	}

	void gl::BindRenderbuffer(GLenum target, GLuint renderbuffer)
	{
		GL_CHECK(glBindRenderbuffer(target, renderbuffer));
	}

	void gl::FramebufferRenderbuffer(GLenum target, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer)
	{
		GL_CHECK(glFramebufferRenderbuffer(target, attachment, renderbuffertarget, renderbuffer));
	}

	void gl::TexParameteri(GLuint texture, GLenum pname, GLint param)
	{
		GL_CHECK(glTexParameteri(texture, pname, param));
	}

	void gl::TexImage2D(GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const void *pixels)
	{
		GL_CHECK(glTexImage2D(target, level, internalformat, width, height, border, format, type, pixels));
	}

	void gl::PixelStorei(GLenum pname, GLint param)
	{
		GL_CHECK(glPixelStorei(pname, param));
	}

	void gl::EnableVertexAttribArray(GLuint index)
	{
		GL_CHECK(glEnableVertexAttribArray(index));
	}

	void gl::DisableVertexAttribArray(GLuint index)
	{
		GL_CHECK(glDisableVertexAttribArray(index));
	}

	void gl::Enable(GLenum cap)
	{
		GL_CHECK(glEnable(cap));
	}

	void gl::Disable(GLenum cap)
	{
		GL_CHECK(glDisable(cap));
	}

	void gl::VertexAttribPointer(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void* pointer)
	{
		GL_CHECK(glVertexAttribPointer(index, size, type, normalized, stride, pointer));
	}

	void gl::ReadPixels(GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, void *pixels)
	{
		GL_CHECK(glReadPixels(x, y, width, height, format, type, pixels));
	}

	void gl::DrawArrays(GLenum mode, GLint first, GLsizei count)
	{
		GL_CHECK(glDrawArrays(mode, first, count));
	}

	void gl::DrawElements(GLenum mode, GLsizei count, GLenum type, const void *indices)
	{
		GL_CHECK(glDrawElements(mode, count, type, indices));
	}

	void gl::CullFace(GLenum mode)
	{
		GL_CHECK(glCullFace(mode));
	}

	void gl::BlendFunc(GLenum sfactor, GLenum dfactor)
	{
		GL_CHECK(glBlendFunc(sfactor, dfactor));
	}

	void gl::Clear(GLbitfield mask)
	{
		GL_CHECK(glClear(mask));
	}

	void gl::ClearColor(GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha)
	{
		GL_CHECK(glClearColor(red, green, blue, alpha));
	}

	// TODO: Fix
#ifndef OPENGL_ES
	void gl::ClearDepth(GLclampd depth)
	{
		GL_CHECK(glClearDepth(depth));
	}
#else
	void gl::ClearDepthf(GLclampf depth)
	{
		GL_CHECK(glClearDepthf(depth));
	}
#endif // #ifdef GLEWAP

	void gl::Uniform1f(GLint location, GLfloat v0)
	{
		GL_CHECK(glUniform1f(location, v0));
	}

	void gl::Uniform2f(GLint location, GLfloat v0, GLfloat v1)
	{
		GL_CHECK(glUniform2f(location, v0, v1));
	}

	void gl::Uniform3f(GLint location, GLfloat v0, GLfloat v1, GLfloat v2)
	{
		GL_CHECK(glUniform3f(location, v0, v1, v2));
	}

	void gl::Uniform4f(GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3)
	{
		GL_CHECK(glUniform4f(location, v0, v1, v2, v3));
	}

	void gl::Uniform4fv(GLint location, GLsizei count, const GLfloat* value)
	{
		GL_CHECK(glUniform4fv(location, count, value));
	}

	void gl::UniformMatrix4(GLint location, const GLfloat* value)
	{
		GL_CHECK(glUniformMatrix4fv(location, 1, false, value));
	}

	void gl::UniformMatrix4fv(GLint location, GLsizei count, const GLfloat* value)
	{
		GL_CHECK(glUniformMatrix4fv(location, count, false, value));
	}

	void gl::Uniform1i(GLint location, GLint v0)
	{
		GL_CHECK(glUniform1i(location, v0));
	}
#endif // _EMPTY_OUT

} // namespace el

