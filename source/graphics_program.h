#pragma once

#include "graphics_types.h"

namespace el {

    class GraphicsVertexAttribute
    {
    public:

        virtual bool isMatch(const GraphicsInputAttribute& layout) const = 0;
    };

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

		GraphicsProgram(const GraphicsProgram&);
		GraphicsProgram& operator=(const GraphicsProgram&);

		virtual const GraphicsProgramDesc& getProgramDesc() const = 0;
        virtual const GraphicsVertexAttributes& getVertexAttributes() const = 0;
	};

} // namespace el {
