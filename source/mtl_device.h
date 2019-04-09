#pragma once

#include "graphics_device.h"

namespace el {

	class MTLDevice final : public GraphicsDevice
	{
	public:

		bool create(GraphicsDeviceDesc desc) override;

		GraphicsProgramPtr createProgram(GraphicsProgramDesc desc) override;
		GraphicsShaderPtr createShader(GraphicsShaderDesc desc) override;
		GraphicsTexturePtr createTexture(GraphicsTextureDesc desc) override;
		GraphicsBufferPtr createBuffer(GraphicsBufferDesc desc) override;
	};

} // namespace el {
