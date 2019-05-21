#ifndef __METLA_TYPES_H__
#define __METLA_TYPES_H__

#include <vector>
#include <Metal/Metal.h>
#include <QuartzCore/CAMetalLayer.h>
#include <el_debug.h>
#include <el_platform.h>
#include <el_math.h>
#include <graphics_types.h>

namespace el 
{
    struct MetalDriver;
    struct MetalContext;

    typedef MTLPixelFormat MetalPixelFormat;
    typedef std::vector<MetalPixelFormat> MetalPixelFormats;

    typedef std::shared_ptr<struct MetalProgram> MetalProgramPtr;
    typedef std::shared_ptr<struct MetalRenderTarget> MetalRenderTargetPtr;
    typedef std::shared_ptr<struct MetalBuffer> MetalBufferPtr;
    typedef std::shared_ptr<struct MetalTexture> MetalTexturePtr;
    
    struct PipelineState
    {
        MetalProgramPtr program;
    };
    
    MTLClearColor asMTLClearColor(const math::float4 &color);
    MTLLoadAction asLoadAction(const RenderPassParms& params, GraphicsTargetBufferFlagBits bit);
    MTLStoreAction asStoreAction(const RenderPassParms& params, GraphicsTargetBufferFlagBits bit);
    
    MTLPixelFormat asMetalPixelFormat(GraphicsPixelFormat format);
    MTLTextureUsage asMetalTextureUsage(GraphicsTextureUsageFlags flags);
    MTLResourceOptions asMetalTextureResourceOptions(GraphicsTextureUsageFlags flags);
}

#endif // __METLA_TYPES_H__
