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
        [[deprecate]]
        void setVertexBuffer(const std::string& name, const GraphicsDataPtr& vertex_buffer, uint32_t stride, uint32_t offset) override;
        void setVertexBuffer(uint32_t binding, const GraphicsDataPtr& vertex_buffer, uint32_t offset = 0);
        void setIndexBuffer(const GraphicsDataPtr& index_buffer) override;
        void setUniform(const std::string& name, const vec3& v0) override;
        void setUniform(const std::string& name, const mat4x4& m0) override;

        void draw(GraphicsPrimitiveType primitive, uint32_t vertexCount, int32_t vertexStartOffset) override;
        void drawIndexed(GraphicsPrimitiveType primitive, uint32_t indexCount, uint32_t startIndexLocation) override;
        void drawIndexed(GraphicsPrimitiveType primitive, uint32_t indexCount, uint32_t startIndexLocation, int32_t baseVertexLocation) override;
        void drawInstanced(GraphicsPrimitiveType primitive, uint32_t vertexCountPerInstance, uint32_t instanceCount,
		                   uint32_t startVertexLocation, uint32_t startInstanceLocation) override;
        void drawIndexedInstanced(GraphicsPrimitiveType primitive, uint32_t indexCountPerInstance, uint32_t instanceCount,
                                  uint32_t startIndexLocation, int32_t vaseVertexLocation, uint32_t startInstanceLocation) override;

    private:

        size_t _indexSize;
        size_t _numIndices;
        size_t _vertexSize;
        size_t _numVertices;
        Viewport _viewport;
        GLProgramPtr _program;
    };

} // namespace el {

#endif // #if EL_BUILD_OPENGL
