#ifndef __GRAPHICS_DEVICE_H__
#define __GRAPHICS_DEVICE_H__

#include <graphics_types.h>

namespace el {
	class GraphicsDeviceDesc final
	{
	public:

		GraphicsDeviceDesc();

		void setType(GraphicsDeviceType type);
		GraphicsDeviceType getType() const;

	private:

		GraphicsDeviceType _type;
	};

    class GraphicsDevice : public std::enable_shared_from_this<GraphicsDevice>
	{
	public:

		GraphicsDevice();
		virtual ~GraphicsDevice();

		virtual bool create(const GraphicsDeviceDesc& desc) = 0;

		virtual GraphicsProgramPtr createProgram(const GraphicsProgramDesc& desc) = 0;
		virtual GraphicsShaderPtr createShader(const GraphicsShaderDesc& desc) = 0;
		virtual GraphicsTexturePtr createTexture(const GraphicsTextureDesc& desc) = 0;
		virtual GraphicsDataPtr createBuffer(const GraphicsDataDesc& desc) = 0;
        virtual GraphicsInputLayoutPtr createInputLayout(const GraphicsInputLayoutDesc& desc) = 0;
        virtual GraphicsContextPtr createContext() = 0;
	};

	GraphicsDevicePtr createDevice(const GraphicsDeviceDesc& desc);

} // namespace el {

#endif // __GRAPHICS_DEVICE_H__
