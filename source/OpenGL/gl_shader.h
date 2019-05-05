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

        bool create(GraphicsShaderStageFlagBits stage, const char* shaderCode);
        void destroy();

        GLuint getID() const;

        virtual const GraphicsShaderDesc& getDesc() const;

    private:

        GLuint _id;
        GraphicsShaderStageFlagBits _stage;
        GraphicsShaderDesc _shaderDesc;
    };

} // namespace el {

#endif // __GL_SHADER_H__
