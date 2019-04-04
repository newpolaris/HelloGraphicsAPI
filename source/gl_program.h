#pragma once

#include <stdint.h>

#include "graphics_types.h"
#include "graphics_program.h"
#include "gl_types.h"
#include "linmath.h"

namespace el {

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

		void setTexture(const GraphicsTexturePtr& texture, GLint location, GLenum unit);

		const GraphicsProgramDesc& getProgramDesc() const override;

	private:

		GLuint _programID;
		GraphicsProgramDesc _programDesc;
	};

} // namespace el {
