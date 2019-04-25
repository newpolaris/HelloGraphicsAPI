#include "mtl_input_layout.h"
#include "mtl_device.h"

using namespace el;

MTLInputLayout::MTLInputLayout()
{
}

MTLInputLayout::~MTLInputLayout()
{
}

bool MTLInputLayout::create(GraphicsInputLayoutDesc desc)
{
    _desc = std::move(desc);

    return true;
}

void MTLInputLayout::destory()
{
}

void MTLInputLayout::setDevice(GraphicsDevicePtr device)
{
    _device = std::static_pointer_cast<MTLDevice>(std::move(device));
}

GraphicsDevicePtr MTLInputLayout::getDevice()
{
    return _device.lock();
}

const GraphicsInputLayoutDesc& MTLInputLayout::getDesc() const
{
    return _desc;
}
