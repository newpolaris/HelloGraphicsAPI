#include "mtl_context.h"

#if EL_PLAT_APPLE

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
    auto device = _device.lock();
    if (!device) return false;

    // TODO: need to texture generate ?
    _commandBuffer = device->getCommandQueue().CommandBuffer();
    if (!_commandBuffer) return false;

    return true;
}

void MTLContext::destroy()
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
}

void MTLContext::setDevice(GraphicsDevicePtr device)
{
    _device = std::static_pointer_cast<MTLDevice>(std::move(device));
}

GraphicsDevicePtr MTLContext::getDevice()
{
    return _device.lock();
}

void MTLContext::beginRendering() 
{
}

void MTLContext::endRendering() 
{
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

#endif // EL_PLAT_APPLE
