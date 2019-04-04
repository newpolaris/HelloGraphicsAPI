#pragma once

#include "graphics_types.h"

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

		virtual const GraphicsProgramDesc& getProgramDesc() const = 0;

	private:

		GraphicsProgram(const GraphicsProgram&) = delete;
		const GraphicsProgram& operator=(const GraphicsProgram&) = delete;
	};

} // namespace el {
