#include "mtl_depth_stencil.h"

#if EL_BUILD_METAL

#include "mtl_types.h"
#include <el_debug.h>

using namespace el;
using namespace mtlpp;

mtlpp::StencilDescriptor el::asStencilDescriptor(const GraphicsStencilOpState& state)
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

MTLDepthStencil::MTLDepthStencil()
{
}

MTLDepthStencil::~MTLDepthStencil()
{
}

bool MTLDepthStencil::create(const GraphicsDepthStencilDesc& desc)
{
    mtlpp::Device device;

    DepthStencilDescriptor descriptor;
    descriptor.SetDepthCompareFunction(asCompareFunction(desc.getDepthCompareOp()));
    descriptor.SetDepthWriteEnabled(desc.getDepthWriteEnable());
    if (desc.getStencilTestEnable())
    {
        descriptor.SetFrontFaceStencil(asStencilDescriptor(desc.getFrontFaceStencil()));
        descriptor.SetBackFaceStencil(asStencilDescriptor(desc.getBackFaceStencil()));
    }
    _depthStencilState = device.NewDepthStencilState(descriptor);

    if (!_depthStencilState)
        return false;
    return true;
}

void MTLDepthStencil::destroy()
{
    _depthStencilState.reset();
}

void MTLDepthStencil::setDevice(GraphicsDevicePtr device)
{
}

GraphicsDevicePtr MTLDepthStencil::getDevice()
{
    return GraphicsDevicePtr();
}

const GraphicsDepthStencilDesc& MTLDepthStencil::getDepthStencilDesc() const
{
    return _depthStencilDesc;
}

#endif // EL_BUILD_METAL
