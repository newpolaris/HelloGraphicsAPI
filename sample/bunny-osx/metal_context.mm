#include "metal_context.h"
#include <el_platform.h>
#include <el_debug.h>
#include <QuartzCore/CAMetalLayer.h>
#include <Metal/Metal.h>
#include <Metal/mtl_types.h>

_EL_NAME_BEGIN

el::GraphicsPixelFormat getSwapchainPixelFormat(SwapchainHandle handle)
{
    EL_ASSERT(handle);
    CAMetalLayer* layer = reinterpret_cast<CAMetalLayer*>(handle);
    EL_ASSERT(layer);
    return el::asGraphicsPixelFormat((mtlpp::PixelFormat)layer.pixelFormat);
}

mtlpp::Drawable aquireCurrentDrawable(SwapchainHandle handle)
{
    EL_ASSERT(handle);
    CAMetalLayer* layer = reinterpret_cast<CAMetalLayer*>(handle);
    EL_ASSERT(layer);
    auto drawable = [layer nextDrawable];
    return mtlpp::Drawable(ns::Handle{(__bridge void*)drawable});
}

mtlpp::Texture getDrawableTexture(const mtlpp::Drawable& drawable)
{
    auto nativeDrawable = (__bridge id<CAMetalDrawable>)drawable.GetPtr();
    EL_ASSERT(nativeDrawable);
    return mtlpp::Texture(ns::Handle{(__bridge void*)nativeDrawable.texture});
}
    
_EL_NAME_END
