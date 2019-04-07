#include "graphics_buffer.h"

using namespace el;

GraphicsBufferDesc::GraphicsBufferDesc()
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

GraphicsBuffer::GraphicsBuffer()
{
}

GraphicsBuffer::~GraphicsBuffer()
{
}