#include "SlideContext.h"

#include <QuartzCore/CAMetalLayer.h>
#include <Metal/Metal.h>
#include "RenderBackendConfig.h"

const char vertexShaderSrc[] = R"""(
#include <metal_stdlib>
using namespace metal;
typedef struct
{
    packed_float3 position;
    packed_float2 texcoord;
} vertex_t;
typedef struct
{
    float4 clipSpacePosition [[position]];
    float2 textureCoordinate;
} RasterizerData;
vertex RasterizerData main0(unsigned int vID[[vertex_id]])
{
    const vertex_t vertexArray[3] = {
        { {-1.0,  3.0, 0.0}, {0.0, 2.0} },
        { { 3.0, -1.0, 0.0}, {2.0, 0.0} },
        { {-1.0, -1.0, 0.0}, {0.0, 0.0} }
    };
    
    RasterizerData data;
    data.clipSpacePosition = float4(vertexArray[vID].position, 1.0);
    data.textureCoordinate = vertexArray[vID].texcoord;
    return data;
}
)""";

const char fragmentShaderSrc[] = R"""(
#include <metal_stdlib>
using namespace metal;
typedef struct
{
    float4 clipSpacePosition [[position]];
    float2 textureCoordinate;
} RasterizerData;
fragment half4 main0(
                     RasterizerData input [[stage_in]],
                     texture2d<half> colorTexture0 [[texture(0)]],
                     texture2d<half> colorTexture1 [[texture(1)]],
                     constant float& ratio[[buffer(2)]])
{
    constexpr sampler textureSampler (mag_filter::nearest,
                                      min_filter::nearest);
    // Sample the texture to obtain a color
    const half4 colorSample = colorTexture1.sample(textureSampler,
                                                   input.textureCoordinate);
    
    // We return the color of the texture
    return colorSample * ratio;
}
)""";

struct Driver
{
    id<MTLDevice> device;
    id<MTLCommandQueue> commandQueue;
    id<MTLFunction> vertexShader;
    id<MTLFunction> fragmentShader;
    id<MTLRenderPipelineState> pipeline;
    id<MTLTexture> textures[8];
    
    id<MTLFunction> bindShader[2];
    id<MTLTexture> bindTextures[8];
    
    CAMetalLayer *layer;
};

void SlideContext::setUp()
{
    backend = GetBackend();
    
    _driver = nullptr;
}

void SlideContext::cleanUp()
{
    _driver->layer = nullptr;
    
#if !__has_feature(objc_arc)
    for (int i = 0; i < 8; i++) {
        [_driver->textures[i] release];
        _driver->textures[i] = nil;
    }
    
    [_driver->pipeline release];
    [_driver->device release];
    [_driver->commandQueue release];
#endif
    _driver->device = nil;
    _driver->commandQueue = nil;
    delete _driver;
    _driver = nullptr;
}

bool SlideContext::isBackendCompatible() const
{
    return backend == GetBackend();
}

void SlideContext::createDriver()
{
    _driver = new Driver();
    _driver->device = MTLCreateSystemDefaultDevice();
    _driver->commandQueue = [_driver->device newCommandQueue];
    
    void* nativeView = GetNativewView();
    CAMetalLayer *layer = (__bridge CAMetalLayer*)nativeView;
    layer.device = _driver->device;
    
    _driver->layer = layer;
}

void SlideContext::clearScreen()
{
    MTLRenderPassDescriptor *renderdesc = [MTLRenderPassDescriptor renderPassDescriptor];
    MTLRenderPassColorAttachmentDescriptor *colorattachment = renderdesc.colorAttachments[0];
    
    /* Clear to a red-orange color when beginning the render pass. */
    colorattachment.clearColor  = MTLClearColorMake(1.0, 0.3, 0.0, 1.0);
    colorattachment.loadAction  = MTLLoadActionClear;
    colorattachment.storeAction = MTLStoreActionStore;
    
    @autoreleasepool {
        id <MTLCommandBuffer> cmdbuf = [_driver->commandQueue commandBuffer];
        
        id <CAMetalDrawable> drawable = [_driver->layer nextDrawable];
        colorattachment.texture = drawable.texture;
        
        /* The drawable's texture is cleared to the specified color here. */
        id <MTLRenderCommandEncoder> encoder = [cmdbuf renderCommandEncoderWithDescriptor:renderdesc];
        [encoder endEncoding];
        
        [cmdbuf presentDrawable:drawable];
        [cmdbuf commit];
        [cmdbuf waitUntilCompleted];
    }
}

id<MTLFunction> getShader(id<MTLDevice> device, const char* source) {
    
    NSError* error = nil;
    NSString* objcSource = [NSString stringWithCString:source
                                              encoding:NSUTF8StringEncoding];
    id<MTLLibrary> library = [device newLibraryWithSource:objcSource
                                                  options:nil
                                                    error:&error];

    if (!library) {
        const char* description = [error.localizedDescription cStringUsingEncoding:NSUTF8StringEncoding];
        NSLog(@"Fail to create MTLPipelineState\n%s", description);
    }
    id<MTLFunction> function = [library newFunctionWithName:@"main0"];

#if !__has_feature(objc_arc)
    [library release];
#endif
    return function;
}

void SlideContext::createShader()
{
    _driver->vertexShader = getShader(_driver->device, vertexShaderSrc);
    _driver->fragmentShader = getShader(_driver->device, fragmentShaderSrc);
    
    MTLRenderPipelineDescriptor* descriptor = [MTLRenderPipelineDescriptor new];
    descriptor.colorAttachments[0].pixelFormat = MTLPixelFormatBGRA8Unorm;
    descriptor.vertexFunction = _driver->vertexShader;
    descriptor.fragmentFunction =_driver->fragmentShader;
    
    NSError* error = nullptr;
    _driver->pipeline = [_driver->device newRenderPipelineStateWithDescriptor:descriptor error:&error];
    assert(_driver->pipeline);
}

/*
void SlideContext::beginRenderPass()
{
}

void SlideContext::endRenderPass()
{
}

void SlideContext::beginFrame()
{
}

void SlideContext::endframe()
{
}
*/

void SlideContext::draw()
{
    MTLRenderPassDescriptor *renderdesc = [MTLRenderPassDescriptor renderPassDescriptor];
    MTLRenderPassColorAttachmentDescriptor *colorattachment = renderdesc.colorAttachments[0];
    
    MTLClearColor clearColor = MTLClearColorMake(this->clearColor.x,
                                                 this->clearColor.y,
                                                 this->clearColor.z,
                                                 this->clearColor.w);
    
    /* Clear to a red-orange color when beginning the render pass. */
    colorattachment.clearColor  = clearColor;
    colorattachment.loadAction  = MTLLoadActionClear;
    colorattachment.storeAction = MTLStoreActionStore;
    
    @autoreleasepool {
        id <MTLCommandBuffer> cmdbuf = [_driver->commandQueue commandBuffer];
        
        id <CAMetalDrawable> drawable = [_driver->layer nextDrawable];
        colorattachment.texture = drawable.texture;
        
        MTLViewport viewport {
            this->viewport.x,
            this->viewport.y,
            this->viewport.z,
            this->viewport.w,
            0.0, 1.0 };
        
        /* The drawable's texture is cleared to the specified color here. */
        id <MTLRenderCommandEncoder> encoder = [cmdbuf renderCommandEncoderWithDescriptor:renderdesc];
        [encoder setFragmentBytes:&(this->fUniform.ratio) length:4 atIndex:2];
        [encoder setViewport:viewport];
        [encoder setRenderPipelineState:_driver->pipeline];
        [encoder setFragmentTextures:_driver->bindTextures withRange:NSRange{ 0, 8 }];
        [encoder drawPrimitives:MTLPrimitiveTypeTriangle vertexStart:0 vertexCount:3];
        [encoder endEncoding];

        [cmdbuf presentDrawable:drawable];
        [cmdbuf commit];
        [cmdbuf waitUntilCompleted];
    }
}

void SlideContext::bindShaders()
{
    _driver->bindShader[0] = _driver->vertexShader;
    _driver->bindShader[1] = _driver->fragmentShader;
}

void SlideContext::createTexture(uint32_t index, uint32_t width, uint32_t height, void* data)
{
    // TODO: swap impl and width/height comparision
    MTLTextureDescriptor* textureDesc = [[MTLTextureDescriptor new] autorelease];
    
    textureDesc.width = width;
    textureDesc.height = height;
    textureDesc.mipmapLevelCount = 1;
    textureDesc.sampleCount = 1;
    textureDesc.pixelFormat = MTLPixelFormatRGBA8Unorm;
    textureDesc.usage = MTLTextureUsageShaderRead;
    textureDesc.resourceOptions = MTLResourceStorageModeShared;
    textureDesc.cpuCacheMode = MTLCPUCacheModeDefaultCache;
    
    id<MTLTexture> texture = [_driver->device newTextureWithDescriptor:textureDesc];
    
    assert(texture != nil);
    if (data)
    {
        auto region = MTLRegionMake2D( 0, 0, textureDesc.width, textureDesc.height );
        [texture replaceRegion:region
                   mipmapLevel:0
                     withBytes:data
                   bytesPerRow:width * 4];
    }
    
    [_driver->textures[index] release];
    _driver->textures[index] = texture;
}

void SlideContext::bindTexture(uint8_t slot, uint32_t index)
{
    _driver->bindTextures[slot] = _driver->textures[index];
}
