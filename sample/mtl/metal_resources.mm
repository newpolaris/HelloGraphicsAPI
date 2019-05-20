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


_EL_NAME_END
