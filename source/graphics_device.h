#pragma once

#include "graphics_types.h"

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

		virtual bool create(GraphicsDeviceDesc desc) = 0;

		virtual GraphicsProgramPtr createProgram(GraphicsProgramDesc desc) = 0;
		virtual GraphicsShaderPtr createShader(GraphicsShaderDesc desc) = 0;
		virtual GraphicsTexturePtr createTexture(GraphicsTextureDesc desc) = 0;
		virtual GraphicsDataPtr createBuffer(GraphicsDataDesc desc) = 0;
        virtual GraphicsInputLayoutPtr createInputLayout(GraphicsInputLayoutDesc desc) = 0;
        virtual GraphicsContextPtr createDeviceContext() = 0;
	};

	GraphicsDevicePtr createDevice(GraphicsDeviceDesc desc);

} // namespace el {
