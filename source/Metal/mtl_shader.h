#pragma once

#include "predefine.h"

#if EL_PLAT_APPLE

#include "graphics_types.h"
#include "graphics_shader.h"
#include "mtl_types.h"
#include "mtlpp.hpp"

namespace el {
    
    class MTLShader final : public GraphicsShader
    {
    public:

        MTLShader();
        ~MTLShader();

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

#endif // EL_PLAT_APPLE
