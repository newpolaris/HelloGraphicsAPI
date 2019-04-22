#pragma once

#include "graphics_types.h"
#include "graphics_program.h"
#include "gl_types.h"
#include <linmath.h>
#include <string>
#include <map>

namespace el {

    struct GLUniform
    {
        GLint location;
        GLenum type;
        GLint size;
        GLuint unit;
        std::string name;
    };

    struct GLAttribute
    {
        GLint location;
        GLenum type;
        GLint size;
        std::string name;
    };

    class GLProgram final : public GraphicsProgram
    {
    public:

        GLProgram();
        ~GLProgram();

        bool create(GraphicsProgramDesc desc);
        void destory();

        GLuint getProgramID() const;

        void apply() const;

        void setUniform(GLint location, const GLint& v0);
        void setUniform(GLint location, const vec3& v0);
        void setUniform(GLint location, const mat4x4& m0);

        void setUniform(const std::string& name, const vec3& v0);
        void setUniform(const std::string& name, const mat4x4& m0);
        void setUniform(const std::string& name, const GraphicsTexturePtr& texture);
        void setVertexBuffer(const GraphicsStoragePtr& buffer);
        void setVertexBuffer(const std::string& name, const GraphicsStoragePtr& buffer, uint32_t stride, uint32_t offset);
        void setVertexBuffer(GLint location, GLint size, GLenum type, GLsizei stride, const void *pointer);
        void setVertexBuffer(GLint location, const GraphicsStoragePtr& buffer, GLint size, GLenum type, GLsizei stride, GLsizei offset);
        void setIndexBuffer(const GraphicsStoragePtr& buffer);
        void setTexture(GLint location, const GraphicsTexturePtr& texture, GLenum unit);
        void setInputLayout(const GraphicsInputLayoutPtr& layout);

        const GraphicsProgramDesc& getProgramDesc() const override;

    private:

        void setupActiveUniform();
        void setupActiveAttribute();

        GLuint _programID;

        GraphicsInputLayoutPtr _inputLayout;

        std::map<std::string, GLUniform> _activeUniform;
        std::map<std::string, GLAttribute> _activeAttribute;

        GraphicsProgramDesc _programDesc;
    };

} // namespace el {
