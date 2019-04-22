#include "graphics_buffer.h"

using namespace el;

GraphicsBufferDesc::GraphicsBufferDesc() :
    _data(nullptr),
    _numElements(0),
    _elementSize(0),
    _dataType(GraphicsDataTypeNone),
    _usage(0)
{
}

void GraphicsBufferDesc::setData(const stream_t* data)
{
    _data = data;
}

const char* GraphicsBufferDesc::getData() const
{
    return _data;
}

streamsize_t GraphicsBufferDesc::getDataSize() const
{
    return _elementSize * _numElements;
}

void GraphicsBufferDesc::setElementSize(size_t elementSize)
{
    _elementSize = elementSize;
}

size_t GraphicsBufferDesc::getElementSize() const
{
    return _elementSize;
}

void GraphicsBufferDesc::setNumElements(size_t numElements)
{
    _numElements = numElements;
}

size_t GraphicsBufferDesc::getNumElements() const
{
    return _numElements;
}

void GraphicsBufferDesc::setDataType(GraphicsDataType dataType)
{
    _dataType = dataType;
}

GraphicsDataType GraphicsBufferDesc::getDataType() const
{
    return _dataType;
}

void GraphicsBufferDesc::setUsage(GraphicsUsageFlags usage)
{
    _usage = usage;
}

GraphicsUsageFlags GraphicsBufferDesc::getUsage() const
{
    return _usage;
}

GraphicsBuffer::GraphicsBuffer()
{
}

GraphicsBuffer::~GraphicsBuffer()
{
}