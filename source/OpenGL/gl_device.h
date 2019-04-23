#pragma once

#include "graphics_device.h"

namespace el {

	class GLDevice final : public GraphicsDevice
	{
	public:

		bool create(GraphicsDeviceDesc desc) override;

		GraphicsProgramPtr createProgram(GraphicsProgramDesc desc) override;
		GraphicsShaderPtr createShader(GraphicsShaderDesc desc) override;
		GraphicsTexturePtr createTexture(GraphicsTextureDesc desc) override;
		GraphicsDataPtr createBuffer(GraphicsDataDesc desc) override;
        GraphicsContextPtr createDeviceContext() override;
	};

} // namespace el {
