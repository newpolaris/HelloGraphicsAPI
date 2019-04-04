#pragma once

#include <stdint.h>

#include "graphics_types.h"
#include "graphics_program.h"
#include "gl_types.h"

namespace el {

	class GLProgram final : public GraphicsProgram
	{
	public:

		GLProgram();
		~GLProgram();

		bool create(const GraphicsProgramDesc &desc);
		void destory();

		GLuint getProgramID() const;

		void use() const;

		const GraphicsProgramDesc& getProgramDesc() const override;

	private:

		GLuint _programID;
		GraphicsProgramDesc _programDesc;
	};

} // namespace el {
