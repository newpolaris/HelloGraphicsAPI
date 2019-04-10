#pragma once

#include "predefine.h"

#if EL_PLAT_APPLE

#include "graphics_device.h"
#include "mtlpp.hpp"

namespace el {

	class MTLDevice final : public GraphicsDevice
	{
	public:

		bool create(GraphicsDeviceDesc desc) override;

		GraphicsProgramPtr createProgram(GraphicsProgramDesc desc) override;
		GraphicsShaderPtr createShader(GraphicsShaderDesc desc) override;
		GraphicsTexturePtr createTexture(GraphicsTextureDesc desc) override;
		GraphicsBufferPtr createBuffer(GraphicsBufferDesc desc) override;
        GraphicsDeviceContextPtr createDeviceContext() override;

        mtlpp::Device& getDevice();
        mtlpp::CommandQueue& getCommandQueue();
        
    private:

        mtlpp::Device _device;
        mtlpp::CommandQueue _commandQueue;
	};

} // namespace el {

#endif // EL_PLAT_APPLE
