#include "graphics_data.h"

using namespace el;

GraphicsDataDesc::GraphicsDataDesc() :
    _data(nullptr),
    _numElements(0),
    _elementSize(0),
    _dataType(GraphicsDataTypeNone),
    _usage(0)
{
}

void GraphicsDataDesc::setData(const stream_t* data)
{
    _data = data;
}

const char* GraphicsDataDesc::getData() const
{
    return _data;
}

streamsize_t GraphicsDataDesc::getDataSize() const
{
    return _elementSize * _numElements;
}

void GraphicsDataDesc::setElementSize(size_t elementSize)
{
    _elementSize = elementSize;
}

size_t GraphicsDataDesc::getElementSize() const
{
    return _elementSize;
}

void GraphicsDataDesc::setNumElements(size_t numElements)
{
    _numElements = numElements;
}

size_t GraphicsDataDesc::getNumElements() const
{
    return _numElements;
}

void GraphicsDataDesc::setDataType(GraphicsDataType dataType)
{
    _dataType = dataType;
}

GraphicsDataType GraphicsDataDesc::getDataType() const
{
    return _dataType;
}

void GraphicsDataDesc::setUsage(GraphicsUsageFlags usage)
{
    _usage = usage;
}

GraphicsUsageFlags GraphicsDataDesc::getUsage() const
{
    return _usage;
}

GraphicsData::GraphicsData()
{
}

GraphicsData::~GraphicsData()
{
}