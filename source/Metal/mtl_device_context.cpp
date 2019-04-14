#include "mtl_device_context.h"

#if EL_PLAT_APPLE

#include "debug.h"
#include "mtl_device.h"
#include "mtl_device_context.h"

using namespace el;

MTLDeviceContext::MTLDeviceContext()
{
}

MTLDeviceContext::~MTLDeviceContext()
{
}

bool MTLDeviceContext::create()
{
    auto device = _device.lock();
    if (!device) return false;

    _commandBuffer = device->getCommandQueue().CommandBuffer();
    if (!_commandBuffer) return false;

	return true;
}

void MTLDeviceContext::destroy()
{
}

void MTLDeviceContext::finsh(bool waitForCompletion)
{
    EL_ASSERT(_commandBuffer);

    _commandBuffer.Commit();
    if (waitForCompletion)
    {
        _commandBuffer.WaitUntilCompleted();
    }
}

void MTLDeviceContext::setDevice(GraphicsDevicePtr device)
{
    _device = std::static_pointer_cast<MTLDevice>(std::move(device));
}

GraphicsDevicePtr MTLDeviceContext::getDevice()
{
    return _device.lock();
}

#endif // EL_PLAT_APPLE
