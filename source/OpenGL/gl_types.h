#include "gl.h"
#include "graphics_types.h"
#include "debug.h"

namespace el {
    typedef std::shared_ptr<class GLProgram> GLProgramPtr;

    bool isSamplerType(GLenum type);

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
}
