#pragma once

#include "predefine.h"
#include <memory>
#include <vector>

namespace el {

	class GraphicsDeviceDesc;
	class GraphicsProgramDesc;
	class GraphicsShaderDesc;

	typedef std::shared_ptr<class GraphicsDevice> GraphicsDevicePtr;
	typedef std::shared_ptr<class GraphicsContext> GraphicsContextPtr;

	typedef std::shared_ptr<class GraphicsShader> GraphicsShaderPtr;
	typedef std::shared_ptr<class GraphicsProgram> GraphicsProgramPtr;
	typedef std::vector<GraphicsShaderPtr> GraphicsShaders;

	enum GraphicsDeviceType
	{
		GraphicsDeviceTypeOpenGL = 0x00000000,
		GraphicsDeviceTypeOpenGLCore = 0x00000001,
		GraphicsDeviceTypeOpenGLES= 0x00000002,
		GraphicsDeviceTypeMetal = 0x00000003,
	};

} // namespace el
