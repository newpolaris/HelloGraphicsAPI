#ifndef __MTL_DEVICE_H__
#define __MTL_DEVICE_H__

#include <el_predefine.h>

#if EL_BUILD_METAL

#include <graphics_device.h>
#include <mtlpp.hpp>

namespace el {

	class MTLDevice final : public GraphicsDevice
	{
	public:

		bool create(GraphicsDeviceDesc desc) override;

		GraphicsProgramPtr createProgram(GraphicsProgramDesc desc) override;
		GraphicsShaderPtr createShader(GraphicsShaderDesc desc) override;
		GraphicsTexturePtr createTexture(GraphicsTextureDesc desc) override;
		GraphicsDataPtr createBuffer(GraphicsDataDesc desc) override;
        GraphicsInputLayoutPtr createInputLayout(GraphicsInputLayoutDesc desc) override;
        GraphicsContextPtr createContext() override;

        mtlpp::Device& getDevice();
        mtlpp::CommandQueue& getCommandQueue();
        
    private:

        mtlpp::Device _device;
        mtlpp::CommandQueue _commandQueue;
	};

} // namespace el {

#endif // EL_BUILD_METAL

#endif // __MTL_DEVICE_H__
