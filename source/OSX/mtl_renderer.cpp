#include "mtl_renderer.h"

#include <debug.h>
#include <graphics_types.h>
#include <graphics_device.h>

namespace mtl_renderer
{
    el::GraphicsDevicePtr device;
}

void mtl_renderer::create()
{
    el::GraphicsDeviceDesc deviceDesc;
    deviceDesc.setType(el::GraphicsDeviceTypeMetal);

    device = el::createDevice(deviceDesc);
    EL_ASSERT(device != nullptr);
}
