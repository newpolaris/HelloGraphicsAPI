#include "graphics_texture.h"

using namespace el;

GraphicsTextureDesc::GraphicsTextureDesc() :
	_width(0),
	_height(0),
	_depth(1),
    _levels(1),
    _addressModeU(GraphicsSamplerAddressMode::GraphicsSamplerAddressModeRepeat),
    _addressModeV(GraphicsSamplerAddressMode::GraphicsSamplerAddressModeRepeat),
    _addressModeW(GraphicsSamplerAddressMode::GraphicsSamplerAddressModeRepeat),
    _minFilter(GraphicsFilterLinear),
    _magFilter(GraphicsFilterLinear),
    _samplerMipmapMode(GraphicsSamplerMipmapModeNone),
    _anisotropyLevel(0),
	_stream(nullptr),
	_streamSize(0),
	_dim(GraphicsTextureDim2D),
	_pixelFormat(GraphicsPixelFormat::GraphicsPixelFormatInvalid),
    _pixelAlignment(GraphicsPixelAlignment::GraphicsPixelAlignment4),
    _textureUsage(0)
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

void GraphicsTextureDesc::setLevels(uint32_t levels)
{
    _levels = levels;
}

uint32_t GraphicsTextureDesc::getLevels() const
{
    return _levels;
}

GraphicsSamplerAddressMode GraphicsTextureDesc::getAddressModeU() const
{
    return _addressModeU;
}

void GraphicsTextureDesc::setAddressModeU(GraphicsSamplerAddressMode mode)
{
    _addressModeU = mode;
}

GraphicsSamplerAddressMode GraphicsTextureDesc::getAddresModeV() const
{
    return _addressModeV;
}

void GraphicsTextureDesc::setAddressModeV(GraphicsSamplerAddressMode mode)
{
    _addressModeV = mode;
}

GraphicsSamplerAddressMode GraphicsTextureDesc::getAddresModeW() const
{
    return _addressModeW;
}

void GraphicsTextureDesc::setAddressModeW(GraphicsSamplerAddressMode mode)
{
    _addressModeW = mode;
}

GraphicsFilter GraphicsTextureDesc::getMinFilter() const
{
    return _minFilter;
}

void GraphicsTextureDesc::setMinFilter(GraphicsFilter filter)
{
    _minFilter = filter;
}

GraphicsFilter GraphicsTextureDesc::getMagFilter() const
{
    return _magFilter;
}

void GraphicsTextureDesc::setMagFilter(GraphicsFilter filter)
{
    _magFilter = filter;
}

GraphicsSamplerMipmapMode GraphicsTextureDesc::getSamplerMipmapMode() const
{
    return _samplerMipmapMode;
}

void GraphicsTextureDesc::setSamplerMipmapMode(GraphicsSamplerMipmapMode mode)
{
    _samplerMipmapMode = mode;
}

float GraphicsTextureDesc::getAnisotropyLevel() const
{
    return _anisotropyLevel;
}

void GraphicsTextureDesc::setAnisotropyLevel(float anisoLevel)
{
    _anisotropyLevel = anisoLevel;
}


void GraphicsTextureDesc::setStream(stream_t* stream)
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

void GraphicsTextureDesc::setDim(GraphicsTextureDim dim)
{
	_dim = dim;
}

GraphicsTextureDim GraphicsTextureDesc::getDim() const
{
	return _dim;
}

void GraphicsTextureDesc::setPixelFormat(GraphicsPixelFormat format)
{
	_pixelFormat = format;
}

GraphicsPixelFormat GraphicsTextureDesc::getPixelFormat() const
{
	return _pixelFormat;
}

void GraphicsTextureDesc::setPixelAlignment(GraphicsPixelAlignment align)
{
    _pixelAlignment = align;
}

GraphicsPixelAlignment GraphicsTextureDesc::getPixelAlignment() const
{
    return _pixelAlignment;
}

void GraphicsTextureDesc::setTextureUsage(GraphicsTextureUsageFlags usage)
{
    _textureUsage = usage;
}

GraphicsTextureUsageFlags GraphicsTextureDesc::getTextureUsage() const
{
    return _textureUsage;
}

GraphicsTexture::GraphicsTexture()
{
}

GraphicsTexture::~GraphicsTexture()
{
}
