#pragma once

#include "graphics_types.h"

namespace el {

class GraphicsShaderDesc final
{
public:

	GraphicsShaderDesc();

	void setStage(GraphicsShaderStageFlagBits stage);
	GraphicsShaderStageFlagBits getStage() const;

	void setShaderCode(const char* code);
	const char* getShaderCode() const;

private:

	GraphicsShaderStageFlagBits _stage;
	const char* _shaderCode;
};

class GraphicsShader
{
public:

	GraphicsShader();
	virtual ~GraphicsShader();

	virtual const GraphicsShaderDesc& getDesc() const = 0;

private:

	GraphicsShader(const GraphicsShader&) = delete;
	GraphicsShader& operator=(const GraphicsShader&) = delete;
};

} // namespace el {
