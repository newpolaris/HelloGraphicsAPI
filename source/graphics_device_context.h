#pragma once

#include "predefine.h"
#include <graphics_types.h>

namespace el {

	class GraphicsDeviceContext
	{
	public:

		GraphicsDeviceContext();
		virtual ~GraphicsDeviceContext();

	private:

		GraphicsDeviceContext(const GraphicsDeviceContext&) = delete;
		GraphicsDeviceContext& operator=(const GraphicsDeviceContext&) = delete;
	};

} // namespace el {
