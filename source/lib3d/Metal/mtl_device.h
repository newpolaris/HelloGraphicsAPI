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

		bool create(const GraphicsDeviceDesc& desc) override;

		GraphicsProgramPtr createProgram(const GraphicsProgramDesc& desc) override;
		GraphicsShaderPtr createShader(const GraphicsShaderDesc& desc) override;
		GraphicsTexturePtr createTexture(const GraphicsTextureDesc& desc) override;
		GraphicsDataPtr createBuffer(const GraphicsDataDesc& desc) override;
        GraphicsInputLayoutPtr createInputLayout(const GraphicsInputLayoutDesc& desc) override;
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
