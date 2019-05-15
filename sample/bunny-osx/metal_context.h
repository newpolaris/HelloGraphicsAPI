#ifndef __METAL_CONTEXT_H
#define __METAL_CONTEXT_H

#include <graphics_types.h>
#include <mtlpp.hpp>

namespace el {

    typedef void *SwapchainHandle;

    el::GraphicsPixelFormat getSwapchainPixelFormat(SwapchainHandle handle);
    mtlpp::Drawable aquireCurrentDrawable(SwapchainHandle handle);
    mtlpp::Texture getDrawableTexture(const mtlpp::Drawable& drawable);

    struct MetalContext
    {
        mtlpp::Device device;
        mtlpp::CommandQueue commandQueue;
        mtlpp::CommandBuffer currentCommandBuffer;
        mtlpp::Drawable currentDrawable;
    };

} // namespace el {

#endif // __METAL_CONTEXT_H
