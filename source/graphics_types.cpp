#include <graphics_types.h>
#include <debug.h>

using namespace el;

uint32_t el::asVertexFormatSize(VertexFormat format)
{
    switch (format) 
    {
    case VertexFormat::Float:               return 4;
    case VertexFormat::Float2:              return 4*2;
    case VertexFormat::Float3:              return 4*3;
    case VertexFormat::Float4:              return 4*4;
    default:
        EL_ASSERT(false);
        return 0;
    }
}
