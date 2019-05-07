#include <graphics_program.h>

using namespace el;

GraphicsProgram::GraphicsProgram()
{
}

GraphicsProgram::~GraphicsProgram()
{
}

GraphicsProgramDesc::GraphicsProgramDesc()
{
}

void GraphicsProgramDesc::addShader(const GraphicsShaderPtr& ptr)
{
    _shaders.push_back(ptr);
}

const GraphicsShaders& GraphicsProgramDesc::getShaders() const
{
    return _shaders;
}