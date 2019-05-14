#ifndef __MTL_INPUT_LAYOUT_H__
#define __MTL_INPUT_LAYOUT_H__

#include <el_predefine.h>

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
        void destroy();

        void setDevice(GraphicsDevicePtr device);
        GraphicsDevicePtr getDevice();

        const GraphicsInputLayoutDesc& getDesc() const override;

    private:

        mtlpp::VertexDescriptor _inputLayout;
        GraphicsInputLayoutDesc _desc;
        MTLDeviceWeakPtr _device;
    };

} // namespace el {

#endif // EL_BUILD_METAL

#endif // __MTL_INPUT_LAYOUT_H__
