#pragma once

#include "predefine.h"

#if EL_PLAT_APPLE

#include "graphics_types.h"
#include "graphics_shader.h"
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

    private:

        GraphicsShaderDesc _desc;
        
        mtlpp::Library _library;
        mtlpp::Function _function;
    };

} // namespace el {

#endif // EL_PLAT_APPLE
