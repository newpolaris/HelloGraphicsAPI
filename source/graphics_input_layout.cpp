#include "graphics_input_layout.h"

using namespace el;

GraphicsInputBinding::GraphicsInputBinding() :
    _binding(0u),
    _stride(0u),
    _inputRate(GraphicsInputRateVertex)
{
}

GraphicsInputBinding::GraphicsInputBinding(uint32_t binding, uint32_t stride, GraphicsInputRate rate) :
    _binding(binding),
    _stride(stride),
    _inputRate(rate)
{
}

void GraphicsInputBinding::setBinding(uint32_t binding)
{
    _binding = binding;
}

uint32_t GraphicsInputBinding::getBinding() const
{
    return _binding;
}

void GraphicsInputBinding::setStride(uint32_t stride)
{
    _stride = stride;
}

uint32_t GraphicsInputBinding::getStride() const
{
    return _stride;
}

void GraphicsInputBinding::setInputRate(GraphicsInputRate rate)
{
    _inputRate = rate;
}

GraphicsInputRate GraphicsInputBinding::getInputRate() const
{
    return _inputRate;
}

GraphicsInputAttribute::GraphicsInputAttribute() :
    _binding(0u),
    _location(0u),
    _format(VertexFormat::Invalid),
    _offset(0u)
{
}

GraphicsInputAttribute::GraphicsInputAttribute(uint32_t binding, std::string name, uint32_t location, VertexFormat format, uint32_t offset) :
    _binding(binding),
    _name(std::move(name)),
    _location(location),
    _format(format),
    _offset(offset)
{
}

void GraphicsInputAttribute::setBinding(uint32_t binding)
{
    _binding = binding;
}

uint32_t GraphicsInputAttribute::getBinding() const
{
    return _binding;
}

void GraphicsInputAttribute::setLocation(uint32_t location)
{
    _location = location;
}

uint32_t GraphicsInputAttribute::getLocation() const
{
    return _location;
}

void GraphicsInputAttribute::setName(std::string name)
{
    _name = std::move(name);
}

const std::string& GraphicsInputAttribute::getName() const
{
    return _name;
}

void GraphicsInputAttribute::setFormat(VertexFormat format)
{
    _format = format;
}

VertexFormat el::GraphicsInputAttribute::getFormat() const
{
    return _format;
}

void GraphicsInputAttribute::setOffset(uint32_t offset)
{
    _offset = offset;
}

uint32_t GraphicsInputAttribute::getOffset() const
{
    return _offset;
}

GraphicsInputLayoutDesc::GraphicsInputLayoutDesc()
{
}

void GraphicsInputLayoutDesc::setAttributes(GraphicsInputAttributes attrib)
{
    _attributes = std::move(attrib);
}

const GraphicsInputAttributes& GraphicsInputLayoutDesc::getAttributes() const
{
    return _attributes;
}

void GraphicsInputLayoutDesc::setBindings(GraphicsInputBindings bindings)
{
    _bindings = std::move(bindings);
}

const GraphicsInputBindings& GraphicsInputLayoutDesc::getBindings() const
{
    return _bindings;
}

el::GraphicsInputLayout::GraphicsInputLayout()
{
}

el::GraphicsInputLayout::~GraphicsInputLayout()
{
}
