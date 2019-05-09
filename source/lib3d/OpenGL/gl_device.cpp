#include <OpenGL/gl_device.h>
#include <OpenGL/gl_program.h>
#include <OpenGL/gl_shader.h>
#include <OpenGL/gl_texture.h>
#include <OpenGL/gl_buffer.h>
#include <OpenGL/gl_context.h>
#include <OpenGL/gl_input_layout.h>

using namespace el;

bool GLDevice::create(const GraphicsDeviceDesc& desc)
{
	// https://www.khronos.org/opengl/wiki/GLSL_:_common_mistakes#Enable_Or_Not_To_Enable
	// For fixed pipeline
	// glEnable(GL_TEXTURE_2D);

    // Max Aniso Level query
    // bSupportAnisotropy
    
	return true;
}

GraphicsProgramPtr GLDevice::createProgram(const GraphicsProgramDesc& desc)
{
	auto program = std::make_shared<GLProgram>();
    if (program == nullptr)
        return nullptr;
	if (program->create(desc))
		return program;
	return nullptr;
}

GraphicsShaderPtr GLDevice::createShader(const GraphicsShaderDesc& desc) 
{
	auto shader = std::make_shared<GLShader>();
    if (shader == nullptr)
        return nullptr;
	if (shader->create(desc))
		return shader;
	return nullptr;
}

GraphicsTexturePtr GLDevice::createTexture(const GraphicsTextureDesc& desc)
{
	auto texture = std::make_shared<GLTexture>();
    if (texture == nullptr)
        return nullptr;
	if (texture->create(desc))
		return texture;
	return nullptr;
}

GraphicsDataPtr GLDevice::createBuffer(const GraphicsDataDesc& desc)
{
	auto buffer = std::make_shared<GLBuffer>();
    if (buffer == nullptr)
        return nullptr;
	if (buffer->create(desc))
		return buffer;
	return nullptr;
}

GraphicsInputLayoutPtr GLDevice::createInputLayout(const GraphicsInputLayoutDesc& desc)
{
    auto layout = std::make_shared<GLInputLayout>();
    if (layout == nullptr)
        return nullptr;
    if (layout->create(desc))
        return layout;
    return nullptr;
}

GraphicsDepthStencilPtr GLDevice::createDepthStencil(const GraphicsDepthStencilDesc& desc)
{
    return nullptr;
}

GraphicsContextPtr GLDevice::createContext()
{
    auto context = std::make_shared<GLContext>();
    if (context == nullptr)
        return nullptr;
    if (context->create())
        return context;
    return nullptr;
}
