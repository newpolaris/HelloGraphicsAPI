#ifndef __METAL_CONTEXT_H
#define __METAL_CONTEXT_H

#include <graphics_types.h>
#include <mtlpp.hpp>
#include "metal_types.h"

namespace el {

    struct MetalContext
    {
        mtlpp::Device device;
        mtlpp::CommandQueue commandQueue;
        mtlpp::CommandBuffer currentCommandBuffer;
        mtlpp::RenderCommandEncoder renderCommandEncoder;
        mtlpp::PixelFormat currentSurfacePixelFormat;
        mtlpp::PixelFormat currentDepthPixelFormat;
        mtlpp::Drawable currentDrawable;
        mtlpp::Texture currentSurfaceTexture;
        MetalSurfacePtr currentSurface;
    };

    el::GraphicsPixelFormat getSurfacePixelFormat(MetalContext* context);
    mtlpp::Texture aquireSurfaceTexture(MetalContext* context);
    void setLayerDevice(MetalContext* context, NativeSurface surface);

} // namespace el {

#endif // __METAL_CONTEXT_H
