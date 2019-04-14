#pragma once

#include "predefine.h"

#if EL_PLAT_IOS
#include <OpenGLES/ES2/gl.h>
#elif EL_PLAT_ANDROID
#include <GLES2/gl2.h>
#elif EL_PLAT_WINDOWS || EL_PLAT_OSX
#include <glad/glad.h>
#endif

#ifndef GL_STACK_OVERFLOW   
#define GL_STACK_OVERFLOW 0x0503
#endif
#ifndef GL_STACK_UNDERFLOW
#define GL_STACK_UNDERFLOW 0x0504
#endif
#ifndef GL_CONTEXT_LOST
#define GL_CONTEXT_LOST 0x0507
#endif

// Fix-up naming differences between OpenGL and OpenGL ES
// TODO:
#define glMapBufferOES glMapBuffer
#define glUnmapBufferOES glUnmapBuffer

namespace el {
namespace gl {
	bool error_handler(const char* file, int line);
} // namespace gl
} // namespace el

#define _GL_CHECK(_call) \
	do { \
		_call; \
		el::gl::error_handler(__FILE__, __LINE__); \
	} while (0)

#if EL_CONFIG_DEBUG
#    define GL_CHECK(_call) _GL_CHECK(_call)
#else
#    define GL_CHECK(_call) ((void)(_call))
#endif

#define _EMPTY_OUT_ 0

namespace el {
namespace gl {
	const GLubyte* GetString(GLenum name);

	GLint GetAttribLocation(GLuint program, const GLchar* name);
	GLint GetUniformLocation(GLuint program, const GLchar* name);
	void GetShaderiv(GLuint shader, GLenum pname, GLint* param);
	void GetShaderInfoLog(GLuint shader, GLsizei bufSize, GLsizei* length, GLchar* infoLog);
	GLuint CreateShader(GLenum type);
	void DeleteShader(GLuint shader);
	void ShaderSource(GLuint shader, GLsizei count, const GLchar *const* string, const GLint* length);
	void CompileShader(GLuint shader);
	GLuint CreateProgram(void);
	void DeleteProgram(GLuint program);
	void AttachShader(GLuint program, GLuint shader);
	void DetachShader(GLuint program, GLuint shader);
	void UseProgram(GLuint program);
	void LinkProgram(GLuint program);

	void Viewport(GLint x, GLint y, GLsizei width, GLsizei height);

	void GenTextures(GLsizei n, GLuint *textures);
	void DeleteTextures(GLsizei n, const GLuint *textures);
	void GenFramebuffers(GLsizei n, GLuint* framebuffers);
	void DeleteFramebuffers(GLsizei n, const GLuint* framebuffers);
	void DeleteRenderbuffers(GLsizei n, const GLuint* renderbuffers);
	void GenRenderbuffers(GLsizei n, GLuint* renderbuffers);
	void BindTexture(GLenum target, GLuint texture);
	void BindFramebuffer(GLenum target, GLuint framebuffer);
	void BindRenderbuffer(GLenum target, GLuint renderbuffer);
	void FramebufferRenderbuffer(GLenum target, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer);

	void TexParameteri(GLuint texture, GLenum pname, GLint param);
	void TexImage2D(GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const void *pixels);

	void PixelStorei(GLenum pname, GLint param);

	void EnableVertexAttribArray(GLuint index);
	void DisableVertexAttribArray(GLuint index);

	void Enable(GLenum cap);
	void Disable(GLenum cap);

	void VertexAttribPointer(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void* pointer);
	void ReadPixels(GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, void *pixels);
	void DrawArrays(GLenum mode, GLint first, GLsizei count);
	void DrawElements(GLenum mode, GLsizei count, GLenum type, const void *indices);

	void CullFace(GLenum mode);
	void BlendFunc(GLenum sfactor, GLenum dfactor);
	void Clear(GLbitfield mask);
	void ClearColor(GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha);

#ifndef OPENGL_ES
	void ClearDepth(GLclampd depth);
#else
	void ClearDepthf(GLclampf depth);
#endif

	void Uniform1f(GLint location, GLfloat v0);
	void Uniform2f(GLint location, GLfloat v0, GLfloat v1);
	void Uniform3f(GLint location, GLfloat v0, GLfloat v1, GLfloat v2);
	void Uniform4f(GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3);
	void Uniform4fv(GLint location, GLsizei count, const GLfloat* value);
	void Uniform1i(GLint location, GLint v0);
	void UniformMatrix4(GLint location, const GLfloat* value);
	void UniformMatrix4fv(GLint location, GLsizei count, const GLfloat* value);
} // namespace gl {
} // namespace el {
