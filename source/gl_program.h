#pragma once

#include <stdint.h>

#include "graphics_types.h"
#include "graphics_program.h"
#include "gl_types.h"
#include "linmath.h"

namespace el {

	class GLVertexBuffer;
	class GLProgram final : public GraphicsProgram
	{
	public:

		GLProgram();
		~GLProgram();

		bool create(const GraphicsProgramDesc& desc);
		void destory();

		GLuint getProgramID() const;

		void use() const;

		void setUniform(GLint location, const GLint& v0);
		void setUniform(GLint location, const vec3& v0);
		void setUniform(GLint location, const mat4x4& m0);

		void setVertexBuffer(GLint location, GLint size, GLenum type, GLsizei stride, const void *pointer);
		void setVertexBuffer(GLint location, const GLVertexBuffer& buffer, GLint size, GLenum type, GLsizei stride, GLsizei offset);
		void setTexture(GLint location, const GraphicsTexturePtr& texture, GLenum unit);

		const GraphicsProgramDesc& getProgramDesc() const override;

	private:

		GLuint _programID;
		GraphicsProgramDesc _programDesc;
	};

} // namespace el {
