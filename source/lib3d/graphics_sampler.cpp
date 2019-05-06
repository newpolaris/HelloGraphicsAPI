#include <graphics_sampler.h>

using namespace el;

GraphicsSamplerDesc::GraphicsSamplerDesc() :
    _addressModeU(GraphicsSamplerAddressMode::GraphicsSamplerAddressModeRepeat),
    _addressModeV(GraphicsSamplerAddressMode::GraphicsSamplerAddressModeRepeat),
    _addressModeW(GraphicsSamplerAddressMode::GraphicsSamplerAddressModeRepeat),
    _minFilter(GraphicsFilterLinear),
    _magFilter(GraphicsFilterLinear),
    _samplerMipmapMode(GraphicsSamplerMipmapModeNone),
    _anisotropyLevel(0)
{
}

GraphicsSamplerAddressMode GraphicsSamplerDesc::getAddressModeU() const
{
    return _addressModeU;
}

void GraphicsSamplerDesc::setAddressModeU(GraphicsSamplerAddressMode mode)
{
    _addressModeU = mode;
}

GraphicsSamplerAddressMode GraphicsSamplerDesc::getAddressModeV() const
{
    return _addressModeV;
}

void GraphicsSamplerDesc::setAddressModeV(GraphicsSamplerAddressMode mode)
{
    _addressModeV = mode;
}

GraphicsSamplerAddressMode GraphicsSamplerDesc::getAddressModeW() const
{
    return _addressModeW;
}

void GraphicsSamplerDesc::setAddressModeW(GraphicsSamplerAddressMode mode)
{
    _addressModeW = mode;
}

GraphicsFilter GraphicsSamplerDesc::getMinFilter() const
{
    return _minFilter;
}

void GraphicsSamplerDesc::setMinFilter(GraphicsFilter filter)
{
    _minFilter = filter;
}

GraphicsFilter GraphicsSamplerDesc::getMagFilter() const
{
    return _magFilter;
}

void GraphicsSamplerDesc::setMagFilter(GraphicsFilter filter)
{
    _magFilter = filter;
}

GraphicsSamplerMipmapMode GraphicsSamplerDesc::getSamplerMipmapMode() const
{
    return _samplerMipmapMode;
}

void GraphicsSamplerDesc::setSamplerMipmapMode(GraphicsSamplerMipmapMode mode)
{
    _samplerMipmapMode = mode;
}

float GraphicsSamplerDesc::getAnisotropyLevel() const
{
    return _anisotropyLevel;
}

void GraphicsSamplerDesc::setAnisotropyLevel(float anisoLevel)
{
    _anisotropyLevel = anisoLevel;
}

GraphicsSampler::GraphicsSampler()
{
}

GraphicsSampler::~GraphicsSampler()
{
}
