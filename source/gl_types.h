#include "gl.h"
#include "graphics_types.h"
#include "debug.h"

namespace el {
	GLenum getShaderStage(GraphicsShaderStageFlagBits stage);
	GLenum asTextureType(GraphicsPixelFormat format);
	GLenum asTextureFormat(GraphicsPixelFormat textureFormat);
	GLenum asTextureInternalFormat(GraphicsPixelFormat format);
}
