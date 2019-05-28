#include "metal_types.h"

_EL_NAME_BEGIN

MTLClearColor asMTLClearColor(const math::float4 &color)
{
    return MTLClearColorMake(color.r, color.g, color.b, color.a);
}

MTLLoadAction asLoadAction(const RenderPassParms& params, GraphicsTargetBufferFlagBits bit)
{
    if (params.flags.clear & bit)
        return MTLLoadActionClear;
    if (params.flags.loadDiscard & bit)
        return MTLLoadActionDontCare;
    return MTLLoadActionLoad;
}

MTLStoreAction asStoreAction(const RenderPassParms& params, GraphicsTargetBufferFlagBits bit)
{
    if (params.flags.storeDiscard & bit)
        return MTLStoreActionDontCare;
    return MTLStoreActionStore;
}

MTLPixelFormat asMetalPixelFormat(GraphicsPixelFormat format)
{
    switch (format)
    {
    case GraphicsPixelFormatA8Unorm:
        return MTLPixelFormatA8Unorm;
    case GraphicsPixelFormatR8Unorm:
        return MTLPixelFormatR8Unorm;
    case GraphicsPixelFormatR8Snorm:
        return MTLPixelFormatR8Snorm;
    case GraphicsPixelFormatR8Uint:
        return MTLPixelFormatR8Uint;
    case GraphicsPixelFormatR8Sint:
        return MTLPixelFormatR8Sint;
    case GraphicsPixelFormatR16Unorm:
        return MTLPixelFormatR16Unorm;
    case GraphicsPixelFormatR16Snorm:
        return MTLPixelFormatR16Snorm;
    case GraphicsPixelFormatR16Uint:
        return MTLPixelFormatR16Uint;
    case GraphicsPixelFormatR16Sint:
        return MTLPixelFormatR16Sint;
    case GraphicsPixelFormatR16Float:
        return MTLPixelFormatR16Float;
    case GraphicsPixelFormatRG8Unorm:
        return MTLPixelFormatRG8Unorm;
    case GraphicsPixelFormatRG8Snorm:
        return MTLPixelFormatRG8Snorm;
    case GraphicsPixelFormatRG8Uint:
        return MTLPixelFormatRG8Uint;
    case GraphicsPixelFormatRG8Sint:
        return MTLPixelFormatRG8Sint;
    case GraphicsPixelFormatR32Uint:
        return MTLPixelFormatR32Uint;
    case GraphicsPixelFormatR32Sint:
        return MTLPixelFormatR32Sint;
    case GraphicsPixelFormatR32Float:
        return MTLPixelFormatR32Float;
    case GraphicsPixelFormatRG16Unorm:
        return MTLPixelFormatRG16Unorm;
    case GraphicsPixelFormatRG16Snorm:
        return MTLPixelFormatRG16Snorm;
    case GraphicsPixelFormatRG16Uint:
        return MTLPixelFormatRG16Uint;
    case GraphicsPixelFormatRG16Sint:
        return MTLPixelFormatRG16Sint;
    case GraphicsPixelFormatRG16Float:
        return MTLPixelFormatRG16Float;
    case GraphicsPixelFormatRGBA8Unorm:
        return MTLPixelFormatRGBA8Unorm;
    case GraphicsPixelFormatRGBA8Unorm_sRGB:
        return MTLPixelFormatRGBA8Unorm_sRGB;
    case GraphicsPixelFormatRGBA8Snorm:
        return MTLPixelFormatRGBA8Snorm;
    case GraphicsPixelFormatRGBA8Uint:
        return MTLPixelFormatRGBA8Uint;
    case GraphicsPixelFormatRGBA8Sint:
        return MTLPixelFormatRGBA8Sint;
    case GraphicsPixelFormatBGRA8Unorm:
        return MTLPixelFormatBGRA8Unorm;
    case GraphicsPixelFormatBGRA8Unorm_sRGB:
        return MTLPixelFormatBGRA8Unorm_sRGB;
    case GraphicsPixelFormatRGB10A2Unorm:
        return MTLPixelFormatRGB10A2Unorm;
    case GraphicsPixelFormatRGB10A2Uint:
        return MTLPixelFormatRGB10A2Uint;
    case GraphicsPixelFormatRG11B10Float:
        return MTLPixelFormatRG11B10Float;
    case GraphicsPixelFormatRGB9E5Float:
        return MTLPixelFormatRGB9E5Float;
    case GraphicsPixelFormatRG32Uint:
        return MTLPixelFormatRG32Uint;
    case GraphicsPixelFormatRG32Sint:
        return MTLPixelFormatRG32Sint;
    case GraphicsPixelFormatRG32Float:
        return MTLPixelFormatRG32Float;
    case GraphicsPixelFormatRGBA16Unorm:
        return MTLPixelFormatRGBA16Unorm;
    case GraphicsPixelFormatRGBA16Snorm:
        return MTLPixelFormatRGBA16Snorm;
    case GraphicsPixelFormatRGBA16Uint:
        return MTLPixelFormatRGBA16Uint;
    case GraphicsPixelFormatRGBA16Sint:
        return MTLPixelFormatRGBA16Sint;
    case GraphicsPixelFormatRGBA16Float:
        return MTLPixelFormatRGBA16Float;
    case GraphicsPixelFormatRGBA32Uint:
        return MTLPixelFormatRGBA32Uint;
    case GraphicsPixelFormatRGBA32Sint:
        return MTLPixelFormatRGBA32Sint;
    case GraphicsPixelFormatRGBA32Float:
        return MTLPixelFormatRGBA32Float;
    case GraphicsPixelFormatBC1_RGBA:
        return MTLPixelFormatBC1_RGBA;
    case GraphicsPixelFormatBC1_RGBA_sRGB:
        return MTLPixelFormatBC1_RGBA_sRGB;
    case GraphicsPixelFormatBC2_RGBA:
        return MTLPixelFormatBC2_RGBA;
    case GraphicsPixelFormatBC2_RGBA_sRGB:
        return MTLPixelFormatBC2_RGBA_sRGB;
    case GraphicsPixelFormatBC3_RGBA:
        return MTLPixelFormatBC3_RGBA;
    case GraphicsPixelFormatBC3_RGBA_sRGB:
        return MTLPixelFormatBC3_RGBA_sRGB;
    case GraphicsPixelFormatBC4_RUnorm:
        return MTLPixelFormatBC4_RUnorm;
    case GraphicsPixelFormatBC4_RSnorm:
        return MTLPixelFormatBC4_RSnorm;
    case GraphicsPixelFormatBC5_RGUnorm:
        return MTLPixelFormatBC5_RGUnorm;
    case GraphicsPixelFormatBC5_RGSnorm:
        return MTLPixelFormatBC5_RGSnorm;
    case GraphicsPixelFormatBC6H_RGBFloat:
        return MTLPixelFormatBC6H_RGBFloat;
    case GraphicsPixelFormatBC6H_RGBUfloat:
        return MTLPixelFormatBC6H_RGBUfloat;
    case GraphicsPixelFormatBC7_RGBAUnorm:
        return MTLPixelFormatBC7_RGBAUnorm;
    case GraphicsPixelFormatBC7_RGBAUnorm_sRGB:
        return MTLPixelFormatBC7_RGBAUnorm_sRGB;
    case GraphicsPixelFormatGBGR422:
        return MTLPixelFormatGBGR422;
    case GraphicsPixelFormatBGRG422:
        return MTLPixelFormatBGRG422;
    case GraphicsPixelFormatDepth16Unorm:
        return MTLPixelFormatDepth16Unorm;
    case GraphicsPixelFormatDepth32Float:
        return MTLPixelFormatDepth32Float;
    case GraphicsPixelFormatStencil8:
        return MTLPixelFormatStencil8;
    case GraphicsPixelFormatDepth24Unorm_Stencil8:
        return MTLPixelFormatDepth24Unorm_Stencil8;
    case GraphicsPixelFormatDepth32Float_Stencil8:
        return MTLPixelFormatDepth32Float_Stencil8;
    case GraphicsPixelFormatX32_Stencil8:
        return MTLPixelFormatX32_Stencil8;
    case GraphicsPixelFormatX24_Stencil8:
        return MTLPixelFormatX24_Stencil8;
 #if EL_PLAT_IOS
    case GraphicsPixelFormatR8Unorm_sRGB:
        return MTLPixelFormatR8Unorm_sRGB;
    case GraphicsPixelFormatRG8Unorm_sRGB:
        return MTLPixelFormatRG8Unorm_sRGB;
    case GraphicsPixelFormatB5G6R5Unorm:
        return MTLPixelFormatB5G6R5Unorm;
    case GraphicsPixelFormatA1BGR5Unorm:
        return MTLPixelFormatA1BGR5Unorm;
    case GraphicsPixelFormatABGR4Unorm:
        return MTLPixelFormatABGR4Unorm;
    case GraphicsPixelFormatBGR5A1Unorm:
        return MTLPixelFormatBGR5A1Unorm;
    case GraphicsPixelFormatBGR10_XR:
        return MTLPixelFormatBGR10_XR;
    case GraphicsPixelFormatBGR10_XR_sRGB:
        return MTLPixelFormatBGR10_XR_sRGB;
    case GraphicsPixelFormatBGRA10_XR:
        return MTLPixelFormatBGRA10_XR;
    case GraphicsPixelFormatBGRA10_XR_sRGB:
        return MTLPixelFormatBGRA10_XR_sRGB;
    case GraphicsPixelFormatPVRTC_RGB_2BPP:
        return MTLPixelFormatPVRTC_RGB_2BPP;
    case GraphicsPixelFormatPVRTC_RGB_2BPP_sRGB:
        return MTLPixelFormatPVRTC_RGB_2BPP_sRGB;
    case GraphicsPixelFormatPVRTC_RGB_4BPP:
        return MTLPixelFormatPVRTC_RGB_4BPP;
    case GraphicsPixelFormatPVRTC_RGB_4BPP_sRGB:
        return MTLPixelFormatPVRTC_RGB_4BPP_sRGB;
    case GraphicsPixelFormatPVRTC_RGBA_2BPP:
        return MTLPixelFormatPVRTC_RGBA_2BPP;
    case GraphicsPixelFormatPVRTC_RGBA_2BPP_sRGB:
        return MTLPixelFormatPVRTC_RGBA_2BPP_sRGB;
    case GraphicsPixelFormatPVRTC_RGBA_4BPP:
        return MTLPixelFormatPVRTC_RGBA_4BPP;
    case GraphicsPixelFormatPVRTC_RGBA_4BPP_sRGB:
        return MTLPixelFormatPVRTC_RGBA_4BPP_sRGB;
    case GraphicsPixelFormatEAC_R11Unorm:
        return MTLPixelFormatEAC_R11Unorm;
    case GraphicsPixelFormatEAC_R11Snorm:
        return MTLPixelFormatEAC_R11Snorm;
    case GraphicsPixelFormatEAC_RG11Unorm:
        return MTLPixelFormatEAC_RG11Unorm;
    case GraphicsPixelFormatEAC_RG11Snorm:
        return MTLPixelFormatEAC_RG11Snorm;
    case GraphicsPixelFormatEAC_RGBA8:
        return MTLPixelFormatEAC_RGBA8;
    case GraphicsPixelFormatEAC_RGBA8_sRGB:
        return MTLPixelFormatEAC_RGBA8_sRGB;
    case GraphicsPixelFormatETC2_RGB8:
        return MTLPixelFormatETC2_RGB8;
    case GraphicsPixelFormatETC2_RGB8_sRGB:
        return MTLPixelFormatETC2_RGB8_sRGB;
    case GraphicsPixelFormatETC2_RGB8A1:
        return MTLPixelFormatETC2_RGB8A1;
    case GraphicsPixelFormatETC2_RGB8A1_sRGB:
        return MTLPixelFormatETC2_RGB8A1_sRGB;
    case GraphicsPixelFormatASTC_4x4_sRGB:
        return MTLPixelFormatASTC_4x4_sRGB;
    case GraphicsPixelFormatASTC_5x4_sRGB:
        return MTLPixelFormatASTC_5x4_sRGB;
    case GraphicsPixelFormatASTC_5x5_sRGB:
        return MTLPixelFormatASTC_5x5_sRGB;
    case GraphicsPixelFormatASTC_6x5_sRGB:
        return MTLPixelFormatASTC_6x5_sRGB;
    case GraphicsPixelFormatASTC_6x6_sRGB:
        return MTLPixelFormatASTC_6x6_sRGB;
    case GraphicsPixelFormatASTC_8x5_sRGB:
        return MTLPixelFormatASTC_8x5_sRGB;
    case GraphicsPixelFormatASTC_8x6_sRGB:
        return MTLPixelFormatASTC_8x6_sRGB;
    case GraphicsPixelFormatASTC_8x8_sRGB:
        return MTLPixelFormatASTC_8x8_sRGB;
    case GraphicsPixelFormatASTC_10x5_sRGB:
        return MTLPixelFormatASTC_10x5_sRGB;
    case GraphicsPixelFormatASTC_10x6_sRGB:
        return MTLPixelFormatASTC_10x6_sRGB;
    case GraphicsPixelFormatASTC_10x8_sRGB:
        return MTLPixelFormatASTC_10x8_sRGB;
    case GraphicsPixelFormatASTC_10x10_sRGB:
        return MTLPixelFormatASTC_10x10_sRGB;
    case GraphicsPixelFormatASTC_12x10_sRGB:
        return MTLPixelFormatASTC_12x10_sRGB;
    case GraphicsPixelFormatASTC_12x12_sRGB:
        return MTLPixelFormatASTC_12x12_sRGB;
    case GraphicsPixelFormatASTC_4x4_LDR:
        return MTLPixelFormatASTC_4x4_LDR;
    case GraphicsPixelFormatASTC_5x4_LDR:
        return MTLPixelFormatASTC_5x4_LDR;
    case GraphicsPixelFormatASTC_5x5_LDR:
        return MTLPixelFormatASTC_5x5_LDR;
    case GraphicsPixelFormatASTC_6x5_LDR:
        return MTLPixelFormatASTC_6x5_LDR;
    case GraphicsPixelFormatASTC_6x6_LDR:
        return MTLPixelFormatASTC_6x6_LDR;
    case GraphicsPixelFormatASTC_8x5_LDR:
        return MTLPixelFormatASTC_8x5_LDR;
    case GraphicsPixelFormatASTC_8x6_LDR:
        return MTLPixelFormatASTC_8x6_LDR;
    case GraphicsPixelFormatASTC_8x8_LDR:
        return MTLPixelFormatASTC_8x8_LDR;
    case GraphicsPixelFormatASTC_10x5_LDR:
        return MTLPixelFormatASTC_10x5_LDR;
    case GraphicsPixelFormatASTC_10x6_LDR:
        return MTLPixelFormatASTC_10x6_LDR;
    case GraphicsPixelFormatASTC_10x8_LDR:
        return MTLPixelFormatASTC_10x8_LDR;
    case GraphicsPixelFormatASTC_10x10_LDR:
        return MTLPixelFormatASTC_10x10_LDR;
    case GraphicsPixelFormatASTC_12x10_LDR:
        return MTLPixelFormatASTC_12x10_LDR;
    case GraphicsPixelFormatASTC_12x12_LDR:
        return MTLPixelFormatASTC_12x12_LDR;
#endif // EL_PLAT_IOS
    default:
        EL_ASSERT(false);
        return MTLPixelFormatInvalid;
    }
}

MTLTextureUsage asMetalTextureUsage(GraphicsTextureUsageFlags flags)
{
    uint32_t usage = 0;
    
    if (flags & GraphicsTextureUsageStorageBit)
        usage |= MTLTextureUsageShaderWrite;
    if (flags & GraphicsTextureUsageColorAttachmentBit)
        usage |= MTLTextureUsageRenderTarget;
    if (flags & GraphicsTextureUsageDepthStencilAttachmentBit)
        usage |= MTLTextureUsageRenderTarget;
    if (flags & GraphicsTextureUsageSampledBit)
        usage |= MTLTextureUsageShaderRead;
    
    return MTLTextureUsage(usage);
}

MTLResourceOptions asMetalTextureResourceOptions(GraphicsTextureUsageFlags flags)
{
    if (flags & GraphicsTextureUsageUploadableBit)
    {
#if EL_PLAT_IOS
        return MTLResourceStorageModeShared;
#else
        return MTLResourceStorageModeManaged;
#endif
    }
    return MTLResourceStorageModePrivate;
}

MTLSamplerMinMagFilter asSamplerMinMagFilter(GraphicsFilter filter)
{
    switch (filter)
    {
        case GraphicsFilterNearest:
            return MTLSamplerMinMagFilterNearest;
        case GraphicsFilterLinear:
            return MTLSamplerMinMagFilterLinear;
        default:
            EL_ASSERT(false);
            return MTLSamplerMinMagFilter(0);
    }
}

MTLSamplerMipFilter asSamplerMipFilter(GraphicsSamplerMipmapMode mode)
{
    switch (mode)
    {
        case GraphicsSamplerMipmapModeNone:
            return MTLSamplerMipFilterNotMipmapped;
        case GraphicsSamplerMipmapModeNearest:
            return MTLSamplerMipFilterNearest;
        case GraphicsSamplerMipmapModeLinear:
            return MTLSamplerMipFilterLinear;
        default:
            EL_ASSERT(false);
            return MTLSamplerMipFilter(0);
    }
}

MTLSamplerAddressMode asSamplerAddressMode(GraphicsSamplerAddressMode mode)
{
    switch (mode)
    {
        case GraphicsSamplerAddressModeRepeat:
            return MTLSamplerAddressModeRepeat;
        case GraphicsSamplerAddressModeMirroredReapt:
            return MTLSamplerAddressModeMirrorRepeat;
        case GraphicsSamplerAddressModeClampToEdge:
            return MTLSamplerAddressModeClampToEdge;
        case GraphicsSamplerAddressModeClampToBorder:
            return MTLSamplerAddressModeClampToBorderColor;
        case GraphicsSamplerAddressModeMirrorClampToEdge:
            return MTLSamplerAddressModeMirrorClampToEdge;
        default:
            EL_ASSERT(false);
            return MTLSamplerAddressMode(0);
    }
}

#if EL_PLAT_OSX
MTLSamplerBorderColor asSamplerBorderColor(GraphicsBorderColor color)
{
    switch (color)
    {
        case GraphicsBorderColorFloatTransparentBlack:
            return MTLSamplerBorderColorTransparentBlack;  // {0,0,0,0}
        case GraphicsBorderColorFloatOpaqueBlack:
            return MTLSamplerBorderColorOpaqueBlack; // {0,0,0,1}
        case GraphicsBorderColorFloatOpaqueWhite:
            return MTLSamplerBorderColorOpaqueWhite; // {1,1,1,1}
            
        case GraphicsBorderColorIntTransparentBlack:
        case GraphicsBorderColorIntOpaqueBlack:
        case GraphicsBorderColorIntOpaqueWhite:
        default:
            EL_ASSERT(false);
            return MTLSamplerBorderColor(0);
    }
}
#endif

MTLCompareFunction asCompareFunction(GraphicsCompareOp func)
{
    switch (func)
    {
        case GraphicsCompareOp::GraphicsCompareOpNever:
            return MTLCompareFunctionNever;
        case GraphicsCompareOp::GraphicsCompareOpLess:
            return MTLCompareFunctionLess;
        case GraphicsCompareOp::GraphicsCompareOpEqual:
            return MTLCompareFunctionEqual;
        case GraphicsCompareOp::GraphicsCompareOpLessOrEqual:
            return MTLCompareFunctionLessEqual;
        case GraphicsCompareOp::GraphicsCompareOpGreater:
            return MTLCompareFunctionGreater;
        case GraphicsCompareOp::GraphicsCompareOpNotEqual:
            return MTLCompareFunctionNotEqual;
        case GraphicsCompareOp::GraphicsCompareOpGreaterOrEqual:
            return MTLCompareFunctionGreaterEqual;
        case GraphicsCompareOp::GraphicsCompareOpAlways:
            return MTLCompareFunctionAlways;
        default:
            EL_ASSERT(false);
            return MTLCompareFunction(0);
    }
}

MTLStencilOperation asStencilOperation(GraphicsStencilOp func)
{
    switch (func)
    {
        case GraphicsStencilOp::GraphicsStencilOpKeep:
            return MTLStencilOperationKeep;
        case GraphicsStencilOp::GraphicsStencilOpZero:
            return MTLStencilOperationZero;
        case GraphicsStencilOp::GraphicsStencilOpReplace:
            return MTLStencilOperationReplace;
        case GraphicsStencilOp::GraphicsStencilOpIncrementAndClamp:
            return MTLStencilOperationIncrementClamp;
        case GraphicsStencilOp::GraphicsStencilOpDecrementAndClamp:
            return MTLStencilOperationDecrementClamp;
        case GraphicsStencilOp::GraphicsStencilOpInvert:
            return MTLStencilOperationInvert;
        case GraphicsStencilOp::GraphicsStencilOpIncrementAndWrap:
            return MTLStencilOperationIncrementWrap;
        case GraphicsStencilOp::GraphicsStencilOpDecrementAndWrap:
            return MTLStencilOperationDecrementWrap;
        default:
            EL_ASSERT(false);
            return MTLStencilOperation(0);
    }
}

MTLVertexFormat asMetalVertexFormat(GraphicsVertexFormat format)
{
    switch (format)
    {
        case GraphicsVertexFormat::UChar2:
            return MTLVertexFormatUChar2;
        case GraphicsVertexFormat::UChar3:
            return MTLVertexFormatUChar3;
        case GraphicsVertexFormat::UChar4:
            return MTLVertexFormatUChar4;
        case GraphicsVertexFormat::Char2:
            return MTLVertexFormatChar2;
        case GraphicsVertexFormat::Char3:
            return MTLVertexFormatChar3;
        case GraphicsVertexFormat::Char4:
            return MTLVertexFormatChar4;
        case GraphicsVertexFormat::UChar2Normalized:
            return MTLVertexFormatUChar2Normalized;
        case GraphicsVertexFormat::UChar3Normalized:
            return MTLVertexFormatUChar3Normalized;
        case GraphicsVertexFormat::UChar4Normalized:
            return MTLVertexFormatUChar4Normalized;
        case GraphicsVertexFormat::Char2Normalized:
            return MTLVertexFormatChar2Normalized;
        case GraphicsVertexFormat::Char3Normalized:
            return MTLVertexFormatChar3Normalized;
        case GraphicsVertexFormat::Char4Normalized:
            return MTLVertexFormatChar4Normalized;
        case GraphicsVertexFormat::UShort2:
            return MTLVertexFormatUShort2;
        case GraphicsVertexFormat::UShort3:
            return MTLVertexFormatUShort3;
        case GraphicsVertexFormat::UShort4:
            return MTLVertexFormatUShort4;
            
        case GraphicsVertexFormat::Short2:
            return MTLVertexFormatShort2;
        case GraphicsVertexFormat::Short3:
            return MTLVertexFormatShort3;
        case GraphicsVertexFormat::Short4:
            return MTLVertexFormatShort4;
            
        case GraphicsVertexFormat::UShort2Normalized:
            return MTLVertexFormatUShort2Normalized;
        case GraphicsVertexFormat::UShort3Normalized:
            return MTLVertexFormatUShort3Normalized;
        case GraphicsVertexFormat::UShort4Normalized:
            return MTLVertexFormatUShort4Normalized;
            
        case GraphicsVertexFormat::Short2Normalized:
            return MTLVertexFormatShort2Normalized;
        case GraphicsVertexFormat::Short3Normalized:
            return MTLVertexFormatShort3Normalized;
        case GraphicsVertexFormat::Short4Normalized:
            return MTLVertexFormatShort4Normalized;
            
        case GraphicsVertexFormat::Half2:
            return MTLVertexFormatHalf2;
        case GraphicsVertexFormat::Half3:
            return MTLVertexFormatHalf3;
        case GraphicsVertexFormat::Half4:
            return MTLVertexFormatHalf4;
            
        case GraphicsVertexFormat::Float:
            return MTLVertexFormatFloat;
        case GraphicsVertexFormat::Float2:
            return MTLVertexFormatFloat2;
        case GraphicsVertexFormat::Float3:
            return MTLVertexFormatFloat3;
        case GraphicsVertexFormat::Float4:
            return MTLVertexFormatFloat4;
            
        case GraphicsVertexFormat::Int:
            return MTLVertexFormatInt;
        case GraphicsVertexFormat::Int2:
            return MTLVertexFormatInt2;
        case GraphicsVertexFormat::Int3:
            return MTLVertexFormatInt3;
        case GraphicsVertexFormat::Int4:
            return MTLVertexFormatInt4;
            
        case GraphicsVertexFormat::UInt:
            return MTLVertexFormatUInt;
        case GraphicsVertexFormat::UInt2:
            return MTLVertexFormatUInt2;
        case GraphicsVertexFormat::UInt3:
            return MTLVertexFormatUInt3;
        case GraphicsVertexFormat::UInt4:
            return MTLVertexFormatUInt4;
            
        case GraphicsVertexFormat::Int1010102Normalized:
            return MTLVertexFormatInt1010102Normalized;
        case GraphicsVertexFormat::UInt1010102Normalized:
            return MTLVertexFormatUInt1010102Normalized;
        default:
            EL_ASSERT(false);
            return MTLVertexFormat(0);
    }
}

MTLVertexStepFunction asMetalVertexStepFunction(GraphicsInputRate func)
{
    switch (func)
    {
        case GraphicsInputRateVertex:
            return MTLVertexStepFunctionPerVertex;
        case GraphicsInputRateInstance:
            return MTLVertexStepFunctionPerInstance;
        default:
            EL_ASSERT(false);
            return MTLVertexStepFunction(0);
    }
}

GraphicsPixelFormat asGraphicsPixelFormat(MTLPixelFormat format)
{
    switch (format)
    {
    case MTLPixelFormatInvalid:
    case MTLPixelFormatA8Unorm:
    case MTLPixelFormatR8Unorm:
    case MTLPixelFormatR8Snorm:
    case MTLPixelFormatR8Uint:
    case MTLPixelFormatR8Sint:
    case MTLPixelFormatR16Unorm:
    case MTLPixelFormatR16Snorm:
    case MTLPixelFormatR16Uint:
    case MTLPixelFormatR16Sint:
    case MTLPixelFormatR16Float:
    case MTLPixelFormatRG8Unorm:
    case MTLPixelFormatRG8Snorm:
    case MTLPixelFormatRG8Uint:
    case MTLPixelFormatRG8Sint:
    case MTLPixelFormatR32Uint:
    case MTLPixelFormatR32Sint:
    case MTLPixelFormatR32Float:
    case MTLPixelFormatRG16Unorm:
    case MTLPixelFormatRG16Snorm:
    case MTLPixelFormatRG16Uint:
    case MTLPixelFormatRG16Sint:
    case MTLPixelFormatRG16Float:
    case MTLPixelFormatRGBA8Unorm:
    case MTLPixelFormatRGBA8Unorm_sRGB:
    case MTLPixelFormatRGBA8Snorm:
    case MTLPixelFormatRGBA8Uint:
    case MTLPixelFormatRGBA8Sint:
        break;
    case MTLPixelFormatBGRA8Unorm:
        return GraphicsPixelFormatBGRA8Unorm;
    case MTLPixelFormatBGRA8Unorm_sRGB:
    case MTLPixelFormatRGB10A2Unorm:
    case MTLPixelFormatRGB10A2Uint:
    case MTLPixelFormatRG11B10Float:
    case MTLPixelFormatRGB9E5Float:
    case MTLPixelFormatRG32Uint:
    case MTLPixelFormatRG32Sint:
    case MTLPixelFormatRG32Float:
    case MTLPixelFormatRGBA16Unorm:
    case MTLPixelFormatRGBA16Snorm:
    case MTLPixelFormatRGBA16Uint:
    case MTLPixelFormatRGBA16Sint:
    case MTLPixelFormatRGBA16Float:
    case MTLPixelFormatRGBA32Uint:
    case MTLPixelFormatRGBA32Sint:
    case MTLPixelFormatRGBA32Float:
    case MTLPixelFormatBC1_RGBA:
    case MTLPixelFormatBC1_RGBA_sRGB:
    case MTLPixelFormatBC2_RGBA:
    case MTLPixelFormatBC2_RGBA_sRGB:
    case MTLPixelFormatBC3_RGBA:
    case MTLPixelFormatBC3_RGBA_sRGB:
    case MTLPixelFormatBC4_RUnorm:
    case MTLPixelFormatBC4_RSnorm:
    case MTLPixelFormatBC5_RGUnorm:
    case MTLPixelFormatBC5_RGSnorm:
    case MTLPixelFormatBC6H_RGBFloat:
    case MTLPixelFormatBC6H_RGBUfloat:
    case MTLPixelFormatBC7_RGBAUnorm:
    case MTLPixelFormatBC7_RGBAUnorm_sRGB:
    case MTLPixelFormatGBGR422:
    case MTLPixelFormatBGRG422:
    case MTLPixelFormatDepth16Unorm:
    case MTLPixelFormatDepth32Float:
    case MTLPixelFormatStencil8:
    case MTLPixelFormatDepth24Unorm_Stencil8:
    case MTLPixelFormatDepth32Float_Stencil8:
    case MTLPixelFormatX32_Stencil8:
    case MTLPixelFormatX24_Stencil8:
 #if EL_PLAT_IOS
    case MTLPixelFormatR8Unorm_sRGB:
    case MTLPixelFormatRG8Unorm_sRGB:
    case MTLPixelFormatB5G6R5Unorm:
    case MTLPixelFormatA1BGR5Unorm:
    case MTLPixelFormatABGR4Unorm:
    case MTLPixelFormatBGR5A1Unorm:
    case MTLPixelFormatBGR10_XR:
    case MTLPixelFormatBGR10_XR_sRGB:
    case MTLPixelFormatBGRA10_XR:
    case MTLPixelFormatBGRA10_XR_sRGB:
    case MTLPixelFormatPVRTC_RGB_2BPP:
    case MTLPixelFormatPVRTC_RGB_2BPP_sRGB:
    case MTLPixelFormatPVRTC_RGB_4BPP:
    case MTLPixelFormatPVRTC_RGB_4BPP_sRGB:
    case MTLPixelFormatPVRTC_RGBA_2BPP:
    case MTLPixelFormatPVRTC_RGBA_2BPP_sRGB:
    case MTLPixelFormatPVRTC_RGBA_4BPP:
    case MTLPixelFormatPVRTC_RGBA_4BPP_sRGB:
    case MTLPixelFormatEAC_R11Unorm:
    case MTLPixelFormatEAC_R11Snorm:
    case MTLPixelFormatEAC_RG11Unorm:
    case MTLPixelFormatEAC_RG11Snorm:
    case MTLPixelFormatEAC_RGBA8:
    case MTLPixelFormatEAC_RGBA8_sRGB:
    case MTLPixelFormatETC2_RGB8:
    case MTLPixelFormatETC2_RGB8_sRGB:
    case MTLPixelFormatETC2_RGB8A1:
    case MTLPixelFormatETC2_RGB8A1_sRGB:
    case MTLPixelFormatASTC_4x4_sRGB:
    case MTLPixelFormatASTC_5x4_sRGB:
    case MTLPixelFormatASTC_5x5_sRGB:
    case MTLPixelFormatASTC_6x5_sRGB:
    case MTLPixelFormatASTC_6x6_sRGB:
    case MTLPixelFormatASTC_8x5_sRGB:
    case MTLPixelFormatASTC_8x6_sRGB:
    case MTLPixelFormatASTC_8x8_sRGB:
    case MTLPixelFormatASTC_10x5_sRGB:
    case MTLPixelFormatASTC_10x6_sRGB:
    case MTLPixelFormatASTC_10x8_sRGB:
    case MTLPixelFormatASTC_10x10_sRGB:
    case MTLPixelFormatASTC_12x10_sRGB:
    case MTLPixelFormatASTC_12x12_sRGB:
    case MTLPixelFormatASTC_4x4_LDR:
    case MTLPixelFormatASTC_5x4_LDR:
    case MTLPixelFormatASTC_5x5_LDR:
    case MTLPixelFormatASTC_6x5_LDR:
    case MTLPixelFormatASTC_6x6_LDR:
    case MTLPixelFormatASTC_8x5_LDR:
    case MTLPixelFormatASTC_8x6_LDR:
    case MTLPixelFormatASTC_8x8_LDR:
    case MTLPixelFormatASTC_10x5_LDR:
    case MTLPixelFormatASTC_10x6_LDR:
    case MTLPixelFormatASTC_10x8_LDR:
    case MTLPixelFormatASTC_10x10_LDR:
    case MTLPixelFormatASTC_12x10_LDR:
    case MTLPixelFormatASTC_12x12_LDR:
#endif // EL_PLAT_IOS
    default:
        EL_ASSERT(false);
        return GraphicsPixelFormatInvalid;
    }
    EL_ASSERT(false);
    return GraphicsPixelFormatInvalid;
}

MTLPrimitiveType asMetalPrimitiveType(GraphicsPrimitiveType type)
{
    switch (type)
    {
        case GraphicsPrimitiveTypeTriangle:
            return MTLPrimitiveTypeTriangle;
        case GraphicsPrimitiveTypeFan:
        default:
            EL_ASSERT(false);
            return MTLPrimitiveType(0);
    }
}

_EL_NAME_END
