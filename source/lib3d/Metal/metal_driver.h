#ifndef __METAL_DRIVER_H__
#define __METAL_DRIVER_H__

#include <graphics_driver.h>
#include "metal_types.h"

namespace el {

    class PlatformMetal;

    class MetalDriver final : public Driver
    {
    public:

        MetalDriver(PlatformMetal* platform);
        ~MetalDriver();

        bool setup(void *nativeSurface) override;
        void cleanup() override;

        void makeCurrent() override;
        void beginFrame() override;
        void commit(bool isWaitFinish) override;
        void beginRenderPass(const MetalRenderTargetPtr& rt, const RenderPassParms& params) override;
        void endRenderPass() override;
        
        void setPipelineState(const PipelineState& state) override;
        void setVertexBuffer(const MetalBufferPtr& buffer, uint32_t slot) override;
        void setFragmentTexture(const MetalTexturePtr& texture, uint32_t slot) override;
        void draw(GraphicsPrimitiveType primitive, uint32_t vertexCount, uint32_t vertexOffset) override;
        
        MetalTexturePtr createTexture(const GraphicsTextureDesc &desc) override;
        MetalBufferPtr createVertexBuffer(const void* stream, size_t streamsize) override;
        MetalProgramPtr createProgram(const char* vertexShaderSrc, const char* fragmentShaderSrc) override;
        MetalRenderTargetPtr createDefaultRenderTarget() override;

        PlatformMetal& _platform;
        MetalContext* _context;
    };

}

#endif // __METAL_DRIVER_H__
