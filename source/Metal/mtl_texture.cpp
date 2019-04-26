#include "mtl_texture.h"

#if EL_PLAT_APPLE

#include "debug.h"
#include "mtl_device.h"

using namespace el;

MTLTexture::MTLTexture()
{
}

MTLTexture::~MTLTexture()
{
}

bool MTLTexture::create(GraphicsTextureDesc desc)
{
    auto device = _device.lock();
    if (!device) return false;

	// TODO:
    auto format = mtlpp::PixelFormat::RGBA8Unorm;

    auto textureDesc = mtlpp::TextureDescriptor::Texture2DDescriptor(
        format, desc.getWidth(), desc.getHeight(), false);

	// TODO:
    textureDesc.SetUsage(mtlpp::TextureUsage::RenderTarget);

    _texture = device->getDevice().NewTexture(textureDesc);
    if (!_texture)
        return false;

	// TODO:
	uint32_t bytesPerRow = 16;

    mtlpp::Region region = { 0, 0, textureDesc.GetWidth(), textureDesc.GetHeight() };
    _texture.Replace(region, 0, desc.getStream(), bytesPerRow);

	_textureDesc = std::move(desc);

	return true;
}

void MTLTexture::destroy()
{
}

void MTLTexture::setDevice(GraphicsDevicePtr device)
{
    _device = std::static_pointer_cast<MTLDevice>(std::move(device));
}

GraphicsDevicePtr MTLTexture::getDevice()
{
    return _device.lock();
}

const GraphicsTextureDesc& MTLTexture::getTextureDesc() const
{
	return _textureDesc;
}

#endif // EL_PLAT_APPLE
