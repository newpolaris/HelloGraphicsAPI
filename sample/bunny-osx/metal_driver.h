#ifndef __METAL_DRIVER_H__
#define __METAL_DRIVER_H__

#include <graphics_types.h>
#include <mtlpp.hpp>
#include <math_types.h>
#include "metal_types.h"

namespace el {

    class MetalDriver
    {
    public:

        MetalDriver();
        virtual ~MetalDriver();

        mtlpp::Device& getDevice();
        mtlpp::CommandBuffer& getCurrentCommandBuffer();

        MetalRendertarget createDefaultRenderTarget();
        
        void makeCurrent(NativeSurface surface);
        void beginFrame();
        void beginRenderPass(const MetalRenderTarget& rt, const PassDescriptor& desc, const std::string& label);
        void endRenderPass();
        void endFrame();
        void present();
        void commit(bool isWaitComplete = false);
        
        void setRenderPipelineDesc(const MetalRenderPipelineDesc& pipelineDesc);
        void setFragmentTexture(const mtlpp::Texture& texture, uint32_t index);
        void draw(GraphicsPrimitiveType primitiveType, uint32_t vertexCount, uint32_t vertexStart = 0);

    private:
        
        MetalContext* _context;
    };

} // namespace el {

#endif // __METAL_DRIVER_H__
