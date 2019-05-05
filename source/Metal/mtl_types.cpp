#include <Metal/mtl_types.h>
#include <el_debug.h>

using namespace el;
using namespace mtlpp;

mtlpp::PixelFormat el::asPixelFormat(GraphicsPixelFormat format)
{
    switch (format)
{
    case GraphicsPixelFormat::GraphicsPixelFormatA8Unorm:
        return mtlpp::PixelFormat::A8Unorm;
    case GraphicsPixelFormat::GraphicsPixelFormatR8Unorm:
        return mtlpp::PixelFormat::R8Unorm;
    case GraphicsPixelFormat::GraphicsPixelFormatR8Unorm_sRGB:
        return mtlpp::PixelFormat::R8Unorm_sRGB;
    case GraphicsPixelFormat::GraphicsPixelFormatR8Snorm:
        return mtlpp::PixelFormat::R8Snorm;
    case GraphicsPixelFormat::GraphicsPixelFormatR8Uint:
        return mtlpp::PixelFormat::R8Uint;
    case GraphicsPixelFormat::GraphicsPixelFormatR8Sint:
        return mtlpp::PixelFormat::R8Sint;
    case GraphicsPixelFormat::GraphicsPixelFormatR16Unorm:
        return mtlpp::PixelFormat::R16Unorm;
    case GraphicsPixelFormat::GraphicsPixelFormatR16Snorm:
        return mtlpp::PixelFormat::R16Snorm;
    case GraphicsPixelFormat::GraphicsPixelFormatR16Uint:
        return mtlpp::PixelFormat::R16Uint;
    case GraphicsPixelFormat::GraphicsPixelFormatR16Sint:
        return mtlpp::PixelFormat::R16Sint;
    case GraphicsPixelFormat::GraphicsPixelFormatR16Float:
        return mtlpp::PixelFormat::R16Float;
    case GraphicsPixelFormat::GraphicsPixelFormatRG8Unorm:
        return mtlpp::PixelFormat::RG8Unorm;
    case GraphicsPixelFormat::GraphicsPixelFormatRG8Unorm_sRGB:
        return mtlpp::PixelFormat::RG8Unorm_sRGB;
    case GraphicsPixelFormat::GraphicsPixelFormatRG8Snorm:
        return mtlpp::PixelFormat::RG8Snorm;
    case GraphicsPixelFormat::GraphicsPixelFormatRG8Uint:
        return mtlpp::PixelFormat::RG8Uint;
    case GraphicsPixelFormat::GraphicsPixelFormatRG8Sint:
        return mtlpp::PixelFormat::RG8Sint;
    case GraphicsPixelFormat::GraphicsPixelFormatB5G6R5Unorm:
        return mtlpp::PixelFormat::B5G6R5Unorm;
    case GraphicsPixelFormat::GraphicsPixelFormatA1BGR5Unorm:
        return mtlpp::PixelFormat::A1BGR5Unorm;
    case GraphicsPixelFormat::GraphicsPixelFormatABGR4Unorm:
        return mtlpp::PixelFormat::ABGR4Unorm;
    case GraphicsPixelFormat::GraphicsPixelFormatBGR5A1Unorm:
        return mtlpp::PixelFormat::BGR5A1Unorm;
    case GraphicsPixelFormat::GraphicsPixelFormatR32Uint:
        return mtlpp::PixelFormat::R32Uint;
    case GraphicsPixelFormat::GraphicsPixelFormatR32Sint:
        return mtlpp::PixelFormat::R32Sint;
    case GraphicsPixelFormat::GraphicsPixelFormatR32Float:
        return mtlpp::PixelFormat::R32Float;
    case GraphicsPixelFormat::GraphicsPixelFormatRG16Unorm:
        return mtlpp::PixelFormat::RG16Unorm;
    case GraphicsPixelFormat::GraphicsPixelFormatRG16Snorm:
        return mtlpp::PixelFormat::RG16Snorm;
    case GraphicsPixelFormat::GraphicsPixelFormatRG16Uint:
        return mtlpp::PixelFormat::RG16Uint;
    case GraphicsPixelFormat::GraphicsPixelFormatRG16Sint:
        return mtlpp::PixelFormat::RG16Sint;
    case GraphicsPixelFormat::GraphicsPixelFormatRG16Float:
        return mtlpp::PixelFormat::RG16Float;
    case GraphicsPixelFormat::GraphicsPixelFormatRGBA8Unorm:
        return mtlpp::PixelFormat::RGBA8Unorm;
    case GraphicsPixelFormat::GraphicsPixelFormatRGBA8Unorm_sRGB:
        return mtlpp::PixelFormat::RGBA8Unorm_sRGB;
    case GraphicsPixelFormat::GraphicsPixelFormatRGBA8Snorm:
        return mtlpp::PixelFormat::RGBA8Snorm;
    case GraphicsPixelFormat::GraphicsPixelFormatRGBA8Uint:
        return mtlpp::PixelFormat::RGBA8Uint;
    case GraphicsPixelFormat::GraphicsPixelFormatRGBA8Sint:
        return mtlpp::PixelFormat::RGBA8Sint;
    case GraphicsPixelFormat::GraphicsPixelFormatBGRA8Unorm:
        return mtlpp::PixelFormat::BGRA8Unorm;
    case GraphicsPixelFormat::GraphicsPixelFormatBGRA8Unorm_sRGB:
        return mtlpp::PixelFormat::BGRA8Unorm_sRGB;
    case GraphicsPixelFormat::GraphicsPixelFormatRGB10A2Unorm:
        return mtlpp::PixelFormat::RGB10A2Unorm;
    case GraphicsPixelFormat::GraphicsPixelFormatRGB10A2Uint:
        return mtlpp::PixelFormat::RGB10A2Uint;
    case GraphicsPixelFormat::GraphicsPixelFormatRG11B10Float:
        return mtlpp::PixelFormat::RG11B10Float;
    case GraphicsPixelFormat::GraphicsPixelFormatRGB9E5Float:
        return mtlpp::PixelFormat::RGB9E5Float;
    case GraphicsPixelFormat::GraphicsPixelFormatBGR10_XR:
        return mtlpp::PixelFormat::BGR10_XR;
    case GraphicsPixelFormat::GraphicsPixelFormatBGR10_XR_sRGB:
        return mtlpp::PixelFormat::BGR10_XR_sRGB;
    case GraphicsPixelFormat::GraphicsPixelFormatRG32Uint:
        return mtlpp::PixelFormat::RG32Uint;
    case GraphicsPixelFormat::GraphicsPixelFormatRG32Sint:
        return mtlpp::PixelFormat::RG32Sint;
    case GraphicsPixelFormat::GraphicsPixelFormatRG32Float:
        return mtlpp::PixelFormat::RG32Float;
    case GraphicsPixelFormat::GraphicsPixelFormatRGBA16Unorm:
        return mtlpp::PixelFormat::RGBA16Unorm;
    case GraphicsPixelFormat::GraphicsPixelFormatRGBA16Snorm:
        return mtlpp::PixelFormat::RGBA16Snorm;
    case GraphicsPixelFormat::GraphicsPixelFormatRGBA16Uint:
        return mtlpp::PixelFormat::RGBA16Uint;
    case GraphicsPixelFormat::GraphicsPixelFormatRGBA16Sint:
        return mtlpp::PixelFormat::RGBA16Sint;
    case GraphicsPixelFormat::GraphicsPixelFormatRGBA16Float:
        return mtlpp::PixelFormat::RGBA16Float;
    case GraphicsPixelFormat::GraphicsPixelFormatBGRA10_XR:
        return mtlpp::PixelFormat::BGRA10_XR;
    case GraphicsPixelFormat::GraphicsPixelFormatBGRA10_XR_sRGB:
        return mtlpp::PixelFormat::BGRA10_XR_sRGB;
    case GraphicsPixelFormat::GraphicsPixelFormatRGBA32Uint:
        return mtlpp::PixelFormat::RGBA32Uint;
    case GraphicsPixelFormat::GraphicsPixelFormatRGBA32Sint:
        return mtlpp::PixelFormat::RGBA32Sint;
    case GraphicsPixelFormat::GraphicsPixelFormatRGBA32Float:
        return mtlpp::PixelFormat::RGBA32Float;
    case GraphicsPixelFormat::GraphicsPixelFormatBC1_RGBA:
        return mtlpp::PixelFormat::BC1_RGBA;
    case GraphicsPixelFormat::GraphicsPixelFormatBC1_RGBA_sRGB:
        return mtlpp::PixelFormat::BC1_RGBA_sRGB;
    case GraphicsPixelFormat::GraphicsPixelFormatBC2_RGBA:
        return mtlpp::PixelFormat::BC2_RGBA;
    case GraphicsPixelFormat::GraphicsPixelFormatBC2_RGBA_sRGB:
        return mtlpp::PixelFormat::BC2_RGBA_sRGB;
    case GraphicsPixelFormat::GraphicsPixelFormatBC3_RGBA:
        return mtlpp::PixelFormat::BC3_RGBA;
    case GraphicsPixelFormat::GraphicsPixelFormatBC3_RGBA_sRGB:
        return mtlpp::PixelFormat::BC3_RGBA_sRGB;
    case GraphicsPixelFormat::GraphicsPixelFormatBC4_RUnorm:
        return mtlpp::PixelFormat::BC4_RUnorm;
    case GraphicsPixelFormat::GraphicsPixelFormatBC4_RSnorm:
        return mtlpp::PixelFormat::BC4_RSnorm;
    case GraphicsPixelFormat::GraphicsPixelFormatBC5_RGUnorm:
        return mtlpp::PixelFormat::BC5_RGUnorm;
    case GraphicsPixelFormat::GraphicsPixelFormatBC5_RGSnorm:
        return mtlpp::PixelFormat::BC5_RGSnorm;
    case GraphicsPixelFormat::GraphicsPixelFormatBC6H_RGBFloat:
        return mtlpp::PixelFormat::BC6H_RGBFloat;
    case GraphicsPixelFormat::GraphicsPixelFormatBC6H_RGBUfloat:
        return mtlpp::PixelFormat::BC6H_RGBUfloat;
    case GraphicsPixelFormat::GraphicsPixelFormatBC7_RGBAUnorm:
        return mtlpp::PixelFormat::BC7_RGBAUnorm;
    case GraphicsPixelFormat::GraphicsPixelFormatBC7_RGBAUnorm_sRGB:
        return mtlpp::PixelFormat::BC7_RGBAUnorm_sRGB;
    case GraphicsPixelFormat::GraphicsPixelFormatPVRTC_RGB_2BPP:
        return mtlpp::PixelFormat::PVRTC_RGB_2BPP;
    case GraphicsPixelFormat::GraphicsPixelFormatPVRTC_RGB_2BPP_sRGB:
        return mtlpp::PixelFormat::PVRTC_RGB_2BPP_sRGB;
    case GraphicsPixelFormat::GraphicsPixelFormatPVRTC_RGB_4BPP:
        return mtlpp::PixelFormat::PVRTC_RGB_4BPP;
    case GraphicsPixelFormat::GraphicsPixelFormatPVRTC_RGB_4BPP_sRGB:
        return mtlpp::PixelFormat::PVRTC_RGB_4BPP_sRGB;
    case GraphicsPixelFormat::GraphicsPixelFormatPVRTC_RGBA_2BPP:
        return mtlpp::PixelFormat::PVRTC_RGBA_2BPP;
    case GraphicsPixelFormat::GraphicsPixelFormatPVRTC_RGBA_2BPP_sRGB:
        return mtlpp::PixelFormat::PVRTC_RGBA_2BPP_sRGB;
    case GraphicsPixelFormat::GraphicsPixelFormatPVRTC_RGBA_4BPP:
        return mtlpp::PixelFormat::PVRTC_RGBA_4BPP;
    case GraphicsPixelFormat::GraphicsPixelFormatPVRTC_RGBA_4BPP_sRGB:
        return mtlpp::PixelFormat::PVRTC_RGBA_4BPP_sRGB;
    case GraphicsPixelFormat::GraphicsPixelFormatEAC_R11Unorm:
        return mtlpp::PixelFormat::EAC_R11Unorm;
    case GraphicsPixelFormat::GraphicsPixelFormatEAC_R11Snorm:
        return mtlpp::PixelFormat::EAC_R11Snorm;
    case GraphicsPixelFormat::GraphicsPixelFormatEAC_RG11Unorm:
        return mtlpp::PixelFormat::EAC_RG11Unorm;
    case GraphicsPixelFormat::GraphicsPixelFormatEAC_RG11Snorm:
        return mtlpp::PixelFormat::EAC_RG11Snorm;
    case GraphicsPixelFormat::GraphicsPixelFormatEAC_RGBA8:
        return mtlpp::PixelFormat::EAC_RGBA8;
    case GraphicsPixelFormat::GraphicsPixelFormatEAC_RGBA8_sRGB:
        return mtlpp::PixelFormat::EAC_RGBA8_sRGB;
    case GraphicsPixelFormat::GraphicsPixelFormatETC2_RGB8:
        return mtlpp::PixelFormat::ETC2_RGB8;
    case GraphicsPixelFormat::GraphicsPixelFormatETC2_RGB8_sRGB:
        return mtlpp::PixelFormat::ETC2_RGB8_sRGB;
    case GraphicsPixelFormat::GraphicsPixelFormatETC2_RGB8A1:
        return mtlpp::PixelFormat::ETC2_RGB8A1;
    case GraphicsPixelFormat::GraphicsPixelFormatETC2_RGB8A1_sRGB:
        return mtlpp::PixelFormat::ETC2_RGB8A1_sRGB;
    case GraphicsPixelFormat::GraphicsPixelFormatASTC_4x4_sRGB:
        return mtlpp::PixelFormat::ASTC_4x4_sRGB;
    case GraphicsPixelFormat::GraphicsPixelFormatASTC_5x4_sRGB:
        return mtlpp::PixelFormat::ASTC_5x4_sRGB;
    case GraphicsPixelFormat::GraphicsPixelFormatASTC_5x5_sRGB:
        return mtlpp::PixelFormat::ASTC_5x5_sRGB;
    case GraphicsPixelFormat::GraphicsPixelFormatASTC_6x5_sRGB:
        return mtlpp::PixelFormat::ASTC_6x5_sRGB;
    case GraphicsPixelFormat::GraphicsPixelFormatASTC_6x6_sRGB:
        return mtlpp::PixelFormat::ASTC_6x6_sRGB;
    case GraphicsPixelFormat::GraphicsPixelFormatASTC_8x5_sRGB:
        return mtlpp::PixelFormat::ASTC_8x5_sRGB;
    case GraphicsPixelFormat::GraphicsPixelFormatASTC_8x6_sRGB:
        return mtlpp::PixelFormat::ASTC_8x6_sRGB;
    case GraphicsPixelFormat::GraphicsPixelFormatASTC_8x8_sRGB:
        return mtlpp::PixelFormat::ASTC_8x8_sRGB;
    case GraphicsPixelFormat::GraphicsPixelFormatASTC_10x5_sRGB:
        return mtlpp::PixelFormat::ASTC_10x5_sRGB;
    case GraphicsPixelFormat::GraphicsPixelFormatASTC_10x6_sRGB:
        return mtlpp::PixelFormat::ASTC_10x6_sRGB;
    case GraphicsPixelFormat::GraphicsPixelFormatASTC_10x8_sRGB:
        return mtlpp::PixelFormat::ASTC_10x8_sRGB;
    case GraphicsPixelFormat::GraphicsPixelFormatASTC_10x10_sRGB:
        return mtlpp::PixelFormat::ASTC_10x10_sRGB;
    case GraphicsPixelFormat::GraphicsPixelFormatASTC_12x10_sRGB:
        return mtlpp::PixelFormat::ASTC_12x10_sRGB;
    case GraphicsPixelFormat::GraphicsPixelFormatASTC_12x12_sRGB:
        return mtlpp::PixelFormat::ASTC_12x12_sRGB;
    case GraphicsPixelFormat::GraphicsPixelFormatASTC_4x4_LDR:
        return mtlpp::PixelFormat::ASTC_4x4_LDR;
    case GraphicsPixelFormat::GraphicsPixelFormatASTC_5x4_LDR:
        return mtlpp::PixelFormat::ASTC_5x4_LDR;
    case GraphicsPixelFormat::GraphicsPixelFormatASTC_5x5_LDR:
        return mtlpp::PixelFormat::ASTC_5x5_LDR;
    case GraphicsPixelFormat::GraphicsPixelFormatASTC_6x5_LDR:
        return mtlpp::PixelFormat::ASTC_6x5_LDR;
    case GraphicsPixelFormat::GraphicsPixelFormatASTC_6x6_LDR:
        return mtlpp::PixelFormat::ASTC_6x6_LDR;
    case GraphicsPixelFormat::GraphicsPixelFormatASTC_8x5_LDR:
        return mtlpp::PixelFormat::ASTC_8x5_LDR;
    case GraphicsPixelFormat::GraphicsPixelFormatASTC_8x6_LDR:
        return mtlpp::PixelFormat::ASTC_8x6_LDR;
    case GraphicsPixelFormat::GraphicsPixelFormatASTC_8x8_LDR:
        return mtlpp::PixelFormat::ASTC_8x8_LDR;
    case GraphicsPixelFormat::GraphicsPixelFormatASTC_10x5_LDR:
        return mtlpp::PixelFormat::ASTC_10x5_LDR;
    case GraphicsPixelFormat::GraphicsPixelFormatASTC_10x6_LDR:
        return mtlpp::PixelFormat::ASTC_10x6_LDR;
    case GraphicsPixelFormat::GraphicsPixelFormatASTC_10x8_LDR:
        return mtlpp::PixelFormat::ASTC_10x8_LDR;
    case GraphicsPixelFormat::GraphicsPixelFormatASTC_10x10_LDR:
        return mtlpp::PixelFormat::ASTC_10x10_LDR;
    case GraphicsPixelFormat::GraphicsPixelFormatASTC_12x10_LDR:
        return mtlpp::PixelFormat::ASTC_12x10_LDR;
    case GraphicsPixelFormat::GraphicsPixelFormatASTC_12x12_LDR:
        return mtlpp::PixelFormat::ASTC_12x12_LDR;
    case GraphicsPixelFormat::GraphicsPixelFormatGBGR422:
        return mtlpp::PixelFormat::GBGR422;
    case GraphicsPixelFormat::GraphicsPixelFormatBGRG422:
        return mtlpp::PixelFormat::BGRG422;
    case GraphicsPixelFormat::GraphicsPixelFormatDepth16Unorm:
        return mtlpp::PixelFormat::Depth16Unorm;
    case GraphicsPixelFormat::GraphicsPixelFormatDepth32Float:
        return mtlpp::PixelFormat::Depth32Float;
    case GraphicsPixelFormat::GraphicsPixelFormatStencil8:
        return mtlpp::PixelFormat::Stencil8;
    case GraphicsPixelFormat::GraphicsPixelFormatDepth24Unorm_Stencil8:
        return mtlpp::PixelFormat::Depth24Unorm_Stencil8;
    case GraphicsPixelFormat::GraphicsPixelFormatDepth32Float_Stencil8:
        return mtlpp::PixelFormat::Depth32Float_Stencil8;
    case GraphicsPixelFormat::GraphicsPixelFormatX32_Stencil8:
        return mtlpp::PixelFormat::X32_Stencil8;
    case GraphicsPixelFormat::GraphicsPixelFormatX24_Stencil8:
        return mtlpp::PixelFormat::X24_Stencil8;
    default:
        EL_ASSERT(false);
        return mtlpp::PixelFormat::Invalid;
    }
}

mtlpp::TextureUsage el::asTextureUsage(GraphicsTextureUsageFlags flags)
{
    uint32_t usage = 0;

    if (flags & GraphicsTextureUsageFlagBitStorageBit)
        usage |= TextureUsage::ShaderWrite;
    if (flags & GraphicsTextureUsageFlagBitColorAttachmentBit)
        usage |= TextureUsage::RenderTarget;
    if (flags & GraphicsTextureUsageFlagBitDepthStencilAttachmentBit)
        usage |= TextureUsage::RenderTarget;
    if (flags & GraphicsTextureUsageFlagBitSampledBit)
        usage |= TextureUsage::ShaderRead;

    return mtlpp::TextureUsage(usage);
}

mtlpp::SamplerMinMagFilter el::asSamplerMinMagFilter(GraphicsFilter filter)
{
    switch (filter)
    {
    case GraphicsFilterNearest:
        return SamplerMinMagFilter::Nearest;
    case GraphicsFilterLinear:
        return SamplerMinMagFilter::Linear;
    default:
        EL_ASSERT(false);
        return SamplerMinMagFilter(0);
    }
}

mtlpp::SamplerMipFilter el::asSamplerMipFilter(GraphicsSamplerMipmapMode mode)
{
    switch (mode)
    {
    case GraphicsSamplerMipmapModeNone:
        return SamplerMipFilter::NotMipmapped;
    case GraphicsSamplerMipmapModeNearest:
        return SamplerMipFilter::Nearest;
    case GraphicsSamplerMipmapModeLinear:
        return SamplerMipFilter::Linear;
    default:
        EL_ASSERT(false);
        return SamplerMipFilter(0);
    }
}

mtlpp::SamplerAddressMode el::asSamplerAddressMode(GraphicsSamplerAddressMode mode)
{
    switch (mode)
    {
    case GraphicsSamplerAddressModeRepeat:
        return SamplerAddressMode::Repeat;
    case GraphicsSamplerAddressModeMirroredReapt:
        return SamplerAddressMode::MirrorRepeat;
    case GraphicsSamplerAddressModeClampToEdge:
        return SamplerAddressMode::ClampToEdge;
    case GraphicsSamplerAddressModeClampToBorder:
        return SamplerAddressMode::ClampToBorderColor;
    case GraphicsSamplerAddressModeMirrorClampToEdge:
        return SamplerAddressMode::MirrorClampToEdge;
    default:
        EL_ASSERT(false);
        return SamplerAddressMode(0);
    }
}

mtlpp::SamplerBorderColor el::asSamplerBorderColor(GraphicsBorderColor color)
{
    switch (color)
    {
    case GraphicsBorderColorFloatTransparentBlack:
        return SamplerBorderColor::TransparentBlack;  // {0,0,0,0}
    case GraphicsBorderColorFloatOpaqueBlack:
        return SamplerBorderColor::OpaqueBlack;       // {0,0,0,1}
    case GraphicsBorderColorFloatOpaqueWhite:
        return SamplerBorderColor::OpaqueWhite;       // {1,1,1,1}

    case GraphicsBorderColorIntTransparentBlack:
    case GraphicsBorderColorIntOpaqueBlack:
    case GraphicsBorderColorIntOpaqueWhite:
    default:
        EL_ASSERT(false);
        return SamplerBorderColor(0);
    }
}

mtlpp::CompareFunction el::asCompareFunction(GraphicsCompareOp func)
{
    switch (func)
    {
    case GraphicsCompareOp::GraphicsCompareOpNever:
        return CompareFunction::Never;
    case GraphicsCompareOp::GraphicsCompareOpLess:
        return CompareFunction::Less;
    case GraphicsCompareOp::GraphicsCompareOpEqual:
        return CompareFunction::Equal;
    case GraphicsCompareOp::GraphicsCompareOpLessOrEqual:
        return CompareFunction::LessEqual;
    case GraphicsCompareOp::GraphicsCompareOpGreater:
        return CompareFunction::Greater;
    case GraphicsCompareOp::GraphicsCompareOpNotEqual:
        return CompareFunction::NotEqual;
    case GraphicsCompareOp::GraphicsCompareOpGreaterOrEqual:
        return CompareFunction::GreaterEqual;
    case GraphicsCompareOp::GraphicsCompareOpAlways:
        return CompareFunction::Always;
    default:
        EL_ASSERT(false);
        return CompareFunction(0);
    }
}


mtlpp::StencilOperation el::asStencilOperation(GraphicsStencilOp func)
{
    switch (func)
    {
    case GraphicsStencilOp::GraphicsStencilOpKeep:
        return StencilOperation::Keep;
    case GraphicsStencilOp::GraphicsStencilOpZero:
        return StencilOperation::Zero;
    case GraphicsStencilOp::GraphicsStencilOpReplace:
        return StencilOperation::Replace;
    case GraphicsStencilOp::GraphicsStencilOpIncrementAndClamp:
        return StencilOperation::IncrementClamp;
    case GraphicsStencilOp::GraphicsStencilOpDecrementAndClamp:
        return StencilOperation::DecrementClamp;
    case GraphicsStencilOp::GraphicsStencilOpInvert:
        return StencilOperation::Invert;
    case GraphicsStencilOp::GraphicsStencilOpIncrementAndWrap:
        return StencilOperation::IncrementWrap;
    case GraphicsStencilOp::GraphicsStencilOpDecrementAndWrap:
        return StencilOperation::DecrementWrap;
    default:
        EL_ASSERT(false);
        return StencilOperation(0);
    }
}

mtlpp::VertexFormat el::asVertexFormat(GraphicsVertexFormat format)
{
    switch (format)
    {
    case GraphicsVertexFormat::UChar2:
        return VertexFormat::UChar2;
    case GraphicsVertexFormat::UChar3:
        return VertexFormat::UChar3;
    case GraphicsVertexFormat::UChar4:
        return VertexFormat::UChar4;
    case GraphicsVertexFormat::Char2:
        return VertexFormat::Char2;
    case GraphicsVertexFormat::Char3:
        return VertexFormat::Char3;
    case GraphicsVertexFormat::Char4:
        return VertexFormat::Char4;
    case GraphicsVertexFormat::UChar2Normalized:
        return VertexFormat::UChar2Normalized;
    case GraphicsVertexFormat::UChar3Normalized:
        return VertexFormat::UChar3Normalized;
    case GraphicsVertexFormat::UChar4Normalized:
        return VertexFormat::UChar4Normalized;
    case GraphicsVertexFormat::Char2Normalized:
        return VertexFormat::Char2Normalized;
    case GraphicsVertexFormat::Char3Normalized:
        return VertexFormat::Char3Normalized;
    case GraphicsVertexFormat::Char4Normalized:
        return VertexFormat::Char4Normalized;
    case GraphicsVertexFormat::UShort2:
        return VertexFormat::UShort2;
    case GraphicsVertexFormat::UShort3:
        return VertexFormat::UShort3;
    case GraphicsVertexFormat::UShort4:
        return VertexFormat::UShort4;

    case GraphicsVertexFormat::Short2:
        return VertexFormat::Short2;
    case GraphicsVertexFormat::Short3:
        return VertexFormat::Short3;
    case GraphicsVertexFormat::Short4:
        return VertexFormat::Short4;

    case GraphicsVertexFormat::UShort2Normalized:
        return VertexFormat::UShort2Normalized;
    case GraphicsVertexFormat::UShort3Normalized:
        return VertexFormat::UShort3Normalized;
    case GraphicsVertexFormat::UShort4Normalized:
        return VertexFormat::UShort4Normalized;

    case GraphicsVertexFormat::Short2Normalized:
        return VertexFormat::Short2Normalized;
    case GraphicsVertexFormat::Short3Normalized:
        return VertexFormat::Short3Normalized;
    case GraphicsVertexFormat::Short4Normalized:
        return VertexFormat::Short4Normalized;

    case GraphicsVertexFormat::Half2:
        return VertexFormat::Half2;
    case GraphicsVertexFormat::Half3:
        return VertexFormat::Half3;
    case GraphicsVertexFormat::Half4:
        return VertexFormat::Half4;

    case GraphicsVertexFormat::Float:
        return VertexFormat::Float;
    case GraphicsVertexFormat::Float2:
        return VertexFormat::Float2;
    case GraphicsVertexFormat::Float3:
        return VertexFormat::Float3;
    case GraphicsVertexFormat::Float4:
        return VertexFormat::Float4;

    case GraphicsVertexFormat::Int:
        return VertexFormat::Int;
    case GraphicsVertexFormat::Int2:
        return VertexFormat::Int2;
    case GraphicsVertexFormat::Int3:
        return VertexFormat::Int3;
    case GraphicsVertexFormat::Int4:
        return VertexFormat::Int4;

    case GraphicsVertexFormat::UInt:
        return VertexFormat::UInt;
    case GraphicsVertexFormat::UInt2:
        return VertexFormat::UInt2;
    case GraphicsVertexFormat::UInt3:
        return VertexFormat::UInt3;
    case GraphicsVertexFormat::UInt4:
        return VertexFormat::UInt4;

    case GraphicsVertexFormat::Int1010102Normalized:
        return VertexFormat::Int1010102Normalized;
    case GraphicsVertexFormat::UInt1010102Normalized:
        return VertexFormat::UInt1010102Normalized;
    default:
        EL_ASSERT(false);
        return VertexFormat(0);
    }
}

mtlpp::VertexStepFunction el::asVertexStepFunction(GraphicsInputRate func)
{
    switch (func)
    {
    case GraphicsInputRateVertex:
        return VertexStepFunction::PerVertex;
    case GraphicsInputRateInstance:
        return VertexStepFunction::PerInstance;
    default:
        EL_ASSERT("false");
        return VertexStepFunction(0);
    }
}
