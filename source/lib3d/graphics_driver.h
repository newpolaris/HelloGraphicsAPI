#ifndef __PLATFORM_DRIVER_H__
#define __PLATFORM_DRIVER_H__

#include <graphics_types.h>

namespace el {

    // TODO:
    typedef std::shared_ptr<struct MetalProgram> MetalProgramPtr;
    typedef std::shared_ptr<struct MetalRenderTarget> MetalRenderTargetPtr;
    typedef std::shared_ptr<struct MetalBuffer> MetalBufferPtr;
    typedef std::shared_ptr<struct MetalUniformBuffer> MetalUniformBufferPtr;
    typedef std::shared_ptr<struct MetalTexture> MetalTexturePtr;
    
    struct PipelineState;

    class Driver
    {
    public:

        virtual ~Driver();

        virtual bool setup(void *nativeSurface) = 0;
        virtual void cleanup() = 0;

        virtual void makeCurrent() = 0;
        virtual void beginFrame() = 0;
        virtual void commit(bool isWaitFinish = false) = 0;
        virtual void beginRenderPass(const MetalRenderTargetPtr& rt, const RenderPassParms& params) = 0;
        virtual void endRenderPass() = 0;
        
        virtual void setPipelineState(const PipelineState& state) = 0;
        virtual void setVertexBuffer(const MetalBufferPtr& buffer, uint32_t slot, uint32_t offset = 0) = 0;
        virtual void setFragmentTexture(const MetalTexturePtr& texture, uint32_t slot) = 0;
        virtual void draw(GraphicsPrimitiveType primitive, uint32_t vertexCount, uint32_t vertexOffset) = 0;
        virtual void draw(GraphicsPrimitiveType primitive, const MetalBufferPtr& indexBuffer, uint32_t indexCount, uint32_t offset) = 0;
        virtual MetalTexturePtr createTexture(const GraphicsTextureDesc &desc) = 0;
        virtual MetalBufferPtr createIndexBuffer(const void* stream, size_t count, size_t elementsize) = 0;
        virtual MetalBufferPtr createVertexBuffer(const void* stream, size_t count, size_t elementsize) = 0;
        virtual MetalUniformBufferPtr createUniformBuffer(size_t streamsize) = 0;
        virtual MetalProgramPtr createProgram(const char* vertexShaderSrc, const char* fragmentShaderSrc) = 0;
        virtual MetalRenderTargetPtr createDefaultRenderTarget() = 0;
        
        virtual void updateUniformBuffer(const MetalUniformBufferPtr& uniform, const void* stream) = 0;
        virtual void setUniform(const MetalUniformBufferPtr& uniform, uint32_t slot) = 0;
    };

}

#endif // __PLATFORM_DRIVER_H__
