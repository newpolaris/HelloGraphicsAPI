#ifndef __METAL_DRIVER_H__
#define __METAL_DRIVER_H__

#include <el/graphics_driver.h>
#include "metal_types.h"

namespace el {

    class PlatformMetal;

    // TODO:
    // - (MetalBuffer *)dequeueReusableBufferOfLength:(NSUInteger)length device:(id<MTLDevice>)device;
    // - (void)enqueueReusableBuffer:(MetalBuffer *)buffer;
    
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
        void setVertexBytes(const void* stream, size_t size, uint32_t slot) override;
        void setVertexBuffer(const MetalBufferPtr& buffer, uint32_t slot, uint32_t offset) override;
        void setFragmentTexture(const MetalTexturePtr& texture, uint32_t slot) override;
        void draw(GraphicsPrimitiveType primitive, uint32_t vertexCount, uint32_t vertexOffset) override;
        void draw(GraphicsPrimitiveType primitive, const MetalBufferPtr& indexBuffer, uint32_t indexCount, uint32_t offset) override;
        
        MetalTexturePtr createTexture(const GraphicsTextureDesc& desc) override;
        MetalBufferPtr createIndexBuffer(const void* stream, size_t count, size_t elementsize) override;
        MetalBufferPtr createVertexBuffer(const void* stream, size_t count, size_t elementsize) override;
        MetalUniformBufferPtr createUniformBuffer(size_t streamsize) override;
        MetalProgramPtr createProgram(const char* vertexShaderSrc, const char* fragmentShaderSrc) override;
        MetalRenderTargetPtr createDefaultRenderTarget() override;
        
        void updateUniformBuffer(const MetalUniformBufferPtr& uniform, const void* stream) override;
        void setUniform(const MetalUniformBufferPtr& uniform, uint32_t slot) override;

        PlatformMetal& _platform;
        MetalContext* _context;
    };

}

#endif // __METAL_DRIVER_H__
