#include "metal_driver.h"
#include "metal_context.h"
#include <el_debug.h>

namespace el {

MetalDriver::MetalDriver() :
    _context(new MetalContext())
{
    _context->device = mtlpp::Device::CreateSystemDefaultDevice();
    _context->commandQueue = _context->device.NewCommandQueue();
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

void MetalDriver::makeCurrent(SwapchainHandle handle)
{
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

void MetalDriver::beginRenderPass()
{
    
}

void MetalDriver::endRenderPass()
{
}

void MetalDriver::present()
{
}

} // namespace el {