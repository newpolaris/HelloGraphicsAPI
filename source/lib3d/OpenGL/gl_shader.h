#ifndef __GL_SHADER_H__
#define __GL_SHADER_H__

#include <graphics_types.h>
#include <graphics_shader.h>
#include <OpenGL/gl_headers.h>

namespace el {

    class GLShader final : public GraphicsShader
    {
    public:

        GLShader();
        ~GLShader();

        bool create(const GraphicsShaderDesc& desc);
        void destroy();

        GLuint getID() const;

        virtual const GraphicsShaderDesc& getDesc() const;

    private:

        GLuint _id;
        GLenum _stage;
        GraphicsShaderDesc _shaderDesc;
    };

} // namespace el {

#endif // __GL_SHADER_H__
