#include "msl_shader.h"

#include "debug.h"
#include "mtlpp.hpp"
#include "graphics_types.h"

namespace el
{ 
	mtlpp::PixelFormat asPixelForamt(el::GraphicsPixelFormat format)
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
}

#if EL_PLAT_APPLE

using namespace el;

MSLShader::MSLShader()
{
}

MSLShader::~MSLShader()
{
}

bool MSLShader::create(GraphicsShaderStageFlagBits stage, const char* shaderCode)
{
    mtlpp::Device device = mtlpp::Device::CreateSystemDefaultDevice();

    _library = device.NewLibrary(shaderCode, mtlpp::CompileOptions(), nullptr);
    assert(_library);

    _function = _library.NewFunction("vertFunc");
    assert(_function);
    return true;
}

void MSLShader::destroy()
{
}

const GraphicsShaderDesc& MSLShader::getDesc() const
{
    return _desc;
}

#endif // #if EL_PLAT_APPLE
