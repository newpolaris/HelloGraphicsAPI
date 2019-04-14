#include "mtl_buffer.h"

#if EL_PLAT_APPLE

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

#endif // #if EL_PLAT_APPLE
