#ifndef __METAL_DRIVER_H__
#define __METAL_DRIVER_H__

#include "metal_types.h"
#include "metal_resources.h"
#include <math_types.h>
#include <vector>

namespace el {
    
    struct MetalAttachmentDesc
    {
        MTLLoadAction load;
        MTLStoreAction store;
    };
    
    typedef std::vector<MetalAttachmentDesc> MetalAttachmentDescs;
    
    struct MetalRenderPassDesc
    {
        //MetalRenderPassDesc();
        //MetalRenderPassDesc(math::float4 color);
        //MetalRenderPassDesc(math::float4 color, float depth);
        
        float clearDepth;
        math::float4 clearColor;
        MetalAttachmentDesc depthAttachment;
        MetalAttachmentDescs colorAttachments;
    };
    
    extern MetalRenderPassDesc renderPassColor;
    extern MetalRenderPassDesc renderPassDepth;

    void setupRenderPasses();
    
    struct MetalDriver final
    {
        MetalDriver();
        ~MetalDriver();

        bool setup(void *nativeSurface);
        void cleanup();

        void makeCurrent();
        void beginFrame();
        void commit(bool isWaitFinish = false);
        void beginRenderPass(const MetalRenderPassDesc& passDesc);
        void endRenderPass();
        void drawFrame(const MetalProgram& program);
        
        MetalProgram createProgram(const char* vertexShaderSrc, const char* fragmentShaderSrc);

        struct MetalContext* _context;
    };

}

#endif // __METAL_DRIVER_H__
