#include "mtl_input_layout.h"

#if EL_BUILD_METAL

#include "mtl_device.h"

using namespace el;

MTLInputLayout::MTLInputLayout()
{
}

MTLInputLayout::~MTLInputLayout()
{
}

bool MTLInputLayout::create(const GraphicsInputLayoutDesc& desc)
{
    auto layout = _inputLayout.GetLayouts();
    for (uint32_t i = 0; desc.getBindings().size(); i++)
    {
        auto& source = desc.getBindings();
        layout[i].SetStepFunction(asVertexStepFunction(source[i].getInputRate()));
        layout[i].SetStepRate(1);
        layout[i].SetStride(source[i].getStride());
    }

    auto attributes = _inputLayout.GetAttributes();
    for (uint32_t i = 0; desc.getAttributes().size(); i++)
    {
        auto& source = desc.getAttributes();
        attributes[i].SetBufferIndex(source[i].getBinding());
        attributes[i].SetFormat(asVertexFormat(source[i].getFormat()));
        attributes[i].SetOffset(source[i].getOffset());
    }

    _desc = std::move(desc);

    return true;
}

void MTLInputLayout::destroy()
{
    _inputLayout = ns::Handle{};
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

#endif // EL_BUILD_METAL
