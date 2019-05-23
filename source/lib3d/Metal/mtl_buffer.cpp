#include <Metal/mtl_buffer.h>

#if EL_BUILD_METAL

#include <el_debug.h>
#include "mtl_device.h"

using namespace el;

MTLBuffer::MTLBuffer()
{
}

MTLBuffer::~MTLBuffer()
{
}

mtlpp::ResourceOptions asResourceOptions(GraphicsUsageFlags flags)
{
    auto options = mtlpp::ResourceOptions::CpuCacheModeWriteCombined;
    if (flags & GraphicsUsageFlagReadBit)
        options = mtlpp::ResourceOptions::CpuCacheModeDefaultCache;
    return options;
}

bool MTLBuffer::create(GraphicsDataDesc desc)
{
    auto device = _device.lock();
    if (!device) return false;

    auto resourceOptions = asResourceOptions(desc.getUsage());
    _buffer = device->getMetalDevice().NewBuffer(desc.getStream(),
                                             desc.getStreamSize(),
                                             resourceOptions);

    _desc = std::move(desc);

    return true;
}

void MTLBuffer::destroy()
{
}

void MTLBuffer::bind() const
{
}

const GraphicsDataDesc& MTLBuffer::getDesc() const
{
    return _desc;
}

void MTLBuffer::setDevice(GraphicsDevicePtr device)
{
    _device = std::static_pointer_cast<MTLDevice>(std::move(device));
}

GraphicsDevicePtr MTLBuffer::getDevice()
{
    return _device.lock();
}

#endif // #if EL_BUILD_METAL
