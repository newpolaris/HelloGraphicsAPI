#include "graphics_buffer.h"

using namespace el;

GraphicsBufferDesc::GraphicsBufferDesc() :
    _data(nullptr),
    _dataSize(0),
    _dataType(GraphicsDataTypeNone),
    _usage(0)
{
}

void GraphicsBufferDesc::setData(const char* data)
{
    _data = data;
}

const char* GraphicsBufferDesc::getData() const
{
    return _data;
}

void GraphicsBufferDesc::setDataSize(size_t size)
{
    _dataSize = size;
}

size_t GraphicsBufferDesc::getDataSize() const
{
    return _dataSize;
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
