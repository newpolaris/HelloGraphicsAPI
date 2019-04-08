#include "graphics_texture.h"

using namespace el;

GraphicsTextureDesc::GraphicsTextureDesc() :
	_width(0),
	_height(0),
	_depth(1),
	_dim(GraphicsTextureDim2D)
{
}

void GraphicsTextureDesc::setWidth(uint32_t width)
{
	_width = width;
}

uint32_t GraphicsTextureDesc::getWidth() const
{
	return _width;
}

void GraphicsTextureDesc::setHeight(uint32_t height)
{
	_height = height;
}

uint32_t GraphicsTextureDesc::getHeight() const
{
	return _height;
}

void GraphicsTextureDesc::setDepth(uint32_t depth)
{
	_depth = depth;
}

uint32_t GraphicsTextureDesc::getDepth() const
{
	return _depth;
}

void GraphicsTextureDesc::setDim(GraphicsTextureDim dim)
{
	_dim = dim;
}

GraphicsTextureDim GraphicsTextureDesc::getDim() const
{
	return _dim;
}

GraphicsTexture::GraphicsTexture()
{
}

GraphicsTexture::~GraphicsTexture()
{
}
