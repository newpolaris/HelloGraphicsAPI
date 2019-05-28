#include  "mtl_context.h"

#if EL_BUILD_METAL

#include <Metal/Metal.h>
#include <QuartzCore/CAMetalLayer.h>
#include <el_debug.h>
#include <mtlpp.hpp>
#include "mtl_device.h"

_EL_NAME_BEGIN

MTLContext::MTLContext()
{
}

MTLContext::~MTLContext()
{
}

bool MTLContext::create()
{
    return true;
}

void MTLContext::destroy()
{
}

void MTLContext::setDevice(GraphicsDevicePtr device)
{
    _device = std::static_pointer_cast<MTLDevice>(std::move(device));
}

GraphicsDevicePtr MTLContext::getDevice()
{
    return _device.lock();
}

#if 0
    VkRenderPassBeginInfo pass = { VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO };
    pass.renderPass = renderpass;
    pass.framebuffer = framebuffer;
    pass.renderArea = scissor;
    pass.clearValueCount = ARRAY_SIZE(clear_values);
    pass.pClearValues = clear_values;

    vkCmdBeginRenderPass(command_buffer, &pass, VK_SUBPASS_CONTENTS_INLINE);
    vkCmdBindPipeline(command_buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline);
    vkCmdSetViewport(command_buffer, 0, 1, &viewport);
    vkCmdSetScissor(command_buffer, 0, 1, &scissor);
#endif
#if 0
- (MTLRenderPassDescriptor *)newRenderPassWithColorAttachmentTexture:(id<MTLTexture>)texture
{
    MTLRenderPassDescriptor *renderPass = [MTLRenderPassDescriptor new];
    
    renderPass.colorAttachments[0].texture = texture;
    renderPass.colorAttachments[0].loadAction = MTLLoadActionClear;
    renderPass.colorAttachments[0].storeAction = MTLStoreActionStore;
    renderPass.colorAttachments[0].clearColor = MBEClearColor;
    
    renderPass.depthAttachment.texture = self.depthTexture;
    renderPass.depthAttachment.loadAction = MTLLoadActionClear;
    renderPass.depthAttachment.storeAction = MTLStoreActionStore;
    renderPass.depthAttachment.clearDepth = 1.0;
    
    return renderPass;
}
#endif

void MTLContext::beginRendering() 
{
    auto device = _device.lock();
    EL_ASSERT(device);
    
    // renderCommandEncoder.SetCullMode(mtlpp::CullMode::Back);
    // renderCommandEncoder.SetCullMode(mtlpp::CullMode::None);
}

void MTLContext::endRendering() 
{
   
}

void MTLContext::finsh(bool waitForCompletion)
{
    EL_ASSERT(_commandBuffer);
    
    _commandBuffer.Commit();
    if (waitForCompletion)
    {
        _commandBuffer.WaitUntilCompleted();
    }
    _commandBuffer = ns::Handle{};
}

void MTLContext::setDepthTestEnable(bool enable)
{
    // _depthTestEnable = enable;
}

void MTLContext::setCullFaceEnable(bool enable)
{
    // _cullFaceEnable = enable;
}

void MTLContext::setViewport(const Viewport& viewport) 
{
}

void MTLContext::setProgram(const GraphicsProgramPtr& program) 
{
}

void MTLContext::setTexture(const std::string& name, const GraphicsTexturePtr& texture) 
{
}

void MTLContext::setVertexBuffer(const std::string& name, const GraphicsDataPtr& vertexData, uint32_t stride, uint32_t offset) 
{
}

void MTLContext::setVertexBuffer(uint32_t binding, const GraphicsDataPtr& vertexData, uint32_t offset)
{
}

void MTLContext::setIndexBuffer(const GraphicsDataPtr& indexData) 
{
}

void MTLContext::setUniform(const std::string& name, float v0) 
{
}

void MTLContext::setUniform(const std::string& name, const vec3& v0) 
{
}

void MTLContext::setUniform(const std::string& name, const quat& q0) 
{
}

void MTLContext::setUniform(const std::string& name, const mat4x4& m0) 
{
}

void MTLContext::setInputLayout(const GraphicsInputLayoutPtr& inputLayout) 
{
}

enum class PrimitiveTopologyClass
{
    Unspecified = 0,
    Point = 1,
    Line = 2,
    Triangle = 3,
};

using namespace mtlpp;

PrimitiveType getPrimitiveType(GraphicsPrimitiveType type)
{
    switch (type)
    {
        case GraphicsPrimitiveType::GraphicsPrimitiveTypeTriangle:
            return PrimitiveType::Triangle;
        default:
            EL_ASSERT(false);
            return PrimitiveType(0);
    }
}

void MTLContext::draw(GraphicsPrimitiveType primitive, uint32_t vertexCount, int32_t vertexStartOffset)
{
    const mtlpp::RenderPassDescriptor renderPassDescriptor;
    auto renderCommandEncoder = _commandBuffer.RenderCommandEncoder(renderPassDescriptor);
    renderCommandEncoder.Draw(getPrimitiveType(primitive), vertexStartOffset, vertexCount);
}

void MTLContext::drawIndexed(GraphicsPrimitiveType primitive, uint32_t indexCount, uint32_t startIndexLocation)
{
}

void MTLContext::drawIndexed(GraphicsPrimitiveType primitive, uint32_t indexCount, uint32_t startIndexLocation, int32_t baseVertexLocation) 
{
}

void MTLContext::drawInstanced(GraphicsPrimitiveType primitive, uint32_t vertexCountPerInstance, uint32_t instanceCount,
                               uint32_t startVertexLocation, uint32_t startInstanceLocation)
{
}
void MTLContext::drawIndexedInstanced(GraphicsPrimitiveType primitive, uint32_t indexCountPerInstance, uint32_t instanceCount,
                                      uint32_t startIndexLocation, int32_t vaseVertexLocation, uint32_t startInstanceLocation)
{
}

MetalContext::MetalContext() :
    _depthFormat(GraphicsPixelFormatInvalid),
    _swapchainHandle(nullptr),
    _clearColor(0.3f),
    _clearDepth(1.f)
{
}

MetalContext::~MetalContext() {
    _currentCommandBuffer = ns::Handle{};
    _currentDrawable = ns::Handle{};
    _swapchainHandle = nullptr;
    _metalDevice = ns::Handle{};
    _device = nullptr;
}

void MetalContext::setDevice(const GraphicsDevicePtr& device) {
    _device = std::static_pointer_cast<MTLDevice>(device);
    EL_ASSERT(_device);
    _metalDevice = _device->getMetalDevice();
}

mtlpp::Drawable& MetalContext::getCurrentDrawable()
{
    auto layer = reinterpret_cast<CAMetalLayer*>(_swapchainHandle);
    EL_ASSERT(layer);
    if (!_currentDrawable) {
        id<CAMetalDrawable> drawable = [layer nextDrawable];
        EL_ASSERT(drawable);
        _currentDrawable = mtlpp::Drawable(ns::Handle{(__bridge void*)drawable});
        EL_ASSERT(_currentDrawable);
    }
    return _currentDrawable;
}

mtlpp::CommandBuffer& MetalContext::getCurrentCommandBuffer()
{
    EL_ASSERT(_device);

    if (!_currentCommandBuffer)
    {
        auto& queue = _device->getCommandQueue();
        EL_ASSERT(queue);
        _currentCommandBuffer = queue.CommandBuffer();
    }
    return _currentCommandBuffer;
}

void MetalContext::beginFrame(SwapchainHandle handle)
{
    _swapchainHandle = reinterpret_cast<CAMetalLayer*>(handle);
}

void MetalContext::endFrame()
{
    _currentDrawable = ns::Handle{};
    _currentCommandBuffer = ns::Handle{};
    _swapchainHandle = nullptr;
}

void MetalContext::present()
{
    auto& drawable = getCurrentDrawable();
    EL_ASSERT(drawable);

    EL_ASSERT(_currentCommandBuffer);
    _currentCommandBuffer.Present(drawable);
}

void MetalContext::commit(bool isWaitComplete)
{
    EL_ASSERT(_currentCommandBuffer);

    _currentCommandBuffer.Commit(); 
    if (isWaitComplete)
        _currentCommandBuffer.WaitUntilCompleted();

    _depthStencilDesc = GraphicsDepthStencilDesc();
}

void MetalContext::beginPass()
{
    EL_ASSERT(_currentCommandBuffer);

#if 0
    mtlpp::RenderPassDescriptor renderPassDesc;
    auto colorAttachment = renderPassDesc.GetColorAttachments()[0];
    colorAttachment.SetClearColor(makeColor(_clearColor);
    colorAttachment.SetLoadAction(mtlpp::LoadAction::Clear);
    colorAttachment.SetStoreAction(mtlpp::StoreAction::Store);
    colorAttachment.SetTexture(swapchain->_color);
    colorAttachment.SetLevel(0);
    colorAttachment.SetSlice(0);
    if (swapchain->_depth)
    {
        auto depthAttachment = renderPassDesc.GetDepthAttachment();
        depthAttachment.SetTexture(swapchain->_depth);
        depthAttachment.SetClearDepth(_clearDepth);
        depthAttachment.SetLevel(0);
        depthAttachment.SetSlice(0);
        depthAttachment.SetLoadAction(mtlpp::LoadAction::Clear);
        depthAttachment.SetStoreAction(mtlpp::StoreAction::DontCare);
    }

    _renderCommandEncoder = _commandBuffer.RenderCommandEncoder(renderPassDesc);
    EL_ASSERT(_renderCommandEncoder);
#endif
}

void MetalContext::endPass()
{
    _currentCommandEncoder.EndEncoding();
}

void MetalContext::setDepthWriteEnable(bool enable)
{
    _depthStencilDesc.setDepthWriteEnable(enable);
}

void MetalContext::setDepthCompareOp(GraphicsCompareOp compare)
{
    _depthStencilDesc.setDepthCompareOp(compare);
}

void MetalContext::setDepthTestEnable(bool enable)
{
    _depthStencilDesc.setDepthTestEnable(enable);
}

void MetalContext::draw(GraphicsPrimitiveType primitive, uint32_t vertexCount, int32_t vertexStartOffset)
{
    EL_ASSERT(_currentCommandBuffer);
    EL_ASSERT(_currentCommandEncoder);

    auto depthStencilState = _depthStencilCache.request(_depthStencilDesc);
    _currentCommandEncoder.SetDepthStencilState(depthStencilState);

    _currentCommandEncoder.Draw(mtlpp::PrimitiveType::Triangle, vertexStartOffset, vertexCount);
}

_EL_NAME_END

#endif // EL_BUILD_METAL
