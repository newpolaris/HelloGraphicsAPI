#pragma once

#include "predefine.h"

#if EL_BUILD_METAL

#include <graphics_types.h>
#include <graphics_depth_stencil.h>
#include "mtl_types.h"
#include "mtlpp.hpp"

namespace el {

    mtlpp::StencilDescriptor asStencilDescriptor(const GraphicsStencilOpState& state);

    class MTLDepthStencil final : GraphicsDepthStencil
    {
    public:

        MTLDepthStencil();
        ~MTLDepthStencil();

        bool create(const GraphicsDepthStencilDesc& desc);
        void destory();

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
