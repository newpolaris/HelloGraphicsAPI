#ifndef __METAL_DRIVER_H__
#define __METAL_DRIVER_H__

#include <graphics_types.h>

namespace el {

    typedef void *SwapchainHandle;

    struct MetalRenderPipeline
    {
        ns::String label;
        mtlpp::Function vertexFunction;
        mtlpp::Function fragmentFunction;
    };

    class MetalDriver
    {
    public:

        MetalDriver();

        void makeCurrent(SwapchainHandle handle);
        void beginRenderPass();
        void endRenderPass();
        void commit();

        std::unique_ptr<class MetalContext> _context;
    };

} // namespace el {

#endif // __METAL_DRIVER_H__