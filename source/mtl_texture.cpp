#include "mtl_texture.h"

#if EL_PLAT_APPLE

#include "debug.h"
#include "mtl_device.h"

// TODO:
#include <GLFW/glfw3.h>

using namespace el;

MTLTexture::MTLTexture()
{
}

MTLTexture::~MTLTexture()
{
}

bool MTLTexture::create(GraphicsTextureDesc desc)
{
	auto device = std::static_pointer_cast<MTLDevice>(this->getDevice());

    auto format = mtlpp::PixelFormat::RGBA8Unorm;
    auto textureDesc = mtlpp::TextureDescriptor::Texture2DDescriptor(
        format, desc.getWidth(), desc.getHeight(), false);
    textureDesc.SetUsage(mtlpp::TextureUsage::RenderTarget);

    _texture = device->getDevice().NewTexture(textureDesc);
    if (!_texture)
        return false;

	_textureDesc = std::move(desc);

	return true;
}

void MTLTexture::destroy()
{
}

const GraphicsTextureDesc& MTLTexture::getTextureDesc() const
{
	return _textureDesc;
}

#endif // EL_PLAT_APPLE
