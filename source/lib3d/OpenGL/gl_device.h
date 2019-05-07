#ifndef __GL_DEVICE_H__
#define __GL_DEVICE_H__

#include <graphics_device.h>

namespace el {

	class GLDevice final : public GraphicsDevice
	{
	public:

		bool create(const GraphicsDeviceDesc& desc) override;

		GraphicsProgramPtr createProgram(const GraphicsProgramDesc& desc) override;
		GraphicsShaderPtr createShader(const GraphicsShaderDesc& desc) override;
		GraphicsTexturePtr createTexture(const GraphicsTextureDesc& desc) override;
		GraphicsDataPtr createBuffer(const GraphicsDataDesc& desc) override;
        GraphicsInputLayoutPtr createInputLayout(const GraphicsInputLayoutDesc& desc) override;
        GraphicsContextPtr createContext() override;
	};

} // namespace el {

#endif // __GL_DEVICE_H__
