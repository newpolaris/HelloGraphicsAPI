#include "mtl_texture.h"

#if EL_BUILD_METAL

#include <el_debug.h>
#include "mtl_device.h"
#include "mtl_types.h"

using namespace el;

MTLTexture::MTLTexture()
{
}

MTLTexture::~MTLTexture()
{
}

bool MTLTexture::create(const GraphicsTextureDesc& desc)
{
    auto device = _device.lock();
    if (!device)
        return false;

    if (desc.getHeight() == 0 || desc.getWidth() == 0)
        return false;

    auto format = asPixelFormat(desc.getPixelFormat());
    auto textureDesc = mtlpp::TextureDescriptor::Texture2DDescriptor(
        format, desc.getWidth(), desc.getHeight(), false);

    textureDesc.SetUsage(asTextureUsage(desc.getTextureUsage()));
    textureDesc.SetResourceOptions(asTextureResourceOptions(desc.getTextureUsage()));
    textureDesc.SetCpuCacheMode(mtlpp::CpuCacheMode::DefaultCache);

    _texture = device->getDevice().NewTexture(textureDesc);
    if (!_texture)
        return false;

    if (desc.getStream())
    {
        // calc. bytesPerRow with alignment fixing
        const int alignment = (int)desc.getPixelAlignment();
        uint32_t bytesPerRow = desc.getWidth() * asTexelSize(desc.getPixelFormat());
        bytesPerRow = ((bytesPerRow + alignment - 1) / alignment) * alignment;

        if (desc.getStreamSize() > 0) {
            EL_ASSERT(bytesPerRow == desc.getStreamSize() / desc.getHeight());
        }

        auto region = mtlpp::Region( 0, 0, textureDesc.GetWidth(), textureDesc.GetHeight() );
        _texture.Replace(region, 0, desc.getStream(), bytesPerRow);
    }
	_textureDesc = std::move(desc);

	return true;
}

void MTLTexture::destroy()
{
    _texture = ns::Handle{};
}

void MTLTexture::setDevice(const GraphicsDevicePtr& device)
{
    _device = std::static_pointer_cast<MTLDevice>(device);
}

GraphicsDevicePtr MTLTexture::getDevice()
{
    return _device.lock();
}

const mtlpp::Texture& MTLTexture::getTexture() const
{
    return _texture;
}

const GraphicsTextureDesc& MTLTexture::getTextureDesc() const
{
	return _textureDesc;
}

#endif // EL_BUILD_METAL
