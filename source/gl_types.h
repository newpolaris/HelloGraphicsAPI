#include "gl.h"
#include "graphics_types.h"
#include "debug.h"

namespace el {
    GLenum getShaderStage(GraphicsShaderStageFlagBits stage);
    GLenum asTextureTarget(GraphicsTextureDim);
    GLenum asTextureType(GraphicsPixelFormat format);
    GLenum asTextureFormat(GraphicsPixelFormat textureFormat);
    GLint  asTextureInternalFormat(GraphicsPixelFormat format);
}
