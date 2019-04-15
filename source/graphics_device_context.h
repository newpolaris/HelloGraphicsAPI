#pragma once

#include "predefine.h"
#include <graphics_types.h>

namespace el {

	class GraphicsDeviceContext
	{
	public:

		GraphicsDeviceContext();
		virtual ~GraphicsDeviceContext();

        virtual void setViewport(int32_t x, int32_t y, uint32_t width, uint32_t height) = 0;
        virtual void setProgram(const GraphicsProgramPtr& program) = 0;
        virtual void setTexture(const std::string& name, const GraphicsTexturePtr& texture) = 0;

	private:

		GraphicsDeviceContext(const GraphicsDeviceContext&) = delete;
		GraphicsDeviceContext& operator=(const GraphicsDeviceContext&) = delete;
	};

} // namespace el {
