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

uint32_t el::asTexelSize(GraphicsPixelFormat format)
{
    switch (format)
    {
    case GraphicsPixelFormat::GraphicsPixelFormatA8Unorm:
    case GraphicsPixelFormat::GraphicsPixelFormatR8Unorm:
    case GraphicsPixelFormat::GraphicsPixelFormatR8Unorm_sRGB:
    case GraphicsPixelFormat::GraphicsPixelFormatR8Snorm:
    case GraphicsPixelFormat::GraphicsPixelFormatR8Uint:
    case GraphicsPixelFormat::GraphicsPixelFormatR8Sint:
        return 1;
    case GraphicsPixelFormat::GraphicsPixelFormatR16Unorm:
    case GraphicsPixelFormat::GraphicsPixelFormatR16Snorm:
    case GraphicsPixelFormat::GraphicsPixelFormatR16Uint:
    case GraphicsPixelFormat::GraphicsPixelFormatR16Sint:
    case GraphicsPixelFormat::GraphicsPixelFormatR16Float:
    case GraphicsPixelFormat::GraphicsPixelFormatRG8Unorm:
    case GraphicsPixelFormat::GraphicsPixelFormatRG8Unorm_sRGB:
    case GraphicsPixelFormat::GraphicsPixelFormatRG8Snorm:
    case GraphicsPixelFormat::GraphicsPixelFormatRG8Uint:
    case GraphicsPixelFormat::GraphicsPixelFormatRG8Sint:
    case GraphicsPixelFormat::GraphicsPixelFormatB5G6R5Unorm:
    case GraphicsPixelFormat::GraphicsPixelFormatA1BGR5Unorm:
    case GraphicsPixelFormat::GraphicsPixelFormatABGR4Unorm:
    case GraphicsPixelFormat::GraphicsPixelFormatBGR5A1Unorm:
        return 2;
    case GraphicsPixelFormat::GraphicsPixelFormatR32Uint:
    case GraphicsPixelFormat::GraphicsPixelFormatR32Sint:
    case GraphicsPixelFormat::GraphicsPixelFormatR32Float:
    case GraphicsPixelFormat::GraphicsPixelFormatRG16Unorm:
    case GraphicsPixelFormat::GraphicsPixelFormatRG16Snorm:
    case GraphicsPixelFormat::GraphicsPixelFormatRG16Uint:
    case GraphicsPixelFormat::GraphicsPixelFormatRG16Sint:
    case GraphicsPixelFormat::GraphicsPixelFormatRG16Float:
    case GraphicsPixelFormat::GraphicsPixelFormatRGBA8Unorm:
    case GraphicsPixelFormat::GraphicsPixelFormatRGBA8Unorm_sRGB:
    case GraphicsPixelFormat::GraphicsPixelFormatRGBA8Snorm:
    case GraphicsPixelFormat::GraphicsPixelFormatRGBA8Uint:
    case GraphicsPixelFormat::GraphicsPixelFormatRGBA8Sint:
    case GraphicsPixelFormat::GraphicsPixelFormatBGRA8Unorm:
    case GraphicsPixelFormat::GraphicsPixelFormatBGRA8Unorm_sRGB:
    case GraphicsPixelFormat::GraphicsPixelFormatRGB10A2Unorm:
    case GraphicsPixelFormat::GraphicsPixelFormatRGB10A2Uint:
    case GraphicsPixelFormat::GraphicsPixelFormatRG11B10Float:
    case GraphicsPixelFormat::GraphicsPixelFormatRGB9E5Float:
        return 4;
    case GraphicsPixelFormat::GraphicsPixelFormatRG32Uint:
    case GraphicsPixelFormat::GraphicsPixelFormatRG32Sint:
    case GraphicsPixelFormat::GraphicsPixelFormatRG32Float:
    case GraphicsPixelFormat::GraphicsPixelFormatRGBA16Unorm:
    case GraphicsPixelFormat::GraphicsPixelFormatRGBA16Snorm:
    case GraphicsPixelFormat::GraphicsPixelFormatRGBA16Uint:
    case GraphicsPixelFormat::GraphicsPixelFormatRGBA16Sint:
    case GraphicsPixelFormat::GraphicsPixelFormatRGBA16Float:
        return 8;
    case GraphicsPixelFormat::GraphicsPixelFormatRGBA32Uint:
    case GraphicsPixelFormat::GraphicsPixelFormatRGBA32Sint:
    case GraphicsPixelFormat::GraphicsPixelFormatRGBA32Float:
        return 16;
    default:
        EL_ASSERT(false);
        return 0;
    };
}
