#include "metal_states.h"
#include <el_debug.h>
#include <map>

_EL_NAME_BEGIN

std::map<PipelineDesc, id<MTLRenderPipelineState>> cache;

id<MTLRenderPipelineState> createPipeline(const PipelineDesc &desc, id<MTLDevice> device) 
{
    MTLRenderPipelineDescriptor* descriptor = [MTLRenderPipelineDescriptor new];
    descriptor.colorAttachments[0].pixelFormat = MTLPixelFormatBGRA8Unorm;
    descriptor.depthAttachmentPixelFormat = MTLPixelFormatInvalid;
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

id<MTLRenderPipelineState> aquirePipeline(id<MTLDevice> device, const PipelineDesc& desc)
{
    auto it = cache.find(desc);
    if (it != cache.end())
        return it->second;
    
    const auto& pipeline = createPipeline(desc, device);
    cache.emplace(desc, pipeline);
   
    return pipeline;
}

void cleanupPipeline()
{
#if !__has_feature(objc_arc)
    for (auto it : cache)
        [it.second release];
#endif
    cache.clear();
}

_EL_NAME_END
