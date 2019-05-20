#ifndef __METAL_DRIVER_H__
#define __METAL_DRIVER_H__

#include "metal_types.h"
#include "metal_resources.h"
#include <math_types.h>
#include <vector>
#include <graphics_types.h>

namespace el {

    struct PipelineState
    {
        MetalProgramPtr program;
    };
    
    struct MetalDriver final
    {
        MetalDriver();
        ~MetalDriver();

        bool setup(void *nativeSurface);
        void cleanup();

        void makeCurrent();
        void beginFrame();
        void commit(bool isWaitFinish = false);
        void beginRenderPass(const MetalRenderTargetPtr& rt, const RenderPassParms& params);
        void endRenderPass();
        
        void setPipelineState(const PipelineState& state);
        void setVertexBuffer(const MetalBufferPtr& buffer, uint32_t slot);
        void draw(MTLPrimitiveType primitive, uint32_t vertexCount, uint32_t vertexOffset);
        
        MetalTexturePtr createTexture(const GraphicsTextureDesc &desc);
        MetalBufferPtr createVertexBuffer(const void* stream, size_t streamsize);
        MetalProgramPtr createProgram(const char* vertexShaderSrc, const char* fragmentShaderSrc);
        MetalRenderTargetPtr createDefaultRenderTarget();

        struct MetalContext* _context;
    };

}

#endif // __METAL_DRIVER_H__
