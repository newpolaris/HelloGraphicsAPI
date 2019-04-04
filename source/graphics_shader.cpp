#include "graphics_shader.h"

using namespace el;

GraphicsShaderDesc::GraphicsShaderDesc() :
	_stage(GraphicsShaderStageFlagBitsMaxEnum),
	_shaderCode(nullptr)
{
}

void GraphicsShaderDesc::setStage(GraphicsShaderStageFlagBits stage)
{
	_stage = stage;
}

GraphicsShaderStageFlagBits GraphicsShaderDesc::getStage() const
{
	return _stage;
}

void GraphicsShaderDesc::setShaderCode(const char* code)
{
	_shaderCode = code;
}

const char* GraphicsShaderDesc::getShaderCode() const
{
	return _shaderCode;
}

GraphicsShader::GraphicsShader()
{
}

GraphicsShader::~GraphicsShader()
{
}

