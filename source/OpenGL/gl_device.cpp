#include "gl_device.h"
#include "gl_program.h"
#include "gl_shader.h"
#include "gl_texture.h"
#include "gl_buffer.h"
#include "gl_context.h"

using namespace el;

bool GLDevice::create(GraphicsDeviceDesc desc)
{
	// https://www.khronos.org/opengl/wiki/GLSL_:_common_mistakes#Enable_Or_Not_To_Enable
	// For fixed pipeline
	// glEnable(GL_TEXTURE_2D);

    // Max Aniso Level query
    // bSupportAnisotropy
    
	return true;
}

GraphicsProgramPtr GLDevice::createProgram(GraphicsProgramDesc desc)
{
	auto program = std::make_shared<GLProgram>();
    if (program == nullptr)
        return nullptr;
	if (program->create(std::move(desc)))
		return program;
	return nullptr;
}

GraphicsShaderPtr GLDevice::createShader(GraphicsShaderDesc desc) 
{
	auto shader = std::make_shared<GLShader>();
    if (shader == nullptr)
        return nullptr;
	if (shader->create(desc.getStageFlag(), desc.getShaderCode()))
		return shader;
	return nullptr;
}

GraphicsTexturePtr GLDevice::createTexture(GraphicsTextureDesc desc)
{
	auto texture = std::make_shared<GLTexture>();
    if (texture == nullptr)
        return nullptr;
	if (texture->create(std::move(desc)))
		return texture;
	return nullptr;
}

GraphicsDataPtr GLDevice::createBuffer(GraphicsDataDesc desc)
{
	auto buffer = std::make_shared<GLBuffer>();
    if (buffer == nullptr)
        return nullptr;
	if (buffer->create(std::move(desc)))
		return buffer;
	return nullptr;
}

GraphicsContextPtr GLDevice::createDeviceContext()
{
    auto context = std::make_shared<GLContext>();
    if (context == nullptr)
        return nullptr;
    if (context->create())
        return context;
    return nullptr;
}