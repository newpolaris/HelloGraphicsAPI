#include "metal_driver.h"
#include "metal_states.h"
#include "metal_resources.h"
#include <math_types.h>
#include <Metal/mtl_types.h>

_EL_NAME_BEGIN

const float vertexData[] =
{
    0.0f,  1.0f, 0.0f,
    -1.0f, -1.0f, 0.0f,
    1.0f, -1.0f, 0.0f,
};

struct MetalContext
{
    id<MTLDevice> device;
    id<MTLCommandQueue> commandQueue;
    id<MTLCommandBuffer> currentCommandBuffer;
    id<CAMetalDrawable> currentDrawable;
    id<MTLRenderCommandEncoder> currentRenderEncoder;
    CAMetalLayer *layer;

#if !__has_feature(objc_arc)
    NSAutoreleasePool *framePool;
    NSAutoreleasePool *driverPool;
#endif
};

id<CAMetalDrawable> aquireDrawable(MetalContext* context)
{
    EL_ASSERT(context);
    EL_ASSERT(context->layer);

    if (context->currentDrawable == nil)
        context->currentDrawable = [context->layer nextDrawable];
    return context->currentDrawable;
}

MetalDriver::MetalDriver() :
    _context(new MetalContext)
{
    _context->device = nil;
    _context->commandQueue = nil;
    _context->currentCommandBuffer = nil;
    _context->currentDrawable = nil;
    _context->currentRenderEncoder = nil;
    _context->layer = nil;

#if !__has_feature(objc_arc)
    _context->framePool = nil;
    _context->driverPool = nil;
#endif
}

MetalDriver::~MetalDriver()
{
}

bool MetalDriver::setup(void *nativeSurface)
{
#if !__has_feature(objc_arc)
    _context->driverPool = [[NSAutoreleasePool alloc] init];
#endif

    id<MTLDevice> device = MTLCreateSystemDefaultDevice();
    if (!device) return false;
    _context->device = device;
    _context->commandQueue = [device newCommandQueue];
    
    _context->layer = (__bridge CAMetalLayer*)nativeSurface;
    _context->layer.device = device;
    
    return true;
}

void MetalDriver::cleanup()
{
    EL_ASSERT(_context);
    if (_context == nullptr) return;

    _context->currentRenderEncoder = nil;
    _context->currentCommandBuffer = nil;
    _context->currentDrawable = nil;

    if (_context->commandQueue)
    {
        id<MTLCommandBuffer> oneOffBuffer = [_context->commandQueue commandBuffer];
        [oneOffBuffer commit];
        [oneOffBuffer waitUntilCompleted];
    }
    _context->layer = nil;
#if !__has_feature(objc_arc)
    [_context->commandQueue release];
    [_context->device release];
#endif
    _context->commandQueue = nil;
    _context->device = nil;
#if !__has_feature(objc_arc)
    [_context->driverPool drain];
    _context->driverPool = nil;
#endif

    delete _context;
    _context = nullptr;
}

void MetalDriver::beginFrame()
{
#if !__has_feature(objc_arc)
    _context->framePool = [[NSAutoreleasePool alloc] init];
#endif

    _context->currentCommandBuffer = [_context->commandQueue commandBuffer];
}

MetalRenderPassDesc renderPassColor;
MetalRenderPassDesc renderPassDepth;

void setupRenderPasses()
{
    renderPassColor = MetalRenderPassDesc{};
    renderPassColor.clearColor = math::float4(0.f, 0.f, 0.f, 0.f);
    renderPassColor.colorAttachments.resize(1);
    renderPassColor.colorAttachments[0].load = MTLLoadActionClear;
    renderPassColor.colorAttachments[0].store = MTLStoreActionStore;

    renderPassDepth = renderPassColor;
    renderPassDepth.clearDepth = 1.f;
    renderPassDepth.depthAttachment.load = MTLLoadActionClear;
    renderPassDepth.depthAttachment.store = MTLStoreActionDontCare;
}

struct MetalTexture
{
};

struct MetalRenderTarget final
{
    MetalRenderTarget(MetalContext& context);
    MetalRenderTarget(MetalContext& context, id<MTLTexture> color);
    MetalRenderTarget(MetalContext& context, id<MTLTexture> color, id<MTLTexture> depth);
    ~MetalRenderTarget();
    
    id<MTLTexture> getColor();
    
    bool isDefault;
    MetalContext& context;
    id<MTLTexture> color;
    id<MTLTexture> depth;
};

MetalRenderTarget::MetalRenderTarget(MetalContext& context) :
    isDefault(true),
    context(context)
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

void MetalDriver::beginRenderPass(const MetalRenderPassDesc& passDesc)
{
    MetalRenderTarget target(*_context);

    EL_ASSERT(true);

    const MTLClearColor clearColor = asMTLClearColor(passDesc.clearColor);

    // _context->currentSurfaceFormats.resize(passDesc.colorAttachments.size());
    // _context->currentDepthFormat
    

    // [_context->device newTextureWithDescriptor>
    
    MTLRenderPassDescriptor* renderPassDesc = [MTLRenderPassDescriptor renderPassDescriptor];
    
    for (auto i = 0u; i < passDesc.colorAttachments.size(); i++)
    {
        const auto& color = passDesc.colorAttachments[i];
        const auto& texture = target.getColor();

        MTLRenderPassColorAttachmentDescriptor* colorAttachment = renderPassDesc.colorAttachments[i];
        [colorAttachment setClearColor:clearColor];
        [colorAttachment setLoadAction:color.load];
        [colorAttachment setStoreAction:color.store];

        [colorAttachment setTexture:texture];

        // _context->currentSurfaceFormats[i] = texture.pixelFormat;
    }

    EL_ASSERT(_context->currentRenderEncoder == nil);
    _context->currentRenderEncoder = [_context->currentCommandBuffer renderCommandEncoderWithDescriptor:renderPassDesc];
}

void MetalDriver::drawFrame(const MetalProgram& program)
{
    PipelineDesc desc {
        program.vertexFunction,
        program.fragmentFunction
    };


    id<MTLRenderPipelineState> pipeline = aquirePipeline(_context->device, desc);
    EL_ASSERT(pipeline);

    [_context->currentRenderEncoder setRenderPipelineState:pipeline];
    [_context->currentRenderEncoder setVertexBytes:vertexData length:sizeof(vertexData) atIndex:0];
    [_context->currentRenderEncoder drawPrimitives:MTLPrimitiveTypeTriangle
                                       vertexStart:0
                                       vertexCount:3];
}

void MetalDriver::endRenderPass()
{
    [_context->currentRenderEncoder endEncoding];
    _context->currentRenderEncoder = nil;
}

void MetalDriver::commit(bool isWaitFinish)
{
    if (_context->currentDrawable)
        [_context->currentCommandBuffer presentDrawable:_context->currentDrawable];
    [_context->currentCommandBuffer commit];
    _context->currentCommandBuffer = nil;
    _context->currentDrawable = nil;

#if !__has_feature(objc_arc)
    [_context->framePool drain];
    _context->framePool = nil;
#endif
}

MetalProgram MetalDriver::createProgram(const char* vertexShaderSrc, const char* fragmentShaderSrc)
{
    MetalProgram program;
    program.create(_context->device, vertexShaderSrc, fragmentShaderSrc);
    return program;
}

_EL_NAME_END
