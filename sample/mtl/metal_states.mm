#include "metal_states.h"
#include <map>
#include <el_debug.h>
#include "metal_context.h"

_EL_NAME_BEGIN

id<MTLRenderPipelineState> createPipeline(id<MTLDevice> device, const MetalPipelineDesc &desc) 
{
    MTLRenderPipelineDescriptor* descriptor = [MTLRenderPipelineDescriptor new];
    for (auto i = 0u; i < desc.colorFormats.size(); i++)
        descriptor.colorAttachments[i].pixelFormat = desc.colorFormats[i];
    descriptor.depthAttachmentPixelFormat = desc.depthFormat;
    descriptor.vertexFunction = desc.vertexFunction;
    descriptor.fragmentFunction = desc.fragmentFunction;
    
    NSError* error = nullptr;
    id<MTLRenderPipelineState> pipeline = [device newRenderPipelineStateWithDescriptor:descriptor
                                                                                 error:&error];
#if !__has_feature(objc_arc)
    [descriptor release];
#endif
    
    if (error) {
        auto description = [error.localizedDescription cStringUsingEncoding:NSUTF8StringEncoding];
        EL_TRACE("Fail to create MTLPipelineState\n%s", description);
    }
    return pipeline;
}

id<MTLRenderPipelineState> aquirePipeline(MetalContext* context, const MetalPipelineDesc& desc)
{
    EL_ASSERT(context);
    
    auto it = context->pipelineCache.find(desc);
    if (it != context->pipelineCache.end())
        return it->second;
    
    const auto& pipeline = createPipeline(context->device, desc);
    context->pipelineCache.emplace(desc, pipeline);
   
    return pipeline;
}

void cleanupPipeline(MetalContext* context)
{
#if !__has_feature(objc_arc)
    for (auto it : context->pipelineCache)
        [it.second release];
#endif
    context->pipelineCache.clear();
}

_EL_NAME_END
