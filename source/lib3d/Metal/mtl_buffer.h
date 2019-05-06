#ifndef __MTL_BUFFER_H__
#define __MTL_BUFFER_H__

#include <el_predefine.h>

#if EL_BUILD_METAL

#include <graphics_types.h>
#include <graphics_data.h>
#include <Metal/mtl_types.h>
#include <Metal/mtlpp.hpp>

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

#endif // EL_BUILD_METAL

#endif // __MTL_BUFFER_H__
