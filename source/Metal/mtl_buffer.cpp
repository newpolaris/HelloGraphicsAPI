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

bool MTLBuffer::create(GraphicsDataDesc desc)
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

const GraphicsDataDesc& MTLBuffer::getDesc() const
{
    return _desc;
}

#endif // #if EL_PLAT_APPLE
