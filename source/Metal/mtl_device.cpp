#include "mtl_device.h"

#if EL_PLAT_APPLE 

#include "debug.h"
#include "mtl_program.h"
#include "mtl_shader.h"
#include "mtl_texture.h"
#include "mtl_buffer.h"
#include "mtl_device_context.h"

using namespace el;

bool MTLDevice::create(GraphicsDeviceDesc desc)
{
    _device = mtlpp::Device::CreateSystemDefaultDevice();
    if (!_device) return false;
    
    _commandQueue = _device.NewCommandQueue();
    if (!_commandQueue) return false;

	return true;
}

GraphicsProgramPtr MTLDevice::createProgram(GraphicsProgramDesc desc)
{
	auto program = std::make_shared<MTLProgram>();
	if (program->create(std::move(desc)))
		return program;
	return nullptr;
}

GraphicsShaderPtr MTLDevice::createShader(GraphicsShaderDesc desc) 
{
	auto shader = std::make_shared<MTLShader>();
	if (shader->create(desc.getStageFlag(), desc.getShaderCode()))
		return shader;
	return nullptr;
}

GraphicsTexturePtr MTLDevice::createTexture(GraphicsTextureDesc desc)
{
	auto texture = std::make_shared<MTLTexture>();
    if (!texture) return nullptr;
    texture->setDevice(shared_from_this());
	if (texture->create(std::move(desc)))
		return texture;
	return nullptr;
}

GraphicsBufferPtr MTLDevice::createBuffer(GraphicsBufferDesc desc)
{
	auto buffer = std::make_shared<MTLBuffer>();
    if (!buffer) return nullptr;
	if (buffer->create(std::move(desc)))
		return buffer;
	return nullptr;
}

GraphicsDeviceContextPtr MTLDevice::createDeviceContext()
{
	auto deviceContext = std::make_shared<MTLDeviceContext>();
    if (!deviceContext)
        return nullptr;
    deviceContext->setDevice(shared_from_this());
	if (!deviceContext->create())
        return nullptr;;
	return deviceContext;
}

mtlpp::Device& MTLDevice::getDevice()
{
    return _device;
}

mtlpp::CommandQueue& MTLDevice::getCommandQueue()
{
    return _commandQueue;
}

#endif // EL_PLAT_APPLE
