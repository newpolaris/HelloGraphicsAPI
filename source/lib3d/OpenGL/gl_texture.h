#ifndef __GL_TEXTURE_H__
#define __GL_TEXTURE_H__

#include <graphics_types.h>
#include <graphics_texture.h>
#include "gl_headers.h"
#include "gl_types.h"

namespace el
{
    class GLTexture final : public GraphicsTexture
    {
    public:

        GLTexture();
        ~GLTexture();

        bool create(GraphicsTextureDesc desc);
        void destroy();

        void bind(GLuint unit) const; 
        void unbind(GLuint unit) const; 

        GLuint getTextureID() const;

        const GraphicsTextureDesc& getTextureDesc() const override;

    private:

        GLuint _textureID;
        GLenum _target;

        GraphicsTextureDesc _textureDesc;
    };
} // namespace el

#endif // __GL_TEXTURE_H__
