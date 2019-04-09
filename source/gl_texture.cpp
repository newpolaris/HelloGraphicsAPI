#include "gl_texture.h"
#include "debug.h"

// TODO:
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

bool GLTexture::create(GraphicsTextureDesc desc)
{
	_target = GL_TEXTURE_2D;

	GL_CHECK(glGenTextures(1, &_textureID));
	GL_CHECK(glBindTexture(GL_TEXTURE_2D, _textureID));

	int x, y;
	char pixels[16 * 16];

	srand((unsigned int)glfwGetTimerValue());

	for (y = 0;  y < 16;  y++)
	{
		for (x = 0;  x < 16;  x++)
			pixels[y * 16 + x] = rand() % 256;
	}

	// https://retokoradi.com/2014/03/30/opengl-transition-to-core-profile/
	// TODO: OpenGL Core
	//  GL_LUMINANCE -> GL_RED
	//  GL_LUMINANCE_ALPHA -> GL_RG
	GL_CHECK(glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, 16, 16, 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, pixels));
	GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
	GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));

	_textureDesc = std::move(desc);

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

	// _Format = GL_INVALID_ENUM;
	// _Target = GL_INVALID_ENUM;
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

