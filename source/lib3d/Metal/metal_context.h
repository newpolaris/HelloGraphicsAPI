#ifndef __METAL_CONTEXT_H__
#define __METAL_CONTEXT_H__

#include "metal_types.h"
#include "metal_states.h"

namespace el {

    id<CAMetalDrawable> aquireDrawable(MetalContext* context);

    struct MetalContext
    {
        id<MTLDevice> device;
        id<MTLCommandQueue> commandQueue;
        id<MTLCommandBuffer> currentCommandBuffer;
        id<CAMetalDrawable> currentDrawable;
        id<MTLRenderCommandEncoder> currentRenderEncoder;
        CAMetalLayer *layer;
        MetalPixelFormats currentColorFormats;
        MetalPixelFormat currentDepthFormat;
    #if !__has_feature(objc_arc)
        NSAutoreleasePool *framePool;
        NSAutoreleasePool *driverPool;
    #endif
        
        std::map<MetalPipelineDesc, id<MTLRenderPipelineState>> pipelineCache;
    };

}

#endif // __METAL_CONTEXT_H__
