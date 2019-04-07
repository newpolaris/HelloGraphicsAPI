#include "gl_program.h"

#include "debug.h"
#include "gl.h"
#include "gl_shader.h"
#include "gl_texture.h"
#include "gl_buffer.h"

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

bool GLProgram::create(const GraphicsProgramDesc& desc)
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
	_programDesc = desc;

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

    GL_CHECK(glEnableVertexAttribArray(location));
    GL_CHECK(glVertexAttribPointer(location, size, type, GL_FALSE, stride, (void*)offset));
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
