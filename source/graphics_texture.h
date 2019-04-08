#pragma once

#include "graphics_types.h"

namespace el {
	class GraphicsTextureDesc final
	{
	public:

		GraphicsTextureDesc();

		void setWidth(uint32_t width);
		uint32_t getWidth() const;

		void setHeight(uint32_t height);
		uint32_t getHeight() const;

		void setDepth(uint32_t depth);
		uint32_t getDepth() const;

		void setDim(GraphicsTextureDim target);
		GraphicsTextureDim getDim() const;

	private:

		uint32_t _width;
		uint32_t _height;
		uint32_t _depth;
		GraphicsTextureDim _dim;
	};

	class GraphicsTexture
	{
	public:

		GraphicsTexture();
		virtual ~GraphicsTexture();

		virtual const GraphicsTextureDesc& getTextureDesc() const = 0;

	private:

		GraphicsTexture(const GraphicsTexture&) = delete;
		GraphicsTexture& operator=(const GraphicsTexture&) = delete;
	};

} // namespace el {
