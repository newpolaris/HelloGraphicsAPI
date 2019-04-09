#include "mtl_texture.h"
#include "debug.h"

// TODO:
#include <GLFW/glfw3.h>

using namespace el;

MTLTexture::MTLTexture()
{
}

MTLTexture::~MTLTexture()
{
}

bool MTLTexture::create(GraphicsTextureDesc desc)
{

	_textureDesc = std::move(desc);

	return true;
}

void MTLTexture::destroy()
{
}

const GraphicsTextureDesc& MTLTexture::getTextureDesc() const
{
	return _textureDesc;
}

