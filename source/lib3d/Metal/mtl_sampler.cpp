#include "mtl_sampler.h"

#if EL_BUILD_METAL

#include <el_debug.h>
#include <Metal/mtl_types.h>

using namespace el;
using namespace mtlpp;

MTLSampler::MTLSampler()
{
}

MTLSampler::~MTLSampler()
{
    destroy();
}

/*
    Default values for metal

    MTLSamplerMinMagFilterNearest.
    MTLSamplerMinMagFilterNearest.
    MTLSamplerMipFilterNotMipmapped.
    MTLSamplerAddressModeClampToEdge.
    MTLSamplerAddressModeClampToEdge.
    MTLSamplerAddressModeClampToEdge.
 */
bool MTLSampler::create(const GraphicsSamplerDesc& desc)
{
    mtlpp::Device device;

    mtlpp::SamplerDescriptor descriptor;
    descriptor.SetMinFilter(asSamplerMinMagFilter(desc.getMinFilter()));
    descriptor.SetMagFilter(asSamplerMinMagFilter(desc.getMagFilter()));
    descriptor.SetMipFilter(asSamplerMipFilter(desc.getSamplerMipmapMode()));
    descriptor.SetMaxAnisotropy((uint32_t)desc.getAnisotropyLevel());
    descriptor.SetSAddressMode(asSamplerAddressMode(desc.getAddressModeU()));
    descriptor.SetTAddressMode(asSamplerAddressMode(desc.getAddressModeV()));
    descriptor.SetRAddressMode(asSamplerAddressMode(desc.getAddressModeW()));

    // TODO:
    // SetBorderColor(SamplerBorderColor borderColor);
    // SetNormalizedCoordinates(bool normalizedCoordinates);
    // SetLodMinClamp(float lodMinClamp);
    // SetLodMaxClamp(float lodMaxClamp);
    // SetCompareFunction(CompareFunction compareFunction);
    // SetLabel(const ns::String& label);

    device.NewSamplerState(descriptor);

    _samplerDesc = desc;

    return true;
}

void MTLSampler::destroy()
{
    _sampler.reset();
}

const GraphicsSamplerDesc& MTLSampler::getSamplerDesc() const
{
    return _samplerDesc;
}

#endif // EL_BUILD_METAL
