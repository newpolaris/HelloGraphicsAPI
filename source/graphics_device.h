#pragma once

#include "graphics_types.h"

namespace el 
{
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

		virtual bool create(const GraphicsDeviceDesc& desc) = 0;

		virtual GraphicsProgramPtr createProgram(const GraphicsProgramDesc& desc) = 0;
		virtual GraphicsShaderPtr createShader(const GraphicsShaderDesc& desc) = 0;
	};

	GraphicsDevicePtr createDevice(const GraphicsDeviceDesc& desc);

} // namespace el 
