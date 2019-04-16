#include "gl_types.h"
#include "debug.h"

bool el::isSamplerType(GLenum type)
{
    switch (type)
    {
    case GL_SAMPLER_2D:
    case GL_SAMPLER_3D:
    case GL_SAMPLER_CUBE:
    case GL_SAMPLER_2D_ARRAY:
        return true;
    }
    return false;
}

GLuint el::asVariableComponentCount(GLenum type)
{
    return asVariableColumnCount(type) * asVariableRowCount(type);
}

GLuint el::asVariableRowCount(GLenum type)
{
    switch (type)
    {
    case GL_BOOL:
    case GL_FLOAT:
    case GL_INT:
    case GL_SAMPLER_2D:
    case GL_SAMPLER_CUBE:
    case GL_SAMPLER_2D_ARRAY:
    case GL_BOOL_VEC2:
    case GL_FLOAT_VEC2:
    case GL_INT_VEC2:
    case GL_INT_VEC3:
    case GL_FLOAT_VEC3:
    case GL_BOOL_VEC3:
    case GL_BOOL_VEC4:
    case GL_FLOAT_VEC4:
    case GL_INT_VEC4:
        return 1;
    case GL_FLOAT_MAT2:
    case GL_FLOAT_MAT3x2:
    case GL_FLOAT_MAT4x2:
        return 2;
    case GL_FLOAT_MAT3:
    case GL_FLOAT_MAT2x3:
    case GL_FLOAT_MAT4x3:
        return 3;
    case GL_FLOAT_MAT4:
    case GL_FLOAT_MAT2x4:
    case GL_FLOAT_MAT3x4:
        return 4;
    default:
        EL_ASSERT(FALSE);
        return 0;
    }
}

GLuint el::asVariableColumnCount(GLenum type)
{
    switch (type)
    {
    case GL_BOOL:
    case GL_FLOAT:
    case GL_INT:
    case GL_SAMPLER_2D:
    case GL_SAMPLER_CUBE:
    case GL_SAMPLER_2D_ARRAY:
        return 1;
    case GL_BOOL_VEC2:
    case GL_FLOAT_VEC2:
    case GL_INT_VEC2:
    case GL_FLOAT_MAT2:
    case GL_FLOAT_MAT2x3:
    case GL_FLOAT_MAT2x4:
        return 2;
    case GL_BOOL_VEC3:
    case GL_FLOAT_VEC3:
    case GL_INT_VEC3:
    case GL_FLOAT_MAT3:
    case GL_FLOAT_MAT3x2:
    case GL_FLOAT_MAT3x4:
        return 3;
    case GL_BOOL_VEC4:
    case GL_FLOAT_VEC4:
    case GL_INT_VEC4:
    case GL_FLOAT_MAT4:
    case GL_FLOAT_MAT4x2:
    case GL_FLOAT_MAT4x3:
        return 4;
    default:
        EL_ASSERT(FALSE);
        return 0;
    }
}

GLuint el::asVariableComponentSize(GLenum type)
{
    switch (type)
    {
    case GL_BOOL: return sizeof(GLint);
    case GL_FLOAT: return sizeof(GLfloat);
    case GL_INT: return sizeof(GLint);
    case GL_UNSIGNED_INT: return sizeof(GLuint);
    default: return 0;
    }
}

GLuint el::asVariableInternalSize(GLenum type)
{
    // Expanded to 4-element vectors
    return asVariableComponentSize(asVariableComponentType(type)) * asVariableRowCount(type) * 4;
}

GLuint el::asVariableExternalSize(GLenum type)
{
    return asVariableComponentSize(asVariableComponentType(type)) * asVariableComponentCount(type);
}

GLenum el::asVariableComponentType(GLenum type)
{
    switch (type)
    {
    case GL_BOOL:
    case GL_BOOL_VEC2:
    case GL_BOOL_VEC3:
    case GL_BOOL_VEC4:
        return GL_BOOL;
    case GL_FLOAT:
    case GL_FLOAT_VEC2:
    case GL_FLOAT_VEC3:
    case GL_FLOAT_VEC4:
    case GL_FLOAT_MAT2:
    case GL_FLOAT_MAT3:
    case GL_FLOAT_MAT4:
        return GL_FLOAT;
    case GL_INT:
    case GL_SAMPLER_2D:
    case GL_SAMPLER_2D_ARRAY:
    case GL_SAMPLER_CUBE:
    case GL_INT_VEC2:
    case GL_INT_VEC3:
    case GL_INT_VEC4:
        return GL_INT;
    default:
        EL_ASSERT(FALSE);
        return GL_NONE;
    }
}

std::string el::asVariableComponentTypename(GLenum type)
{
    switch(type)
    {
    case GL_FLOAT: return "float";
    case GL_FLOAT_VEC2: return "vec2";
    case GL_FLOAT_VEC3: return "vec3";
    case GL_FLOAT_VEC4: return "vec4";
    case GL_INT: return "int";
    case GL_INT_VEC2: return "ivec2";
    case GL_INT_VEC3: return "ivec3";
    case GL_INT_VEC4: return "ivec4";
    case GL_UNSIGNED_INT: return "unsigned int";
    case GL_UNSIGNED_INT_VEC2: return "uvec2";
    case GL_UNSIGNED_INT_VEC3: return "uvec3";
    case GL_UNSIGNED_INT_VEC4: return "uvec4";
    case GL_BOOL: return "bool";
    case GL_BOOL_VEC2: return "bvec2";
    case GL_BOOL_VEC3: return "bvec3";
    case GL_BOOL_VEC4: return "bvec4";
    case GL_FLOAT_MAT2: return "mat2";
    case GL_FLOAT_MAT3: return "mat3";
    case GL_FLOAT_MAT4: return "mat4";
    case GL_FLOAT_MAT2x3: return "mat2x3";
    case GL_FLOAT_MAT2x4: return "mat2x4";
    case GL_FLOAT_MAT3x2: return "mat3x2";
    case GL_FLOAT_MAT3x4: return "mat3x4";
    case GL_FLOAT_MAT4x2: return "mat4x2";
    case GL_FLOAT_MAT4x3: return "mat4x3";
    case GL_SAMPLER_2D: return "sampler2D";
    case GL_SAMPLER_3D: return "sampler3D";
    case GL_SAMPLER_CUBE: return "samplerCube";
    default: EL_ASSERT(false); return "";
    }
}

GLenum el::getShaderStage(GraphicsShaderStageFlagBits stage)
{
    switch (stage) {
    case GraphicsShaderStageVertexBit:
        return GL_VERTEX_SHADER;
    case GraphicsShaderStageFragmentBit:
        return GL_FRAGMENT_SHADER;
    default:
        EL_ASSERT(false);
    }
    return 0;
}

GLenum el::asTextureTarget(GraphicsTextureDim dim)
{
    GLenum target = 0;
    switch (dim)
    {
    case GraphicsTextureDim1D:			target = GL_TEXTURE_1D; break;
    case GraphicsTextureDim1DArray:		target = GL_TEXTURE_1D_ARRAY; break;
    case GraphicsTextureDim2D:			target = GL_TEXTURE_2D; break;
    case GraphicsTextureDim2DArray:		target = GL_TEXTURE_2D_ARRAY; break;
    case GraphicsTextureDim3D:			target = GL_TEXTURE_3D; break;
    case GraphicsTextureDimRect:		target = 0; break;
    case GraphicsTextureDimRectArray:	target = 0; break;
    case GraphicsTextureDimCube:		target = GL_TEXTURE_CUBE_MAP; break;
    case GraphicsTextureDimCubeArray:	target = 0; break;
    default:							target = 0; break;
    }
    EL_ASSERT(target != 0);
    return target;
}

GLenum el::asTextureType(GraphicsPixelFormat format)
{
    GLenum textureType = 0;
    switch (format)
    {
    case GraphicsPixelFormat::GraphicsPixelFormatA8Unorm:				textureType = GL_UNSIGNED_BYTE; break;
    case GraphicsPixelFormat::GraphicsPixelFormatR8Unorm:				textureType = GL_UNSIGNED_BYTE; break;
    case GraphicsPixelFormat::GraphicsPixelFormatR8Unorm_sRGB:			textureType = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatR8Snorm:				textureType = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatR8Uint:				textureType = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatR8Sint:				textureType = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatR16Unorm:				textureType = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatR16Snorm:			textureType = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatR16Uint:			textureType = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatR16Sint:			textureType = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatR16Float:			textureType = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatRG8Unorm:			textureType = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatRG8Unorm_sRGB:			textureType = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatRG8Snorm:			textureType = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatRG8Uint:			textureType = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatRG8Sint:			textureType = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatB5G6R5Unorm:			textureType = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatA1BGR5Unorm:			textureType = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatABGR4Unorm:			textureType = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatBGR5A1Unorm:			textureType = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatR32Uint:			textureType = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatR32Sint:			textureType = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatR32Float:			textureType = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatRG16Unorm:			textureType = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatRG16Snorm:			textureType = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatRG16Uint:			textureType = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatRG16Sint:			textureType = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatRG16Float:			textureType = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatRGBA8Unorm:			textureType = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatRGBA8Unorm_sRGB:			textureType = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatRGBA8Snorm:			textureType = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatRGBA8Uint:			textureType = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatRGBA8Sint:			textureType = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatBGRA8Unorm:			textureType = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatBGRA8Unorm_sRGB:			textureType = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatRGB10A2Unorm:			textureType = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatRGB10A2Uint:			textureType = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatRG11B10Float:			textureType = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatRGB9E5Float:			textureType = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatBGR10_XR:			textureType = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatBGR10_XR_sRGB:			textureType = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatRG32Uint:			textureType = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatRG32Sint:			textureType = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatRG32Float:			textureType = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatRGBA16Unorm:			textureType = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatRGBA16Snorm:			textureType = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatRGBA16Uint:			textureType = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatRGBA16Sint:			textureType = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatRGBA16Float:			textureType = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatBGRA10_XR:			textureType = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatBGRA10_XR_sRGB:			textureType = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatRGBA32Uint:			textureType = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatRGBA32Sint:			textureType = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatRGBA32Float:			textureType = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatBC1_RGBA:			textureType = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatBC1_RGBA_sRGB:			textureType = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatBC2_RGBA:			textureType = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatBC2_RGBA_sRGB:			textureType = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatBC3_RGBA:			textureType = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatBC3_RGBA_sRGB:			textureType = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatBC4_RUnorm:			textureType = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatBC4_RSnorm:			textureType = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatBC5_RGUnorm:			textureType = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatBC5_RGSnorm:			textureType = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatBC6H_RGBFloat:			textureType = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatBC6H_RGBUfloat:			textureType = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatBC7_RGBAUnorm:			textureType = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatBC7_RGBAUnorm_sRGB:			textureType = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatPVRTC_RGB_2BPP:			textureType = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatPVRTC_RGB_2BPP_sRGB:			textureType = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatPVRTC_RGB_4BPP:			textureType = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatPVRTC_RGB_4BPP_sRGB:			textureType = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatPVRTC_RGBA_2BPP:			textureType = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatPVRTC_RGBA_2BPP_sRGB:			textureType = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatPVRTC_RGBA_4BPP:			textureType = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatPVRTC_RGBA_4BPP_sRGB:			textureType = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatEAC_R11Unorm:			textureType = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatEAC_R11Snorm:			textureType = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatEAC_RG11Unorm:			textureType = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatEAC_RG11Snorm:			textureType = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatEAC_RGBA8:			textureType = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatEAC_RGBA8_sRGB:			textureType = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatETC2_RGB8:			textureType = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatETC2_RGB8_sRGB:			textureType = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatETC2_RGB8A1:			textureType = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatETC2_RGB8A1_sRGB:			textureType = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatASTC_4x4_sRGB:			textureType = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatASTC_5x4_sRGB:			textureType = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatASTC_5x5_sRGB:			textureType = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatASTC_6x5_sRGB:			textureType = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatASTC_6x6_sRGB:			textureType = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatASTC_8x5_sRGB:			textureType = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatASTC_8x6_sRGB:			textureType = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatASTC_8x8_sRGB:			textureType = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatASTC_10x5_sRGB:			textureType = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatASTC_10x6_sRGB:			textureType = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatASTC_10x8_sRGB:			textureType = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatASTC_10x10_sRGB:			textureType = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatASTC_12x10_sRGB:			textureType = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatASTC_12x12_sRGB:			textureType = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatASTC_4x4_LDR:			textureType = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatASTC_5x4_LDR:			textureType = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatASTC_5x5_LDR:			textureType = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatASTC_6x5_LDR:			textureType = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatASTC_6x6_LDR:			textureType = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatASTC_8x5_LDR:			textureType = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatASTC_8x6_LDR:			textureType = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatASTC_8x8_LDR:			textureType = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatASTC_10x5_LDR:			textureType = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatASTC_10x6_LDR:			textureType = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatASTC_10x8_LDR:			textureType = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatASTC_10x10_LDR:			textureType = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatASTC_12x10_LDR:			textureType = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatASTC_12x12_LDR:			textureType = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatGBGR422:			textureType = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatBGRG422:			textureType = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatDepth16Unorm:			textureType = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatDepth32Float:			textureType = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatStencil8:			textureType = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatDepth24Unorm_Stencil8:			textureType = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatDepth32Float_Stencil8:			textureType = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatX32_Stencil8:			textureType = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatX24_Stencil8:			textureType = 0; break;
    default: textureType = 0; break;
    }
    EL_ASSERT(textureType != 0);
    return textureType;
}

GLenum el::asTextureFormat(GraphicsPixelFormat textureFormat)
{
    GLenum format = 0;
    switch (textureFormat)
    {
    case GraphicsPixelFormat::GraphicsPixelFormatA8Unorm:				format = GL_ALPHA; break;
    case GraphicsPixelFormat::GraphicsPixelFormatR8Unorm:				format = GL_RED; break;
    case GraphicsPixelFormat::GraphicsPixelFormatR8Unorm_sRGB:			format = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatR8Snorm:				format = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatR8Uint:				format = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatR8Sint:				format = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatR16Unorm:				format = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatR16Snorm:				format = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatR16Uint:				format = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatR16Sint:				format = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatR16Float:				format = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatRG8Unorm:				format = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatRG8Unorm_sRGB:			format = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatRG8Snorm:			format = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatRG8Uint:			format = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatRG8Sint:			format = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatB5G6R5Unorm:			format = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatA1BGR5Unorm:			format = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatABGR4Unorm:			format = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatBGR5A1Unorm:			format = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatR32Uint:			format = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatR32Sint:			format = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatR32Float:			format = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatRG16Unorm:			format = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatRG16Snorm:			format = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatRG16Uint:			format = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatRG16Sint:			format = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatRG16Float:			format = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatRGBA8Unorm:			format = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatRGBA8Unorm_sRGB:			format = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatRGBA8Snorm:			format = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatRGBA8Uint:			format = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatRGBA8Sint:			format = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatBGRA8Unorm:			format = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatBGRA8Unorm_sRGB:			format = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatRGB10A2Unorm:			format = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatRGB10A2Uint:			format = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatRG11B10Float:			format = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatRGB9E5Float:			format = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatBGR10_XR:			format = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatBGR10_XR_sRGB:			format = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatRG32Uint:			format = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatRG32Sint:			format = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatRG32Float:			format = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatRGBA16Unorm:			format = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatRGBA16Snorm:			format = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatRGBA16Uint:			format = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatRGBA16Sint:			format = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatRGBA16Float:			format = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatBGRA10_XR:			format = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatBGRA10_XR_sRGB:			format = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatRGBA32Uint:			format = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatRGBA32Sint:			format = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatRGBA32Float:			format = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatBC1_RGBA:			format = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatBC1_RGBA_sRGB:			format = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatBC2_RGBA:			format = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatBC2_RGBA_sRGB:			format = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatBC3_RGBA:			format = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatBC3_RGBA_sRGB:			format = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatBC4_RUnorm:			format = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatBC4_RSnorm:			format = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatBC5_RGUnorm:			format = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatBC5_RGSnorm:			format = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatBC6H_RGBFloat:			format = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatBC6H_RGBUfloat:			format = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatBC7_RGBAUnorm:			format = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatBC7_RGBAUnorm_sRGB:			format = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatPVRTC_RGB_2BPP:			format = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatPVRTC_RGB_2BPP_sRGB:			format = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatPVRTC_RGB_4BPP:			format = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatPVRTC_RGB_4BPP_sRGB:			format = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatPVRTC_RGBA_2BPP:			format = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatPVRTC_RGBA_2BPP_sRGB:			format = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatPVRTC_RGBA_4BPP:			format = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatPVRTC_RGBA_4BPP_sRGB:			format = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatEAC_R11Unorm:			format = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatEAC_R11Snorm:			format = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatEAC_RG11Unorm:			format = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatEAC_RG11Snorm:			format = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatEAC_RGBA8:			format = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatEAC_RGBA8_sRGB:			format = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatETC2_RGB8:			format = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatETC2_RGB8_sRGB:			format = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatETC2_RGB8A1:			format = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatETC2_RGB8A1_sRGB:			format = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatASTC_4x4_sRGB:			format = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatASTC_5x4_sRGB:			format = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatASTC_5x5_sRGB:			format = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatASTC_6x5_sRGB:			format = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatASTC_6x6_sRGB:			format = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatASTC_8x5_sRGB:			format = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatASTC_8x6_sRGB:			format = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatASTC_8x8_sRGB:			format = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatASTC_10x5_sRGB:			format = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatASTC_10x6_sRGB:			format = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatASTC_10x8_sRGB:			format = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatASTC_10x10_sRGB:			format = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatASTC_12x10_sRGB:			format = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatASTC_12x12_sRGB:			format = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatASTC_4x4_LDR:			format = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatASTC_5x4_LDR:			format = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatASTC_5x5_LDR:			format = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatASTC_6x5_LDR:			format = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatASTC_6x6_LDR:			format = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatASTC_8x5_LDR:			format = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatASTC_8x6_LDR:			format = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatASTC_8x8_LDR:			format = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatASTC_10x5_LDR:			format = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatASTC_10x6_LDR:			format = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatASTC_10x8_LDR:			format = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatASTC_10x10_LDR:			format = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatASTC_12x10_LDR:			format = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatASTC_12x12_LDR:			format = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatGBGR422:			format = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatBGRG422:			format = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatDepth16Unorm:			format = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatDepth32Float:			format = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatStencil8:			format = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatDepth24Unorm_Stencil8:			format = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatDepth32Float_Stencil8:			format = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatX32_Stencil8:			format = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatX24_Stencil8:			format = 0; break;
    default: format = 0; break;
    }
    EL_ASSERT(format != 0);
    return format;
}

GLint el::asTextureInternalFormat(GraphicsPixelFormat format)
{
    GLint internalFormat = 0;
    switch (format)
    {
    case GraphicsPixelFormat::GraphicsPixelFormatA8Unorm:				internalFormat = GL_ALPHA; break;
    case GraphicsPixelFormat::GraphicsPixelFormatR8Unorm:				internalFormat = GL_LUMINANCE; break;
    case GraphicsPixelFormat::GraphicsPixelFormatR8Unorm_sRGB:			internalFormat = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatR8Snorm:				internalFormat = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatR8Uint:				internalFormat = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatR8Sint:				internalFormat = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatR16Unorm:				internalFormat = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatR16Snorm:			internalFormat = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatR16Uint:			internalFormat = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatR16Sint:			internalFormat = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatR16Float:			internalFormat = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatRG8Unorm:			internalFormat = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatRG8Unorm_sRGB:			internalFormat = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatRG8Snorm:			internalFormat = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatRG8Uint:			internalFormat = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatRG8Sint:			internalFormat = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatB5G6R5Unorm:			internalFormat = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatA1BGR5Unorm:			internalFormat = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatABGR4Unorm:			internalFormat = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatBGR5A1Unorm:			internalFormat = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatR32Uint:			internalFormat = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatR32Sint:			internalFormat = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatR32Float:			internalFormat = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatRG16Unorm:			internalFormat = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatRG16Snorm:			internalFormat = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatRG16Uint:			internalFormat = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatRG16Sint:			internalFormat = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatRG16Float:			internalFormat = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatRGBA8Unorm:			internalFormat = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatRGBA8Unorm_sRGB:			internalFormat = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatRGBA8Snorm:			internalFormat = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatRGBA8Uint:			internalFormat = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatRGBA8Sint:			internalFormat = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatBGRA8Unorm:			internalFormat = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatBGRA8Unorm_sRGB:			internalFormat = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatRGB10A2Unorm:			internalFormat = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatRGB10A2Uint:			internalFormat = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatRG11B10Float:			internalFormat = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatRGB9E5Float:			internalFormat = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatBGR10_XR:			internalFormat = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatBGR10_XR_sRGB:			internalFormat = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatRG32Uint:			internalFormat = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatRG32Sint:			internalFormat = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatRG32Float:			internalFormat = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatRGBA16Unorm:			internalFormat = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatRGBA16Snorm:			internalFormat = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatRGBA16Uint:			internalFormat = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatRGBA16Sint:			internalFormat = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatRGBA16Float:			internalFormat = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatBGRA10_XR:			internalFormat = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatBGRA10_XR_sRGB:			internalFormat = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatRGBA32Uint:			internalFormat = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatRGBA32Sint:			internalFormat = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatRGBA32Float:			internalFormat = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatBC1_RGBA:			internalFormat = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatBC1_RGBA_sRGB:			internalFormat = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatBC2_RGBA:			internalFormat = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatBC2_RGBA_sRGB:			internalFormat = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatBC3_RGBA:			internalFormat = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatBC3_RGBA_sRGB:			internalFormat = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatBC4_RUnorm:			internalFormat = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatBC4_RSnorm:			internalFormat = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatBC5_RGUnorm:			internalFormat = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatBC5_RGSnorm:			internalFormat = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatBC6H_RGBFloat:			internalFormat = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatBC6H_RGBUfloat:			internalFormat = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatBC7_RGBAUnorm:			internalFormat = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatBC7_RGBAUnorm_sRGB:			internalFormat = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatPVRTC_RGB_2BPP:			internalFormat = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatPVRTC_RGB_2BPP_sRGB:			internalFormat = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatPVRTC_RGB_4BPP:			internalFormat = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatPVRTC_RGB_4BPP_sRGB:			internalFormat = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatPVRTC_RGBA_2BPP:			internalFormat = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatPVRTC_RGBA_2BPP_sRGB:			internalFormat = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatPVRTC_RGBA_4BPP:			internalFormat = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatPVRTC_RGBA_4BPP_sRGB:			internalFormat = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatEAC_R11Unorm:			internalFormat = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatEAC_R11Snorm:			internalFormat = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatEAC_RG11Unorm:			internalFormat = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatEAC_RG11Snorm:			internalFormat = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatEAC_RGBA8:			internalFormat = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatEAC_RGBA8_sRGB:			internalFormat = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatETC2_RGB8:			internalFormat = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatETC2_RGB8_sRGB:			internalFormat = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatETC2_RGB8A1:			internalFormat = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatETC2_RGB8A1_sRGB:			internalFormat = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatASTC_4x4_sRGB:			internalFormat = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatASTC_5x4_sRGB:			internalFormat = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatASTC_5x5_sRGB:			internalFormat = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatASTC_6x5_sRGB:			internalFormat = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatASTC_6x6_sRGB:			internalFormat = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatASTC_8x5_sRGB:			internalFormat = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatASTC_8x6_sRGB:			internalFormat = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatASTC_8x8_sRGB:			internalFormat = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatASTC_10x5_sRGB:			internalFormat = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatASTC_10x6_sRGB:			internalFormat = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatASTC_10x8_sRGB:			internalFormat = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatASTC_10x10_sRGB:			internalFormat = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatASTC_12x10_sRGB:			internalFormat = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatASTC_12x12_sRGB:			internalFormat = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatASTC_4x4_LDR:			internalFormat = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatASTC_5x4_LDR:			internalFormat = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatASTC_5x5_LDR:			internalFormat = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatASTC_6x5_LDR:			internalFormat = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatASTC_6x6_LDR:			internalFormat = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatASTC_8x5_LDR:			internalFormat = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatASTC_8x6_LDR:			internalFormat = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatASTC_8x8_LDR:			internalFormat = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatASTC_10x5_LDR:			internalFormat = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatASTC_10x6_LDR:			internalFormat = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatASTC_10x8_LDR:			internalFormat = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatASTC_10x10_LDR:			internalFormat = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatASTC_12x10_LDR:			internalFormat = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatASTC_12x12_LDR:			internalFormat = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatGBGR422:			internalFormat = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatBGRG422:			internalFormat = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatDepth16Unorm:			internalFormat = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatDepth32Float:			internalFormat = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatStencil8:			internalFormat = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatDepth24Unorm_Stencil8:			internalFormat = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatDepth32Float_Stencil8:			internalFormat = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatX32_Stencil8:			internalFormat = 0; break;
    case GraphicsPixelFormat::GraphicsPixelFormatX24_Stencil8:			internalFormat = 0; break;
    default: internalFormat = 0; break;
    }
    EL_ASSERT(internalFormat != 0);
    return internalFormat;
}
