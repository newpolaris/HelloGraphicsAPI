#pragma once

#include "gl.h"
#include "graphics_types.h"
#include "debug.h"

namespace el {
    typedef std::shared_ptr<class GLProgram> GLProgramPtr;
}

namespace el {
    const GLenum defaultWrap = GL_REPEAT;
    const GLenum defaultMinFilter = GL_LINEAR_MIPMAP_LINEAR;
    const GLenum defaultMagFilter = GL_LINEAR;
    const GLfloat defaultAnisoLevel = 0;
    const GLint defaultPixelAlignement = 4;

    bool isSamplerType(GLenum type);
    GLenum asIndexType(GraphicsIndexType type);

	GLenum asPrimitiveType(GraphicsPrimitiveType primitiveType);
    GLenum getShaderStage(GraphicsShaderStageFlagBits stage);

    GLuint asVariableComponentCount(GLenum type);
    GLuint asVariableRowCount(GLenum type);
    GLuint asVariableColumnCount(GLenum type);
    GLuint asVariableComponentSize(GLenum type);
    GLenum asVariableComponentType(GLenum type);
    GLuint asVariableInternalSize(GLenum type);
    GLuint asVariableExternalSize(GLenum type);
    std::string asVariableComponentTypename(GLenum type);

    GLenum asTextureTarget(GraphicsTextureDim);
    GLenum asTextureType(GraphicsPixelFormat format);
    GLenum asTextureFormat(GraphicsPixelFormat textureFormat);
    GLint  asTextureInternalFormat(GraphicsPixelFormat format);
    GLenum asSamplerAddressMode(GraphicsSamplerAddressMode mode);
    GLenum asSamplerMinFilter(GraphicsFilter filter, GraphicsSamplerMipmapMode mode);
    GLenum asSamplerMagFilter(GraphicsFilter filter);
}
