#pragma once
    
#include "predefine.h"

#if EL_BUILD_METAL

#include "graphics_types.h"
#include "graphics_pipeline.h"
#include "mtl_types.h"
#include "mtlpp.hpp"

namespace el {
    
    class MTLRenderPipeline final : public GraphicsPipeline
    {
    public:

        MTLRenderPipeline();
        ~MTLRenderPipeline();

        bool create(const GraphicsPipelineDesc& desc);
        void destroy();
        
        const GraphicsPipelineDesc& getDesc() const override;

        void setDevice(GraphicsDevicePtr device);
        GraphicsDevicePtr getDevice();

    private:

        mtlpp::RenderPipelineState _pipelineState;
        GraphicsPipelineDesc _pipelineDesc;
        MTLDeviceWeakPtr _device;
    };

} // namespace el {

#endif // EL_BUILD_METAL
