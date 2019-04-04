#include "gl_device.h"
#include "gl_program.h"
#include "gl_shader.h"
#include "gl_texture.h"

using namespace el;

bool GLDevice::create(const GraphicsDeviceDesc& desc)
{
	return true;
}

GraphicsProgramPtr GLDevice::createProgram(const GraphicsProgramDesc& desc)
{
	auto program = std::make_shared<GLProgram>();
	if (program->create(desc))
		return program;
	return nullptr;
}


GraphicsShaderPtr GLDevice::createShader(const GraphicsShaderDesc& desc) 
{
	auto shader = std::make_shared<GLShader>();
	if (shader->create(desc.getStage(), desc.getShaderCode()))
		return shader;
	return nullptr;
}

GraphicsTexturePtr GLDevice::createTexture(const GraphicsTextureDesc& desc)
{
	auto texture = std::make_shared<GLTexture>();
	if (texture->create(desc))
		return texture;
	return nullptr;
}

