#ifndef __GRAPHICS_PROGRAM_H__
#define __GRAPHICS_PROGRAM_H__

#include <el/graphics_types.h>

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

		virtual const GraphicsProgramDesc& getProgramDesc() const = 0;
        virtual const GraphicsVertexAttributes& getVertexAttributes() const = 0;

    private:

		GraphicsProgram(const GraphicsProgram&);
		GraphicsProgram& operator=(const GraphicsProgram&);
	};

} // namespace el {

#endif // __GRAPHICS_PROGRAM_H__
