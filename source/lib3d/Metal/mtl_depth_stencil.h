#ifndef __MTL_DEPTH_STENCIL_H__
#define __MTL_DEPTH_STENCIL_H__

#include <el_predefine.h>

#if EL_BUILD_METAL

#include <graphics_types.h>
#include <graphics_depth_stencil.h>
#include <Metal/mtl_types.h>
#include <mtlpp.hpp>

namespace el {

    class MTLDepthStencil final : GraphicsDepthStencil
    {
    public:

        MTLDepthStencil();
        ~MTLDepthStencil();

        bool create(const GraphicsDepthStencilDesc& desc);
        void destroy();

        void setDevice(GraphicsDevicePtr device);
        GraphicsDevicePtr getDevice();

        const GraphicsDepthStencilDesc& getDepthStencilDesc() const override;

    private:

        mtlpp::DepthStencilState _depthStencilState;

        GraphicsDepthStencilDesc _depthStencilDesc;
        MTLDeviceWeakPtr _device;
    };

} // namespace el {

#endif // EL_BUILD_METAL

#endif // __MTL_DEPTH_STENCIL_H__
