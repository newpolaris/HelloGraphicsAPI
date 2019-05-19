#include "metal_resources.h"
#include <el_debug.h>
#include <initializer_list>

_EL_NAME_BEGIN

MetalProgram::MetalProgram()
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
        outputs[function.functionType] = function;

#if !__has_feature(objc_arc)
        [objcSource release];
        [library release];
#endif
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

_EL_NAME_END
