#include "metal_driver.h"
#include "metal_states.h"
#include "metal_resources.h"
#include "metal_context.h"
#include <math_types.h>
#include <Metal/mtl_types.h>

_EL_NAME_BEGIN

const float vertexData[] =
{
    0.0f,  1.0f, 0.0f,
    -1.0f, -1.0f, 0.0f,
    1.0f, -1.0f, 0.0f,
};

MetalDriver::MetalDriver() :
    _context(new MetalContext)
{
    _context->device = nil;
    _context->commandQueue = nil;
    _context->currentCommandBuffer = nil;
    _context->currentDrawable = nil;
    _context->currentRenderEncoder = nil;
    _context->layer = nil;
    _context->currentDepthFormat = MTLPixelFormatInvalid;

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
    _context->currentColorFormats.clear();
    _context->currentDepthFormat = MTLPixelFormatInvalid;

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

void MetalDriver::beginRenderPass(const MetalRenderTargetPtr& rt, const RenderPassParms& params)
{
    const uint8_t numRenderTarget = 1;
    const MTLClearColor clearColor = asMTLClearColor(params.clearColor);
    
    auto renderTarget = rt;
    
    _context->currentColorFormats.resize(numRenderTarget);
    
    MTLRenderPassDescriptor* renderPassDesc = [MTLRenderPassDescriptor renderPassDescriptor];
    
    const auto loadColor = asLoadAction(params, GraphicsTargetBufferFlagBitColor);
    const auto storeColor = asStoreAction(params, GraphicsTargetBufferFlagBitColor);
    
    for (auto i = 0u; i < numRenderTarget; i++)
    {
        id<MTLTexture> texture = renderTarget->getColor();

        MTLRenderPassColorAttachmentDescriptor* colorAttachment = renderPassDesc.colorAttachments[i];
        [colorAttachment setClearColor:clearColor];
        [colorAttachment setLoadAction:loadColor];
        [colorAttachment setStoreAction:storeColor];
        [colorAttachment setTexture:texture];

        _context->currentColorFormats[i] = texture.pixelFormat;
    }
    
    id<MTLTexture> depth = renderTarget->getDepth();
    MTLRenderPassDepthAttachmentDescriptor* depthAttachment = renderPassDesc.depthAttachment;
    [depthAttachment setClearDepth:params.clearDepth];
    [depthAttachment setLoadAction:asLoadAction(params, GraphicsTargetBufferFlagBitDepth)];
    [depthAttachment setStoreAction:asStoreAction(params, GraphicsTargetBufferFlagBitDepth)];
    [depthAttachment setTexture:depth];
    
    _context->currentDepthFormat = depth.pixelFormat;
    
    // TODO:
    MTLRenderPassStencilAttachmentDescriptor* stencilAttachment = renderPassDesc.stencilAttachment;
    [stencilAttachment setClearStencil:params.clearStencil];
    
    EL_ASSERT(_context->currentRenderEncoder == nil);
    _context->currentRenderEncoder = [_context->currentCommandBuffer renderCommandEncoderWithDescriptor:renderPassDesc];
}

void MetalDriver::draw(const PipelineDesc& desc)
{
    MetalPipelineDesc pipelineDesc {
        desc.program->vertexFunction,
        desc.program->fragmentFunction,
        _context->currentColorFormats,
        _context->currentDepthFormat
    };

    id<MTLRenderPipelineState> pipeline = aquirePipeline(_context->device, pipelineDesc);
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

MetalProgramPtr MetalDriver::createProgram(const char* vertexShaderSrc, const char* fragmentShaderSrc)
{
    auto program = std::make_shared<MetalProgram>();
    if (!program) return nullptr;
    if (!program->create(_context->device, vertexShaderSrc, fragmentShaderSrc))
        return nullptr;;
    return program;
}

MetalRenderTargetPtr MetalDriver::createDefaultRenderTarget()
{
    auto target = std::make_shared<MetalRenderTarget>(*_context);
    if (!target) return nullptr;
    return target;
}


MetalDriver::MetalBufferPtr MetalDriver::createVertexBuffer(const void *stream, size_t streamsize)
{
    el::GraphicsDataDesc vertexData;
    vertexData.setStream((const el::stream_t*)stream);
    vertexData.setElementSize(sizeof(char));
    vertexData.setNumElements(streamsize);
    
    auto buffer = std::make_shared<MetalBuffer>();
    if (!buffer) return nullptr;
    if (!buffer->create(_context->device, vertexData))
        return nullptr;
    return buffer;
}

_EL_NAME_END
