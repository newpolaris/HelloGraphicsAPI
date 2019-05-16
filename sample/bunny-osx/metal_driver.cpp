#include "metal_driver.h"
#include "metal_context.h"
#include <el_debug.h>

#include <Metal/mtl_types.h>

_EL_NAME_BEGIN

MetalDriver::MetalDriver() :
    _context(new MetalContext())
{
    _context->device = mtlpp::Device::CreateSystemDefaultDevice();
    _context->commandQueue = _context->device.NewCommandQueue();
}

MetalDriver::~MetalDriver()
{
    delete _context;
}

mtlpp::Device &MetalDriver::getDevice()
{
    return _context->device;
}

mtlpp::CommandBuffer &MetalDriver::getCurrentCommandBuffer()
{
    EL_ASSERT(_context->currentCommandBuffer);
    return _context->currentCommandBuffer;
}

void MetalDriver::makeCurrent(NativeSurface surface)
{
    _context->currentSurface = std::unique_ptr<MetalSurface>(new MetalSurface(surface));
}

void MetalDriver::beginFrame()
{
    _context->currentCommandBuffer = _context->commandQueue.CommandBuffer();
    EL_ASSERT(_context->currentCommandBuffer);
}

void MetalDriver::commit(bool isWaitComplete)
{
    _context->currentCommandBuffer.Commit();
    if (isWaitComplete)
        _context->currentCommandBuffer.WaitUntilCompleted();
}

void MetalDriver::endFrame()
{
    _context->currentCommandBuffer = ns::Handle{};    
}

void MetalDriver::beginRenderPass(const MetalRenderTarget& rt, const PassDescriptor& passDesc, const std::string& label)
{
    EL_ASSERT(!_context->renderCommandEncoder);
    
    mtlpp::RenderPassDescriptor desc;

    auto colorAttachment = desc.GetColorAttachments()[0];
    colorAttachment.SetClearColor(asMetalClearColor(passDesc.clearColor));
    colorAttachment.SetLoadAction(passDesc.loadColor);
    colorAttachment.SetStoreAction(passDesc.storeColor);
    colorAttachment.SetTexture(rt.getColor());
    colorAttachment.SetLevel(rt.getLevel());
    colorAttachment.SetSlice(0);
    desc.SetRenderTargetArrayLength(1);
    
    _context->currentSurfacePixelFormat = colorAttachment.GetTexture().GetPixelFormat();

    auto depthAttachment = desc.GetDepthAttachment();
    depthAttachment.SetClearDepth(passDesc.clearDepth);
    depthAttachment.SetTexture(rt.getDepth());
    depthAttachment.SetLoadAction(passDesc.loadDepth);
    depthAttachment.SetStoreAction(passDesc.storeDepth);
        
    _context->currentDepthPixelFormat = depthAttachment.GetTexture().GetPixelFormat();

    auto encoder = _context->currentCommandBuffer.RenderCommandEncoder(desc);
    if (!label.empty())
        encoder.SetLabel(label.c_str());
    EL_ASSERT(encoder);
    _context->renderCommandEncoder = encoder;
}

void MetalDriver::endRenderPass()
{
    EL_ASSERT(_context->renderCommandEncoder);
    _context->renderCommandEncoder.EndEncoding();
    _context->renderCommandEncoder = ns::Handle{};
}

void MetalDriver::present()
{
    EL_ASSERT(_context->currentCommandBuffer);
    _context->currentCommandBuffer.Present(_context->currentDrawable);
}

void MetalDriver::setRenderPipelineDesc(const MetalRenderPipelineDesc& pipelineDesc)
{
    EL_ASSERT(_context->renderCommandEncoder);
    
    mtlpp::RenderPipelineDescriptor desc;
    desc.SetLabel(pipelineDesc.label);
    desc.SetVertexFunction(pipelineDesc.vertexFunction);
    desc.SetFragmentFunction(pipelineDesc.fragmentFunction);
    auto colorAttachment = desc.GetColorAttachments()[0];
    colorAttachment.SetPixelFormat(_context->currentSurfacePixelFormat);
    desc.SetDepthAttachmentPixelFormat(_context->currentDepthPixelFormat);

    ns::Error error;
    auto pipelineState = _context->device.NewRenderPipelineState(desc, &error);
    EL_ASSERT(pipelineState);
    _context->renderCommandEncoder.SetRenderPipelineState(pipelineState);
}

void MetalDriver::setFragmentTexture(const mtlpp::Texture &texture, uint32_t index)
{
    EL_ASSERT(_context->renderCommandEncoder);
    _context->renderCommandEncoder.SetFragmentTexture(texture, index);
}

void MetalDriver::draw(GraphicsPrimitiveType primitiveType, uint32_t vertexCount, uint32_t vertexStart)
{
    EL_ASSERT(_context->renderCommandEncoder);
    _context->renderCommandEncoder.Draw(asMetalPrimitiveType(primitiveType), vertexStart, vertexCount);
}

_EL_NAME_END
