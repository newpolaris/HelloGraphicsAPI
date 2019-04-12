#include "graphics_texture.h"
#include <istream>

using namespace el;

GraphicsTextureDesc::GraphicsTextureDesc() :
	_width(0),
	_height(0),
	_depth(1),
	_stream(nullptr),
	_streamSize(0),
	_dim(GraphicsTextureDim2D),
	_pixelFormat(GraphicsPixelFormat::GraphicsPixelFormatInvalid)
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

void GraphicsTextureDesc::setStream(stream_t * stream)
{
	_stream = stream;
}

const char* GraphicsTextureDesc::getStream() const
{
	return _stream;
}

void GraphicsTextureDesc::setStreamSize(streamsize_t streamSize)
{
	_streamSize = streamSize;
}

streamsize_t GraphicsTextureDesc::getStreamSize() const
{
	return _streamSize;
}

void GraphicsTextureDesc::setPixelFormat(GraphicsPixelFormat format)
{
	_pixelFormat = format;
}

GraphicsPixelFormat GraphicsTextureDesc::getPixelFormat() const
{
	return _pixelFormat;
}

GraphicsTexture::GraphicsTexture()
{
}

GraphicsTexture::~GraphicsTexture()
{
}
