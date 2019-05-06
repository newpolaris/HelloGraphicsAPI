#ifndef __MTL_RENDER_PIPELINE_H__
#define __MTL_RENDER_PIPELINE_H__
    
#include <el_predefine.h>

#if EL_BUILD_METAL

#include <graphics_types.h>
#include <graphics_pipeline.h>
#include <Metal/mtl_types.h>
#include <Metal/mtlpp.hpp>

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

#endif // __MTL_RENDER_PIPELINE_H__
