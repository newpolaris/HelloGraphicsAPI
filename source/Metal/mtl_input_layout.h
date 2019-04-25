#pragma once

#include "mtl_types.h"
#include <graphics_types.h>
#include <graphics_input_layout.h>

namespace el {

    class MTLInputLayout final : public GraphicsInputLayout
    {
    public:

        MTLInputLayout();
        ~MTLInputLayout();

        bool create(GraphicsInputLayoutDesc desc);
        void destory();

        void setDevice(GraphicsDevicePtr device);
        GraphicsDevicePtr getDevice();

        const GraphicsInputLayoutDesc& getDesc() const override;

    private:

        GraphicsInputLayoutDesc _desc;

        MetalDeviceWeakPtr _device;
    };

} // namespace el {
