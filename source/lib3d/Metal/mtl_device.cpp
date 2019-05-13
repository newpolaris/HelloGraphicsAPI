#include "mtl_device.h"

#if EL_BUILD_METAL 

#include <el_debug.h>
#include <Metal/mtl_program.h>
#include "mtl_shader.h"
#include <Metal/mtl_texture.h>
#include <Metal/mtl_buffer.h>
#include <Metal/mtl_context.h>
#include <Metal/mtl_input_layout.h>
#include <Metal/mtl_depth_stencil.h>

using namespace el;

bool MTLDevice::create(const GraphicsDeviceDesc& desc)
{
    _metalDevice = mtlpp::Device::CreateSystemDefaultDevice();
    if (!_metalDevice) return false;
    
    _commandQueue = _metalDevice.NewCommandQueue();
    if (!_commandQueue) return false;

	return true;
}

MTLDevice::MTLDevice()
{
}

MTLDevice::~MTLDevice()
{
    destroy();
}

void MTLDevice::destroy()
{
    _commandQueue = ns::Handle{};
    _metalDevice = ns::Handle{};
}

GraphicsProgramPtr MTLDevice::createProgram(const GraphicsProgramDesc& desc)
{
	auto program = std::make_shared<MTLProgram>();
	if (program->create(desc))
		return program;
	return nullptr;
}

GraphicsShaderPtr MTLDevice::createShader(const GraphicsShaderDesc& desc) 
{
	auto shader = std::make_shared<MTLShader>();
    if (!shader) return nullptr;
    shader->setDevice(shared_from_this());
	if (shader->create(desc))
		return shader;
	return nullptr;
}

GraphicsTexturePtr MTLDevice::createTexture(const GraphicsTextureDesc& desc)
{
	auto texture = std::make_shared<MTLTexture>();
    if (!texture) return nullptr;
    texture->setDevice(shared_from_this());
	if (texture->create(desc))
		return texture;
	return nullptr;
}

GraphicsDataPtr MTLDevice::createBuffer(const GraphicsDataDesc& desc)
{
	auto buffer = std::make_shared<MTLBuffer>();
    if (!buffer) return nullptr;
	if (buffer->create(desc))
		return buffer;
	return nullptr;
}

GraphicsInputLayoutPtr MTLDevice::createInputLayout(const GraphicsInputLayoutDesc& desc)
{
    auto inputLayout = std::make_shared<MTLInputLayout>();
    if (!inputLayout)
        return nullptr;
    inputLayout->setDevice(shared_from_this());
    if (inputLayout->create(desc))
        return inputLayout;
    return nullptr;
}

GraphicsDepthStencilPtr MTLDevice::createDepthStencil(const GraphicsDepthStencilDesc& desc)
{
    auto depthStencil = std::make_shared<MTLDepthStencil>();
    if (!depthStencil)
        return nullptr;
    depthStencil->setDevice(shared_from_this());
    if (depthStencil->create(desc))
        return depthStencil;
    return nullptr;
}

GraphicsContextPtr MTLDevice::createContext()
{
	auto deviceContext = std::make_shared<MTLContext>();
    if (!deviceContext)
        return nullptr;
    deviceContext->setDevice(shared_from_this());
	if (!deviceContext->create())
        return nullptr;;
	return deviceContext;
}

mtlpp::Device& MTLDevice::getMetalDevice()
{
    return _metalDevice;
}

mtlpp::CommandQueue& MTLDevice::getCommandQueue()
{
    return _commandQueue;
}

#endif // EL_BUILD_METAL
