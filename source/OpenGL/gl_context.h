#pragma once

#include "predefine.h"

#if EL_BUILD_OPENGL

#include <graphics_context.h>
#include "gl_types.h"

namespace el {

    class GLContext final: public GraphicsContext
    {
    public:

        GLContext();
        ~GLContext();

        bool create();
        void destory();

        void beginRendering() override;
        void endRendering() override;

        void setViewport(const Viewport& viewport) override;
        void setProgram(const GraphicsProgramPtr& ptr) override;
        void setTexture(const std::string& name, const GraphicsTexturePtr& texture) override;
        void setVertexBuffer(const std::string& name, const GraphicsBufferPtr& vertex_buffer, uint32_t stride, uint32_t offset) override;
        void setUniform(const std::string& name, const vec3& v0) override;
        void setUniform(const std::string& name, const mat4x4& m0) override;

        void draw(GraphicsPrimitiveType primitive, int32_t first, uint32_t count) override;

    private:

        Viewport _viewport;
        GLProgramPtr _program;
    };

} // namespace el {

#endif // #if EL_BUILD_OPENGL
