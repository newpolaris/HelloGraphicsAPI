#include "metal_context.h"
#include <el_platform.h>
#include <el_debug.h>
#include <QuartzCore/CAMetalLayer.h>
#include <Metal/Metal.h>
#include <Metal/mtl_types.h>

_EL_NAME_BEGIN

el::GraphicsPixelFormat getSurfacePixelFormat(MetalContext* context)
{
    EL_ASSERT(context);
    EL_ASSERT(context->currentSurface);
    EL_ASSERT(context->currentSurface->layer);
    
    auto layer = reinterpret_cast<CAMetalLayer*>(context->currentSurface->layer);
    return el::asGraphicsPixelFormat((mtlpp::PixelFormat)layer.pixelFormat);
}

mtlpp::Texture aquireSurfaceTexture(MetalContext* context)
{
    EL_ASSERT(context);
    EL_ASSERT(context->currentSurface);
    EL_ASSERT(context->currentSurface->layer);

    if (!context->currentSurfaceTexture)
    {
        auto layer = reinterpret_cast<CAMetalLayer*>(context->currentSurface->layer);
        auto drawable = [layer nextDrawable];
        context->currentDrawable = mtlpp::Drawable(ns::Handle{(__bridge void*)drawable});
        context->currentSurfaceTexture = mtlpp::Texture(ns::Handle{(__bridge void*)drawable.texture});
    }
    if (!context->currentSurfaceTexture)
    {
        context = nullptr;
    }
    return context->currentSurfaceTexture;
}

void setLayerDevice(MetalContext* context, NativeSurface surface)
{
    EL_ASSERT(context);
    EL_ASSERT(context->device);
    
    auto layer = reinterpret_cast<CAMetalLayer*>(surface);
    layer.device = (__bridge id<MTLDevice>)context->device.GetPtr();
}
    
_EL_NAME_END
