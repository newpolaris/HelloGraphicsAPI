#pragma once

#include "predefine.h"

#if EL_PLAT_APPLE

#include <graphics_types.h>
#include <graphics_data.h>
#include "mtl_types.h"
#include "mtlpp.hpp"

namespace el {

    class MTLBuffer final : public GraphicsData
    {
    public:

        MTLBuffer();
        ~MTLBuffer();

        bool create(GraphicsDataDesc desc) override;
        void destroy();

        void bind() const;
        
        const GraphicsDataDesc& getDesc() const override;
        void setDevice(GraphicsDevicePtr device);
        GraphicsDevicePtr getDevice();

    private:

        mtlpp::Buffer _buffer;
        
        GraphicsDataDesc _desc;

        MTLDeviceWeakPtr _device;
    };

} // namespace el {

#endif // EL_PLAT_APPLE
