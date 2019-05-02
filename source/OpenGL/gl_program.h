#pragma once

#include "graphics_types.h"
#include "graphics_program.h"
#include "gl_types.h"
#include <linmath.h>
#include <string>
#include <map>

namespace el {

    struct Limits
    {
        Limits() :
            maxVertexAttributes(8)
        {
        }

        GLint maxVertexAttributes;
    };

    struct GLUniform
    {
        GLint location;
        GLenum type;
        GLint count;
        GLuint unit;
        std::string name;
    };

    class GLVertexAttribute final : public GraphicsVertexAttribute
    {
    public:

        bool isMatch(const GraphicsInputAttribute& layout) const override;

        std::string name;
        GLint location;
        GLenum type;
        GLint count;
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

        void setUniform(const std::string& name, float v0);
        void setUniform(const std::string& name, const vec3& v0);
        void setUniform(const std::string& name, const vec4& v0);
        void setUniform(const std::string& name, const mat4x4& m0);
        void setUniform(const std::string& name, const GraphicsTexturePtr& texture);

        void setVertexBuffer(const GraphicsDataPtr& buffer);
        void setVertexBuffer(const std::string& name, const GraphicsDataPtr& buffer, uint32_t stride, uint32_t offset);
        void setVertexBuffer(GLint location, GLint size, GLenum type, GLsizei stride, const void *pointer);
        void setVertexBuffer(GLint location, const GraphicsDataPtr& buffer, GLint size, GLenum type, GLsizei stride, GLsizei offset);
        void setIndexBuffer(const GraphicsDataPtr& buffer);
        void setTexture(GLint location, const GraphicsTexturePtr& texture, GLenum unit);
        void setInputLayout(const GraphicsInputLayoutPtr& layout);

        const GraphicsProgramDesc& getProgramDesc() const override;
        const GraphicsVertexAttributes& getVertexAttributes() const override;

    private:

        void setUniform(GLint location, const GLint& v0);
        void setUniform(GLint location, const vec3& v0);
        void setUniform(GLint location, const mat4x4& m0);

        void setupActiveUniform();
        void setupActiveAttribute();

        Limits _limits;

        GLuint _programID;

        GraphicsInputLayoutPtr _inputLayout;

        // TODO: tempolar; move to context binding list
        std::map<std::string, GLUniform> _activeUniforms;
        std::map<std::string, GLVertexAttribute> _activeAttributes;
        
        GraphicsVertexAttributes _activeAttributeList;

        GraphicsProgramDesc _programDesc;
    };

} // namespace el {
