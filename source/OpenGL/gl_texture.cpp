#include "gl_texture.h"
#include "debug.h"
#include "gl_types.h"

using namespace el;

GLTexture::GLTexture() : 
	_textureID(0u), 
	_target(0u),
    _levels(1),
    _wrapS(GL_REPEAT),
    _wrapT(GL_REPEAT),
    _wrapR(GL_REPEAT),
    _minFilter(GL_NEAREST_MIPMAP_LINEAR),
    _magFilter(GL_LINEAR),
    _anisotropyLevel(0)
{
}

GLTexture::~GLTexture()
{
    destroy();
}

bool GLTexture::create(GraphicsTextureDesc desc)
{
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

	const stream_t* stream = desc.getStream();

	GL_CHECK(glTexImage2D(_target, level, internalformat, width, height, border, format, type, stream));

	EL_ASSERT(_target != 0);
	EL_ASSERT(_textureID != 0);

    GLenum defaultWrap = GL_REPEAT;
    auto wrapS = desc.getWrapS();
    auto wrapT = desc.getWrapT();
    auto wrapR = desc.getWrapR();

    if (wrapS != defaultWrap)
        GL_CHECK(glTexParameteri(GL_TEXTURE_WRAP_S, wrapS));
    if (wrapT != defaultWrap)
        GL_CHECK(glTexParameteri(GL_TEXTURE_WRAP_T, wrapS));
    if (wrapR != defaultWrap)
        GL_CHECK(glTexParameteri(GL_TEXTURE_WRAP_R, wrapS));

    auto minFilter = desc.getMinFilter();
    auto magFilter = desc.getMagFilter();
    auto defaultMinFilter = GL_LINEAR_MIPMAP_LINEAR;
    auto defaultMagFilter = GL_LINEAR;
    EL_ASSERT(magFilter == GL_NEAREST || magFilter == GL_LINEAR);
    if (minFilter != defaultMinFilter)
        GL_CHECK(glTexParameteri(GL_TEXTURE_MIN_FILTER, minFilter));
    if (magFilter != defaultMagFilter)
        GL_CHECK(glTexParameteri(GL_TEXTURE_MAG_FILTER, magFilter));

    GLfloat defaultAnisoLevel = 0;
    auto anisoLevel = desc.getAnisotropyLevel();
    if (anisoLevel > defaultAnisoLevel)
        GL_CHECK(glTextureParameterf(GL_TEXTURE_MAX_ANISOTROPY_EXT, anisoLevel));

	_textureDesc = std::move(desc);

	return true;
}

void GLTexture::destroy()
{
    GL_CHECK(glDeleteTextures(1, &_textureID));
    _textureID = 0;
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
