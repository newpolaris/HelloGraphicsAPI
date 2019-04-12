#include "gl_texture.h"
#include "debug.h"
#include "gl_types.h"
#include <GLFW/glfw3.h>

using namespace el;

GLTexture::GLTexture() : 
	_textureID(0u), 
	_target(0u)
{
}

GLTexture::~GLTexture()
{
}

bool GLTexture::create(const GraphicsTextureDesc& desc)
{
	_textureDesc = desc;
	_target = asTextureTarget(desc.getDim());
	
	const GLint border = 0;

	GLint level = 0;
	GLint internalformat = asTextureInternalFormat(desc.getPixelFormat());
	GLenum format = asTextureFormat(desc.getPixelFormat());
	GLenum type = asTextureType(desc.getPixelFormat());

	GL_CHECK(glGenTextures(1, &_textureID));
    if (_textureID == 0)
        return false;
    GL_CHECK(glBindTexture(_target, _textureID));

    uint32_t width = desc.getWidth();
    uint32_t height = desc.getHeight();
    uint32_t depth = desc.getDepth();

    const stream_t* stream = desc.getStream();

    // https://retokoradi.com/2014/03/30/opengl-transition-to-core-profile/
    // TODO: OpenGL Core
    //  GL_LUMINANCE -> GL_RED
    //  GL_LUMINANCE_ALPHA -> GL_RG
    GL_CHECK(glTexImage2D(_target, level, internalformat, width, height, border, format, type, stream));
    GL_CHECK(glTexParameteri(_target, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
    GL_CHECK(glTexParameteri(_target, GL_TEXTURE_MAG_FILTER, GL_NEAREST));

    return true;
}

void GLTexture::destroy()
{
    // No need to unboud before deleting and silently ignores 0
    if (!_textureID)
    {
        GL_CHECK(glDeleteTextures(1, &_textureID));
        _textureID = 0;
    }
}

void GLTexture::bind(GLuint unit) const
{
    EL_ASSERT(_textureID != 0u);
    GL_CHECK(glActiveTexture(GL_TEXTURE0 + unit));
    GL_CHECK(glBindTexture(_target, _textureID));
}

void GLTexture::unbind(GLuint unit) const
{
    EL_ASSERT(_textureID != 0u);
    GL_CHECK(glActiveTexture(GL_TEXTURE0 + unit));
    GL_CHECK(glBindTexture(_target, 0));
}

GLuint GLTexture::getTextureID() const
{
    return _textureID;
}

const GraphicsTextureDesc& GLTexture::getTextureDesc() const
{
    return _textureDesc;
}

