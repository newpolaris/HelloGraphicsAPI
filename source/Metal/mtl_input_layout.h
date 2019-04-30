#pragma once

#include "predefine.h"

#if EL_BUILD_METAL

#include "mtl_types.h"
#include <graphics_types.h>
#include <graphics_input_layout.h>

namespace el {

    class MTLInputLayout final : public GraphicsInputLayout
    {
    public:

        MTLInputLayout();
        ~MTLInputLayout();

        bool create(const GraphicsInputLayoutDesc& desc);
        void destory();

        void setDevice(GraphicsDevicePtr device);
        GraphicsDevicePtr getDevice();

        const GraphicsInputLayoutDesc& getDesc() const override;

    private:

        GraphicsInputLayoutDesc _desc;

        MTLDeviceWeakPtr _device;
    };

} // namespace el {

#endif // EL_BUILD_METAL
