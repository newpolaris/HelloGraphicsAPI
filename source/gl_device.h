#pragma once

#include "graphics_device.h"

namespace el {

	class GLDevice final : public GraphicsDevice
	{
	public:

		bool create(const GraphicsDeviceDesc &desc) override;

		GraphicsProgramPtr createProgram(const GraphicsProgramDesc& desc) override;
		GraphicsShaderPtr createShader(const GraphicsShaderDesc& desc) override;
		GraphicsTexturePtr createTexture(const GraphicsTextureDesc& desc) override;
		GraphicsBufferPtr createBuffer(const GraphicsBufferDesc& desc) override;
	};

} // namespace el {
