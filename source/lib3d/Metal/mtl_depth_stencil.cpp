#include "mtl_depth_stencil.h"

#if EL_BUILD_METAL

#include <el_debug.h>
#include "mtl_types.h"
#include "mtl_device.h"

using namespace el;

namespace el {

    mtlpp::StencilDescriptor asStencilDescriptor(const GraphicsStencilOpState& state)
    {
        mtlpp::StencilDescriptor desc;
        desc.SetStencilCompareFunction(asCompareFunction(state.getCompareOp()));
        desc.SetStencilFailureOperation(asStencilOperation(state.getFailOp()));
        desc.SetDepthFailureOperation(asStencilOperation(state.getDepthFailOp()));
        desc.SetDepthStencilPassOperation(asStencilOperation(state.getPassOp()));
        desc.SetReadMask(state.getCompareMask());
        desc.SetWriteMask(state.getWriteMask());
        return desc;
    }
}

MTLDepthStencil::MTLDepthStencil()
{
}

MTLDepthStencil::~MTLDepthStencil()
{
}

bool MTLDepthStencil::create(const GraphicsDepthStencilDesc& desc)
{
	auto device = _device.lock();
	if (!device) return false;

    mtlpp::Device& mtlDevice = device->getDevice();

    DepthStencilDescriptor descriptor;
    descriptor.SetDepthCompareFunction(asCompareFunction(desc.getDepthCompareOp()));
    descriptor.SetDepthWriteEnabled(desc.getDepthWriteEnable());
    if (desc.getStencilTestEnable())
    {
        descriptor.SetFrontFaceStencil(asStencilDescriptor(desc.getFrontFaceStencil()));
        descriptor.SetBackFaceStencil(asStencilDescriptor(desc.getBackFaceStencil()));
    }
    _metalDepthStencilState = mtlDevice.NewDepthStencilState(descriptor);

    if (!_metalDepthStencilState)
        return false;
    return true;
}

void MTLDepthStencil::destroy()
{
    _metalDepthStencilState = ns::Handle{};
}

void MTLDepthStencil::setDevice(GraphicsDevicePtr device)
{
    _device = std::static_pointer_cast<MTLDevice>(device);
}

GraphicsDevicePtr MTLDepthStencil::getDevice()
{
    return _device.lock();
}

const GraphicsDepthStencilDesc& MTLDepthStencil::getDepthStencilDesc() const
{
    return _depthStencilDesc;
}

const mtlpp::DepthStencilState& MTLDepthStencil::getMetalDepthStencilState() const
{
    return _metalDepthStencilState;
}


#endif // EL_BUILD_METAL
