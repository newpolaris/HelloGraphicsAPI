#include "graphics_input_layout.h"

using namespace el;

GraphicsVertexInputBindingDesc::GraphicsVertexInputBindingDesc() :
    _binding(0u),
    _stride(0u),
    _inputRate(GraphicsVertexInputRate::GraphicsVertexInputRateVertex)
{
}

void GraphicsVertexInputBindingDesc::setBinding(uint32_t binding)
{
    _binding = binding;
}

uint32_t GraphicsVertexInputBindingDesc::getBinding() const
{
    return _binding;
}

void GraphicsVertexInputBindingDesc::setStride(uint32_t stride)
{
    _stride = stride;
}

uint32_t GraphicsVertexInputBindingDesc::getStride() const
{
    return _stride;
}

GraphicsInputLayoutDesc::GraphicsInputLayoutDesc()
{
}

void GraphicsInputLayoutDesc::setAttributes(GraphicsVertexInputAttributeDescs descs)
{
    _attributes = std::move(descs);
}

const GraphicsVertexInputAttributeDescs& GraphicsInputLayoutDesc::getAttributes() const
{
    return _attributes;
}

void GraphicsInputLayoutDesc::setBindings(GraphicsVertexInputBindingDescs descs)
{
    _bindings = std::move(descs);
}

const GraphicsVertexInputBindingDescs& GraphicsInputLayoutDesc::getBindings() const
{
    return _bindings;
}

el::GraphicsInputLayout::GraphicsInputLayout()
{
}

el::GraphicsInputLayout::~GraphicsInputLayout()
{
}

bool GraphicsInputLayout::create(GraphicsInputLayoutDesc desc)
{
    _desc = std::move(desc);

    return false;
}

void GraphicsInputLayout::destory()
{
}

const GraphicsInputLayoutDesc& el::GraphicsInputLayout::getDesc()
{
    return _desc;
}
