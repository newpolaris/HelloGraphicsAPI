#pragma once

#include "predefine.h"

#if EL_BUILD_OPENGL

#include <graphics_device_context.h>
#include "gl_types.h"

namespace el {

	class GLDeviceContext final : public GraphicsDeviceContext
	{
	public:

		GLDeviceContext();
		~GLDeviceContext();

        bool create();
        void destory();

        void setViewport(int32_t x, int32_t y, uint32_t width, uint32_t height) override;
        void setProgram(const GraphicsProgramPtr& ptr) override;
        void setTexture(const std::string& name, const GraphicsTexturePtr& texture) override;
        void setVertexBuffer(const std::string& name, vertex_buffer, vertices, )
        
	private:

        GLProgramPtr _program;
	};

} // namespace el {

#endif // #if EL_BUILD_OPENGL
