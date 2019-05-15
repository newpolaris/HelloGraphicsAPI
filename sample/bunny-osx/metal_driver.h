#ifndef __METAL_DRIVER_H__
#define __METAL_DRIVER_H__

#include <graphics_types.h>
#include <mtlpp.hpp>

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

        mtlpp::Device& getDevice();
        mtlpp::CommandBuffer& getCurrentCommandBuffer();

        void makeCurrent(SwapchainHandle handle);
        void beginFrame();
        void beginRenderPass();
        void endRenderPass();
        void endFrame();
        void present();
        void commit(bool isWaitComplete = false);

    private:
        
        std::unique_ptr<struct MetalContext> _context;
    };

} // namespace el {

#endif // __METAL_DRIVER_H__
