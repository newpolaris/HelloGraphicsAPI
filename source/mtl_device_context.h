#pragma once

#include "predefine.h"

#if EL_PLAT_APPLE

#include "mtlpp.hpp"
#include "mtl_types.h"
#include "graphics_device_context.h"

namespace el {

    class MTLDeviceContext final : public GraphicsDeviceContext
    {
    public:

        MTLDeviceContext();
        ~MTLDeviceContext();

        bool create();
        void destroy();

        void setDevice(GraphicsDevicePtr device);
        GraphicsDevicePtr getDevice();

        void finsh(bool waitForCompletion = false);

    private:

        mtlpp::CommandBuffer _commandBuffer;

        MetalDeviceWeakPtr _device;
    };

} // namespace el {

#endif // EL_PLAT_APPLE
