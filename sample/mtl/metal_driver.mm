#include "metal_driver.h"
#include "metal_states.h"
#include "metal_resources.h"
#include "metal_context.h"
#include <graphics_data.h>
#include <graphics_texture.h>

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

    cleanupPipeline(_context);
    
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
    // MTLRenderPassStencilAttachmentDescriptor* stencilAttachment = renderPassDesc.stencilAttachment;
    // [stencilAttachment setClearStencil:params.clearStencil];
    
    EL_ASSERT(_context->currentRenderEncoder == nil);
    _context->currentRenderEncoder = [_context->currentCommandBuffer renderCommandEncoderWithDescriptor:renderPassDesc];
    
    // TODO:
    // Metal requires a new command encoder for each render pass, and they cannot be reused.
    // We must bind certain states for each command encoder, so we dirty the states here to force a
    // rebinding at the first the draw call of this pass.
    // mContext->pipelineState.invalidate();
    // mContext->depthStencilState.invalidate();
    // mContext->cullModeState.invalidate();
}
void MetalDriver::setPipelineState(const PipelineState& state)
{
    MetalPipelineDesc pipelineDesc {
        state.program->vertexFunction,
        state.program->fragmentFunction,
        _context->currentColorFormats,
        _context->currentDepthFormat
    };

    id<MTLRenderPipelineState> pipeline = aquirePipeline(_context, pipelineDesc);
    EL_ASSERT(pipeline);
    
    // TODO:
    // if (updateIfChanged(pipelineDesc)) {
    [_context->currentRenderEncoder setRenderPipelineState:pipeline];
    // }
}


void MetalDriver::setVertexBuffer(const MetalBufferPtr& vertex, uint32_t slot)
{
    [_context->currentRenderEncoder setVertexBuffer:vertex->buffer offset:0 atIndex:slot];
}

void MetalDriver::setFragmentTexture(const MetalTexturePtr& texture, uint32_t slot)
{
    [_context->currentRenderEncoder setFragmentTexture:texture->texture atIndex:slot];
}

void MetalDriver::draw(MTLPrimitiveType primitive, uint32_t vertexCount, uint32_t vertexOffset)
{
    // [_context->currentRenderEncoder setVertexBytes:vertexData length:sizeof(vertexData) atIndex:0];
    // [_context->currentRenderEncoder setCullMode:(MTLCullMode)]
    // [_context->curretnRenderEncoder setFrontFacingWinding:(MTLWinding)];
    // [_context->currentRenderEncoder setCullMode:(MTLCullMode)];
    // [_context->currentRenderEncoder setLabel:(NSString * _Nullable)]
    // [_context->currentRenderEncoder setViewport:(MTLViewport)]
    
    // TODO:
    // [_context->currentRenderEncoder setFragmentSamplerState:(nullable id<MTLSamplerState>) atIndex:(NSUInteger)];
    
    [_context->currentRenderEncoder drawPrimitives:primitive
                                       vertexStart:vertexOffset
                                       vertexCount:vertexCount];
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

MetalTexturePtr MetalDriver::createTexture(const GraphicsTextureDesc &desc)
{
    @autoreleasepool {
        auto texture = std::make_shared<MetalTexture>();
        if (!texture) return nil;
        if (!texture->create(_context->device, desc))
            return nil;
        return texture;
    }
}

MetalBufferPtr MetalDriver::createVertexBuffer(const void *stream, size_t streamsize)
{
    GraphicsDataDesc desc;
    desc.setStream((const el::stream_t*)stream);
    desc.setElementSize(sizeof(char));
    desc.setNumElements(streamsize);
    
    auto buffer = std::make_shared<MetalBuffer>();
    if (!buffer) return nullptr;
    if (!buffer->create(_context->device, desc))
        return nullptr;
    return buffer;
}

_EL_NAME_END
