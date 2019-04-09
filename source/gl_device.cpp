#include "gl_device.h"
#include "gl_program.h"
#include "gl_shader.h"
#include "gl_texture.h"
#include "gl_buffer.h"

using namespace el;

bool GLDevice::create(GraphicsDeviceDesc desc)
{
	// https://www.khronos.org/opengl/wiki/GLSL_:_common_mistakes#Enable_Or_Not_To_Enable
	// For fixed pipeline
	// glEnable(GL_TEXTURE_2D);
	return true;
}

GraphicsProgramPtr GLDevice::createProgram(GraphicsProgramDesc desc)
{
	auto program = std::make_shared<GLProgram>();
	if (program->create(std::move(desc)))
		return program;
	return nullptr;
}

GraphicsShaderPtr GLDevice::createShader(GraphicsShaderDesc desc) 
{
	auto shader = std::make_shared<GLShader>();
	if (shader->create(desc.getStageFlag(), desc.getShaderCode()))
		return shader;
	return nullptr;
}

GraphicsTexturePtr GLDevice::createTexture(GraphicsTextureDesc desc)
{
	auto texture = std::make_shared<GLTexture>();
	if (texture->create(std::move(desc)))
		return texture;
	return nullptr;
}

GraphicsBufferPtr GLDevice::createBuffer(GraphicsBufferDesc desc)
{
	auto buffer = std::make_shared<GLBuffer>();
	if (buffer->create(std::move(desc)))
		return buffer;
	return nullptr;
}
