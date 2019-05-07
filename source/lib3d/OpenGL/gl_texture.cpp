#include <OpenGL/gl_texture.h>

#include <el_debug.h>

using namespace el;

GLTexture::GLTexture() : 
    _textureID(0u), 
    _target(0u)
{
}

GLTexture::~GLTexture()
{
    destroy();
}

bool GLTexture::create(GraphicsTextureDesc desc)
{
    // TODO:
    // int value;
    // glGetIntegerv(GL_MAX_TEXTURE_SIZE, &value);   //Returns 1 value
    // MaxTexture2DWidth = MaxTexture2DHeight = value;

    _target = asTextureTarget(desc.getDim());

    GL_CHECK(glGenTextures(1, &_textureID));
    if (_textureID == 0)
        return false;

    EL_ASSERT(_target != 0);
    EL_ASSERT(_textureID != 0);

    GL_CHECK(glBindTexture(_target, _textureID));

    const GLint border = 0;

    const GLint levels = desc.getLevels();

    // TODO: support multi level images
    EL_ASSERT(levels == 1);
    const GLint level = 0;

    // https://stackoverflow.com/questions/34497195/difference-between-format-and-internalformat
    GLint internalformat = asTextureInternalFormat(desc.getPixelFormat());
    GLenum format = asTextureFormat(desc.getPixelFormat());

    GLenum type = asTextureType(desc.getPixelFormat());

    uint32_t width = desc.getWidth();
    uint32_t height = desc.getHeight();

    const stream_t* stream = desc.getStream();

    auto pixelAlignment = static_cast<GLint>(desc.getPixelAlignment());

    GLint oldPixelAlignment = defaultPixelAlignement;
    GL_CHECK(glGetIntegerv(GL_UNPACK_ALIGNMENT, &oldPixelAlignment));
    if (oldPixelAlignment != pixelAlignment)
        GL_CHECK(glPixelStorei(GL_UNPACK_ALIGNMENT, pixelAlignment));

    GL_CHECK(glTexImage2D(_target, level, internalformat, width, height, border, format, type, stream));

    if (oldPixelAlignment != pixelAlignment)
        GL_CHECK(glPixelStorei(GL_UNPACK_ALIGNMENT, oldPixelAlignment));

    // TODO: GL-2.1
    // glTexSubImage2D

#if 0
    glTexSubImage2D
        GLsizei mapSize = w * h * num;
    if (_pboSize < mapSize)
    {
        glBufferData(GL_PIXEL_PACK_BUFFER, mapSize, nullptr, GL_STREAM_READ);
        _pboSize = mapSize;
    }

    glBindTexture(_target, _texture);
    glGetTexImage(_target, mipLevel, format, type, 0);

    *data = glMapBufferRange(GL_PIXEL_PACK_BUFFER, 0, mapSize, GL_MAP_READ_BIT);
#endif

    auto wrapS = asTextureWrapMode(desc.getAddressModeU());
    auto wrapT = asTextureWrapMode(desc.getAddressModeV());
    auto wrapR = asTextureWrapMode(desc.getAddressModeW());

    if (wrapS != defaultWrap)
        GL_CHECK(glTexParameteri(_target, GL_TEXTURE_WRAP_S, wrapS));
    if (wrapT != defaultWrap)
        GL_CHECK(glTexParameteri(_target, GL_TEXTURE_WRAP_T, wrapS));
    if (wrapR != defaultWrap)
        GL_CHECK(glTexParameteri(_target, GL_TEXTURE_WRAP_R, wrapS));

    auto minFilter = desc.getMinFilter();
    auto magFilter = desc.getMagFilter();
    auto mipFilter = desc.getSamplerMipmapMode();

    GLenum glMinFilter = asSamplerMinFilter(minFilter, mipFilter);
    GLenum glMagFilter = asSamplerMagFilter(magFilter);

    EL_ASSERT(glMagFilter == GL_NEAREST || glMagFilter == GL_LINEAR);

    if (glMinFilter != defaultMinFilter)
        GL_CHECK(glTexParameteri(_target, GL_TEXTURE_MIN_FILTER, glMinFilter));
    if (glMagFilter != defaultMagFilter)
        GL_CHECK(glTexParameteri(_target, GL_TEXTURE_MAG_FILTER, glMagFilter));

#if 0 // TODO:
    // bSupportAnisotropy
    auto anisoLevel = desc.getAnisotropyLevel();
    if (anisoLevel > defaultAnisoLevel)
        GL_CHECK(glTexParameterf(_target, GL_TEXTURE_MAX_ANISOTROPY_EXT, anisoLevel));
#endif

    GL_CHECK(glBindTexture(_target, 0));

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
