#ifndef __GL_DEVICE_H__
#define __GL_DEVICE_H__

#include <graphics_device.h>

namespace el {

	class GLDevice final : public GraphicsDevice
	{
	public:

		bool create(GraphicsDeviceDesc desc) override;

		GraphicsProgramPtr createProgram(GraphicsProgramDesc desc) override;
		GraphicsShaderPtr createShader(GraphicsShaderDesc desc) override;
		GraphicsTexturePtr createTexture(GraphicsTextureDesc desc) override;
		GraphicsDataPtr createBuffer(GraphicsDataDesc desc) override;
        GraphicsInputLayoutPtr createInputLayout(GraphicsInputLayoutDesc desc) override;
        GraphicsContextPtr createContext() override;
	};

} // namespace el {

#endif // __GL_DEVICE_H__
