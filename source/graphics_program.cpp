#include "graphics_program.h"

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

void GraphicsProgramDesc::addShader(GraphicsShaderPtr ptr) 
{
	_shaders.push_back(std::move(ptr));
}

const GraphicsShaders& GraphicsProgramDesc::getShaders() const
{
	return _shaders;
}

