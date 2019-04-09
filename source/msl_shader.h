#pragma once

#include "predefine.h"
#include "graphics_types.h"
#include "graphics_shader.h"
#include "mtlpp.hpp"

#if EL_PLAT_APPLE

namespace el {
    
    class MSLShader final : public GraphicsShader
    {
    public:

        MSLShader();
        ~MSLShader();

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
