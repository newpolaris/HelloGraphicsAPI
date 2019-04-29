#include "mtl_context.h"

#if EL_BUILD_METAL

#include "debug.h"
#include "mtl_device.h"

using namespace el;

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
    EL_ASSERT(!_commandBuffer);
    
    _commandBuffer = device->getCommandQueue().CommandBuffer();
    EL_ASSERT(!_commandBuffer);
    
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
    _commandBuffer.reset();
}

void MTLContext::setDepthTest(bool isEnable)
{
    // _isDepthTest = isEnable;
}

void MTLContext::setCullFace(bool isEnable)
{
    // _isCullFace = isEnable;
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

void MTLContext::draw(GraphicsPrimitiveType primitive, uint32_t vertexCount, int32_t vertexStartOffset)
{
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

#endif // EL_BUILD_METAL
