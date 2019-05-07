#ifndef __MTL_SHADER_H__
#define __MTL_SHADER_H__

#include <el_predefine.h>

#if EL_BUILD_METAL

#include <graphics_types.h>
#include <graphics_shader.h>
#include <Metal/mtl_types.h>
#include <mtlpp.hpp>

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

#endif // EL_BUILD_METAL

#endif // __MTL_SHADER_H__
