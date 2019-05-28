#include "metal_states.h"
#include <el_debug.h>
#include "metal_context.h"

_EL_NAME_BEGIN

id<MTLRenderPipelineState> createPipeline(id<MTLDevice> device, const MetalPipelineDesc &desc) 
{
    MTLVertexDescriptor *vertex = [MTLVertexDescriptor vertexDescriptor];
    
    const auto& inputLayout = desc.inputLayout;
    const auto& bindings = inputLayout.getBindings();
    for (uint32_t i = 0; i < bindings.size(); i++)
    {
        vertex.layouts[i].stepFunction = asMetalVertexStepFunction(bindings[i].getInputRate());
        vertex.layouts[i].stride = bindings[i].getStride();
    }
    
    const auto& attributes = inputLayout.getAttributes();
    for (uint32_t i = 0; i < attributes.size(); i++)
    {
        vertex.attributes[i].bufferIndex = attributes[i].getBinding();
        vertex.attributes[i].format = asMetalVertexFormat(attributes[i].getFormat());
        vertex.attributes[i].offset = attributes[i].getOffset();
    }
    
    MTLRenderPipelineDescriptor* descriptor = [MTLRenderPipelineDescriptor new];
    for (auto i = 0u; i < desc.colorFormats.size(); i++)
        descriptor.colorAttachments[i].pixelFormat = desc.colorFormats[i];
    descriptor.depthAttachmentPixelFormat = desc.depthFormat;
    descriptor.vertexFunction = desc.vertexFunction;
    descriptor.fragmentFunction = desc.fragmentFunction;
    descriptor.vertexDescriptor = vertex;

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

#if 0
/*
 Default values for metal
 
 MTLSamplerMinMagFilterNearest.
 MTLSamplerMinMagFilterNearest.
 MTLSamplerMipFilterNotMipmapped.
 MTLSamplerAddressModeClampToEdge.
 MTLSamplerAddressModeClampToEdge.
 MTLSamplerAddressModeClampToEdge.
 */

MetalSampler::MetalSampler() :
sampler(nil)
{
}

MetalSampler::~MetalSampler()
{
    destroy();
}

bool MetalSampler::create(id<MTLDevice> device, const GraphicsSamplerDesc &desc)
{
    if (device == nil) return false;
    
    MTLSamplerDescriptor *samplerDesc = [[MTLSamplerDescriptor new] autorelease];
    
    samplerDesc.minFilter = asSamplerMinMagFilter(desc.getMinFilter());
    samplerDesc.magFilter = asSamplerMinMagFilter(desc.getMagFilter());
    samplerDesc.mipFilter = asSamplerMipFilter(desc.getSamplerMipmapMode());
    samplerDesc.maxAnisotropy = (uint32_t)desc.getAnisotropyLevel();
    samplerDesc.sAddressMode = asSamplerAddressMode(desc.getAddressModeU());
    samplerDesc.tAddressMode = asSamplerAddressMode(desc.getAddressModeV());
    samplerDesc.tAddressMode = asSamplerAddressMode(desc.getAddressModeW());
    
    // TODO:
#if EL_PLAT_OSX
    // SetBorderColor(SamplerBorderColor borderColor);
#endif
    // SetNormalizedCoordinates(bool normalizedCoordinates);
    // SetLodMinClamp(float lodMinClamp);
    // SetLodMaxClamp(float lodMaxClamp);
    // SetCompareFunction(CompareFunction compareFunction);
    // SetLabel(const ns::String& label);
    
    sampler = [device newSamplerStateWithDescriptor:samplerDesc];
    
    if (sampler == nil)
        return false;
    
    this->desc = desc;
    return true;
}

void MetalSampler::destroy()
{
    [sampler release];
    sampler = nil;
}

const GraphicsSamplerDesc &MetalSampler::getDesc() const
{
    return desc;
}
#endif

_EL_NAME_END
