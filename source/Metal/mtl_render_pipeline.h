#pragma once
    
#include "predefine.h"

#if EL_BUILD_METAL

#include "graphics_types.h"
#include "graphics_pipeline.h"
#include "mtl_types.h"
#include "mtlpp.hpp"

namespace el {
    
    class MTLPipeline final : public GraphicsPipeline
    {
    public:

        MTLPipeline();
        ~MTLPipeline();

        bool create(GraphicsShaderStageFlagBits stage, const char* shaderCode);
        void destroy();
        
        const GraphicsShaderDesc& getDesc() const override;

        void setDevice(GraphicsDevicePtr device);
        GraphicsDevicePtr getDevice();

    private:

        GraphicsShaderDesc _desc;
        
        mtlpp::Library _library;
        mtlpp::Function _function;

        MTLDeviceWeakPtr _device;
    };

} // namespace el {

#endif // EL_BUILD_METAL
