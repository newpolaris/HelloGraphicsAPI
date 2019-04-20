#pragma once

#include "graphics_types.h"

namespace el {

	class GraphicsProgramDesc final
	{
	public:

		GraphicsProgramDesc();

		void addShader(const GraphicsShaderPtr& ptr);

		const GraphicsShaders& getShaders() const;

	private:

		GraphicsShaders _shaders;
	};

	class GraphicsProgram
	{
	public:

		GraphicsProgram();
		virtual ~GraphicsProgram();

		GraphicsProgram(const GraphicsProgram&) = delete;
		GraphicsProgram& operator=(const GraphicsProgram&) = delete;

		virtual const GraphicsProgramDesc& getProgramDesc() const = 0;
	};

} // namespace el {
