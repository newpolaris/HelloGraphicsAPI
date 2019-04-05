#pragma once

#include "predefine.h"
#include <cstdint>

namespace el {
	class GraphicsContext 
	{
	public:

		GraphicsContext();
		~GraphicsContext();

		GraphicsContext(const GraphicsContext&) = delete;
		GraphicsContext& operator=(const GraphicsContext&) = delete;

	private:
	};
} // namespace el {
