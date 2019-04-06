#pragma once

#include "predefine.h"
#include <cstdint>

namespace el {
	class GraphicsContext 
	{
	public:

		GraphicsContext();
		~GraphicsContext();

	private:

		GraphicsContext(const GraphicsContext&) = delete;
		GraphicsContext& operator=(const GraphicsContext&) = delete;
	};
} // namespace el {
