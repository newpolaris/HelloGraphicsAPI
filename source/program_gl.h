#pragma once

#include <stdint.h>

#include "gl.h"
#include "shader_gl.h"

namespace el {

class GraphicsProgramDesc final
{
public:

	GraphicsProgramDesc();

	void addShader(GraphicsShaderPtr ptr);

	const GraphicsShaders& getShaders() const;

private:

	GraphicsShaders _shaders;
};

class GraphicsProgram
{
public:

	GraphicsProgram();
	virtual ~GraphicsProgram();

	virtual const GraphicsProgramDesc& getGraphicsProgramDesc() const noexcept = 0;
};

class GLProgram final : public GraphicsProgram
{
public:

	GLProgram();
	~GLProgram();

	bool create(const GraphicsProgramDesc& desc);
	void destory();

	GLuint GetID() const;

	const GraphicsProgramDesc& getGraphicsProgramDesc() const noexcept;

private:

	GraphicsProgramDesc _desc;
	GLuint _id;
};

} // namespace el {
