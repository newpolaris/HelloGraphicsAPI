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

	class GraphicsDevice
	{
	public:

		GraphicsDevice();
		virtual ~GraphicsDevice();

		virtual bool create(GraphicsDeviceDesc desc) = 0;

		virtual GraphicsProgramPtr createProgram(GraphicsProgramDesc desc) = 0;
		virtual GraphicsShaderPtr createShader(GraphicsShaderDesc desc) = 0;
		virtual GraphicsTexturePtr createTexture(GraphicsTextureDesc desc) = 0;
		virtual GraphicsBufferPtr createBuffer(GraphicsBufferDesc desc) = 0;
	};

	GraphicsDevicePtr createDevice(GraphicsDeviceDesc desc);

} // namespace el {
