#include "metal_resources.h"
#include <el_debug.h>
#include <initializer_list>
#include "metal_context.h"

_EL_NAME_BEGIN

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
            auto description = [error.localizedDescription cStringUsingEncoding:NSUTF8StringEncoding];
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

    buffer  = [device newBufferWithBytes:desc.getStream()
                                  length:desc.getStreamSize()
                                 options:MTLResourceStorageModeShared];
    if (buffer == nil)
        return false;

    this->desc = desc;
    return true;
}

void MetalBuffer::destroy()
{
    [buffer release];
    buffer = nil;
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
        const int alignment = (int)desc.getPixelAlignment();
        uint32_t bytesPerRow = desc.getWidth() * asTexelSize(desc.getPixelFormat());
        // TODO:
        bytesPerRow = ((bytesPerRow + alignment - 1) / alignment) * alignment;
        
        // TODO:
        if ((desc.getStreamSize() % bytesPerRow) != 0)
        {
            
            return false;
        }
    
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

_EL_NAME_END
