#pragma once

// #include "graphics_types.h"
// #include "gl_types.h"

#include <memory>

namespace el {
	typedef std::shared_ptr<class GraphicsContext> GraphicsContextPtr;

	class GraphicsConext
	{
	public:

		GraphicsConext() = default;
		virtual ~GraphicsConext() = default;
	};

	class GLContxt final : public GraphicsContext
	{
	public:

		GLContext() = default;
	};
} // namespace el { 
