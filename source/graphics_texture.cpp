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

uint32_t GraphicsTextureDesc::getWrapS() const
{
    return _wrapS;
}

void GraphicsTextureDesc::setWrapS(uint32_t wrap)
{
    _wrapS = wrap;
}

uint32_t GraphicsTextureDesc::getWrapT() const
{
    return _wrapT;
}

void GraphicsTextureDesc::setWrapT(uint32_t wrap)
{
    _wrapT = wrap;
}

uint32_t GraphicsTextureDesc::getWrapR() const
{
    return _wrapT;
}

void GraphicsTextureDesc::setWrapR(uint32_t wrap)
{
    _wrapR = wrap;
}

uint32_t GraphicsTextureDesc::getMinFilter() const
{
    return _minFilter;
}

void GraphicsTextureDesc::setMinFilter(uint32_t filter)
{
    _minFilter = filter;
}

uint32_t GraphicsTextureDesc::getMagFilter() const
{
    return _magFilter;
}

void GraphicsTextureDesc::setMagFilter(uint32_t filter)
{
    _magFilter = filter;
}

float GraphicsTextureDesc::getAnisotropyLevel() const
{
    return _anisotropyLevel;
}

void GraphicsTextureDesc::setAnisotropyLevel(float anisoLevel)
{
    _anisotropyLevel = anisoLevel;
}

GraphicsTexture::GraphicsTexture()
{
}

GraphicsTexture::~GraphicsTexture()
{
}
