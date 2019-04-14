#pragma once

#include "predefine.h"
#include "gl.h"
#include "graphics_types.h"
#include "graphics_shader.h"

namespace el {
	namespace gl {
		namespace program
		{
			typedef GLuint Handle;
		}
	}

	class GLShader final : public GraphicsShader
	{
	public:

		GLShader();
		~GLShader();

		bool create(GraphicsShaderStageFlagBits stage, const char* shaderCode);
		void destroy(gl::program::Handle program);

		GLuint getID() const;

		virtual const GraphicsShaderDesc& getDesc() const noexcept;

	private:

		GLuint _id;
		GraphicsShaderStageFlagBits _stage;
		GraphicsShaderDesc _shaderDesc;
	};

} // namespace el {
