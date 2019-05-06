#include "graphics_shader.h"

using namespace el;

GraphicsShaderDesc::GraphicsShaderDesc() :
    _stage(GraphicsShaderStageFlagBitsMaxEnum),
    _shaderCode(nullptr),
    _shaderCodeSize(0)
{
}

void GraphicsShaderDesc::setStageFlag(GraphicsShaderStageFlagBits stage)
{
    _stage = stage;
}

GraphicsShaderStageFlagBits GraphicsShaderDesc::getStageFlag() const
{
    return _stage;
}

void GraphicsShaderDesc::setShaderCode(const char* code)
{
    _shaderCode = code;
}

void GraphicsShaderDesc::setShaderCodeSize(size_t size)
{
    _shaderCodeSize = size;
}

const char* GraphicsShaderDesc::getShaderCode() const
{
    return _shaderCode;
}

size_t GraphicsShaderDesc::getShaderCodeSize() const
{
    return _shaderCodeSize;
}

GraphicsShader::GraphicsShader()
{
}

GraphicsShader::~GraphicsShader()
{
}

