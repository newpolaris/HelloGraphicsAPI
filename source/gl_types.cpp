#include "gl_types.h"
#include "debug.h"

GLenum el::getShaderStage(GraphicsShaderStageFlagBits stage)
{
	switch (stage) {
	case GraphicsShaderStageVertexBit:
		return GL_VERTEX_SHADER;
	case GraphicsShaderStageFragmentBit:
		return GL_FRAGMENT_SHADER;
	default:
		EL_ASSERT(false);
	}
	return GraphicsShaderStageAll;
}

