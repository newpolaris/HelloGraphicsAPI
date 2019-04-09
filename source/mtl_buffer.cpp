#include "mtl_buffer.h"
#include "debug.h"

using namespace el;

MTLBuffer::MTLBuffer()
{
}

MTLBuffer::~MTLBuffer()
{
}

bool MTLBuffer::create(GraphicsBufferDesc desc)
{
    _desc = std::move(desc);

    return true;
}

void MTLBuffer::destroy()
{
}

void MTLBuffer::bind() const
{
}
