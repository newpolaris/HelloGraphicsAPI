#include "metal_resources.h"
#include <el/debug.h>
#include <initializer_list>
#include "metal_context.h"

_EL_NAME_BEGIN

uint32_t asAlignmentSize(uint32_t size, uint32_t alignment)
{
    EL_ASSERT((int32_t)size >= 0);
    return ((size + alignment - 1) / alignment) * alignment;
}

MetalProgram::MetalProgram() :
    vertexFunction(nil),
    fragmentFunction(nil)
{
}

MetalProgram::~MetalProgram()
{
    destroy();
}

bool MetalProgram::create(id<MTLDevice> device, const char* vertex, const char* fragment)
{
    enum { MTLFunctionTypeMax = MTLFunctionTypeKernel };
    
    NSError* error = nil;

    auto sources = { vertex, fragment };
    id<MTLFunction> outputs[MTLFunctionTypeMax] = {};
    
    for (auto source : sources)
    {
        NSString* objcSource = [NSString stringWithCString:source
                                                  encoding:NSUTF8StringEncoding];
        id<MTLLibrary> library = [device newLibraryWithSource:objcSource
            options:nil
            error:&error];

        if (!library) {
            const char* description = [error.localizedDescription cStringUsingEncoding:NSUTF8StringEncoding];
            EL_TRACE("Fail to create MTLPipelineState\n%s", description);
            el::debug_break();
            return false;
        }
        id<MTLFunction> function = [library newFunctionWithName:@"main0"];

#if !__has_feature(objc_arc)
        [library release];
#endif
        
        if (function == nil)
            return false;
        
        outputs[function.functionType] = function;
    }
    
    vertexFunction = outputs[MTLFunctionTypeVertex];
    fragmentFunction = outputs[MTLFunctionTypeFragment];

    return true;
}

void MetalProgram::destroy()
{
#if !__has_feature(objc_arc)
    [vertexFunction release];
    [fragmentFunction release];
#endif
    vertexFunction = nil;
    fragmentFunction = nil;
}


MetalRenderTarget::MetalRenderTarget(MetalContext& context) :
    isDefault(true),
    context(context),
    color(nil),
    depth(nil)
{
}

MetalRenderTarget::MetalRenderTarget(MetalContext& context, id<MTLTexture> color) :
    isDefault(false),
    context(context)
{
    this->color = color;
#if !__has_feature(objc_arc)
    [color retain];
#endif
}

MetalRenderTarget::MetalRenderTarget(MetalContext& context, id<MTLTexture> color, id<MTLTexture> depth) :
    isDefault(false),
    context(context)
{
    this->color = color;
    this->depth = depth;
#if !__has_feature(objc_arc)
    [color retain];
    [depth retain];
#endif
}

MetalRenderTarget::~MetalRenderTarget()
{
#if !__has_feature(objc_arc)
    [color release];
    color = nil;
    [depth release];
    depth = nil;
#endif
}

id<MTLTexture> MetalRenderTarget::getColor()
{
    if (isDefault) {
        id<CAMetalDrawable> drawable = aquireDrawable(&context);
        if (drawable != nil)
            return drawable.texture;
    }
    return color;
}

id<MTLTexture> MetalRenderTarget::getDepth()
{
    return depth;
}

MetalBuffer::MetalBuffer() :
    buffer(nil)
{
}

MetalBuffer::~MetalBuffer()
{
    destroy();
}

const GraphicsDataDesc& MetalBuffer::getDesc() const
{
    return desc;
}

#if 0
mtlpp::ResourceOptions asResourceOptions(GraphicsUsageFlags flags)
{
    auto options = mtlpp::ResourceOptions::CpuCacheModeWriteCombined;
    if (flags & GraphicsUsageFlagReadBit)
        options = mtlpp::ResourceOptions::CpuCacheModeDefaultCache;
    return options;
}
#endif

bool MetalBuffer::create(id<MTLDevice> device, const GraphicsDataDesc& desc)
{
    // auto resourceOptions = asResourceOptions(desc.getUsage());

    buffer = [device newBufferWithBytes:desc.getStream()
                                 length:desc.getStreamSize()
                                options:MTLResourceStorageModeShared];
    if (buffer == nil)
        return false;
    buffer.label = @"vertex/index-buffer";
    
    this->desc = desc;
    return true;
}

void MetalBuffer::destroy()
{
    [buffer release];
    buffer = nil;
}

MetalUniformBuffer::MetalUniformBuffer() :
    buffer(nil)
{
}

MetalUniformBuffer::~MetalUniformBuffer()
{
    destroy();
}

const GraphicsDataDesc& MetalUniformBuffer::getDesc() const
{
    return desc;
}

bool MetalUniformBuffer::create(id<MTLDevice> device, const GraphicsDataDesc& desc)
{
    MTLResourceOptions options = MTLCPUCacheModeWriteCombined | MTLResourceStorageModeShared;
    buffer = [device newBufferWithLength:desc.getStreamSize()
                                 options:options];
    buffer.label = @"uniform-buffer";
    if (buffer == nil)
        return false;
    
    this->desc = desc;
    return true;
}

void MetalUniformBuffer::destroy()
{
    [buffer release];
    buffer = nil;
}


void MetalUniformBuffer::update(const stream_t* stream)
{
    char *contents = (char *)[buffer contents];
    memcpy(contents, stream, desc.getStreamSize());
}

MetalTexture::MetalTexture() :
    texture(nil)
{
}

 MetalTexture::~MetalTexture()
 {
     destroy();
 }

bool MetalTexture::create(id<MTLDevice> device, const GraphicsTextureDesc &desc)
{
    if (device == nil) return false;
    if (desc.getHeight() == 0 || desc.getWidth() == 0)
        return false;

    auto format = asMetalPixelFormat(desc.getPixelFormat());
    MTLTextureDescriptor* textureDesc = [[MTLTextureDescriptor new] autorelease];
    
    textureDesc.width = desc.getWidth();
    textureDesc.height = desc.getHeight();
    textureDesc.mipmapLevelCount = 1;
    textureDesc.sampleCount = 1;
    textureDesc.pixelFormat = format;
    textureDesc.usage = asMetalTextureUsage(desc.getTextureUsage());
    textureDesc.resourceOptions = asMetalTextureResourceOptions(desc.getTextureUsage());
    textureDesc.cpuCacheMode = MTLCPUCacheModeDefaultCache;

    texture = [device newTextureWithDescriptor:textureDesc];
    
    if (texture == nil)
        return false;

    if (desc.getStream())
    {
        const auto alignment = (uint32_t)desc.getPixelAlignment();
        uint32_t bytesPerPixel = asTexelSize(desc.getPixelFormat());
        uint32_t bytesPerRow = asAlignmentSize(desc.getWidth() * bytesPerPixel, alignment);

        auto region = MTLRegionMake2D( 0, 0, textureDesc.width, textureDesc.height );
        [texture replaceRegion:region mipmapLevel:0 withBytes:desc.getStream() bytesPerRow:bytesPerRow];
    }
	this->desc = desc;
    return true;
}

void MetalTexture::destroy()
{
    [texture release];
    texture = nil;
}
    
const GraphicsTextureDesc &MetalTexture::getDesc() const
{
    return desc;
}

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

_EL_NAME_END
