#pragma once

#include "predefine.h"
#include <graphics_types.h>
#include <linmath.h>

namespace el {

    class GraphicsContext
    {
    public:

        GraphicsContext();
        virtual ~GraphicsContext();

        virtual void beginRendering() = 0;
        virtual void endRendering() = 0;
        virtual void setViewport(const Viewport& viewport) = 0;
        virtual void setProgram(const GraphicsProgramPtr& program) = 0;
        virtual void setTexture(const std::string& name, const GraphicsTexturePtr& texture) = 0;
        [[deprecate]]
        virtual void setVertexBuffer(const std::string& name, const GraphicsDataPtr& vertex_buffer, uint32_t stride, uint32_t offset) = 0;
        virtual void setVertexBuffer(uint32_t binding, const GraphicsDataPtr& vertex_buffer, uint32_t offset = 0);
        virtual void setIndexBuffer(const GraphicsDataPtr& index_buffer) = 0;
        virtual void setUniform(const std::string& name, const vec3& v0) = 0;
        virtual void setUniform(const std::string& name, const mat4x4& m0) = 0;
        virtual void setInputLayout(const GraphicsInputLayoutPtr& input_layout) = 0;

        virtual void draw(GraphicsPrimitiveType primitive, uint32_t vertexCount, int32_t vertexStartOffset = 0) = 0;
        virtual void drawIndexed(GraphicsPrimitiveType primitive, uint32_t indexCount, uint32_t startIndexLocation = 0) = 0;
        virtual void drawIndexed(GraphicsPrimitiveType primitive, uint32_t indexCount, uint32_t startIndexLocation, int32_t baseVertexLocation) = 0;
        virtual void drawInstanced(GraphicsPrimitiveType primitive, uint32_t vertexCountPerInstance, uint32_t instanceCount,
		                           uint32_t startVertexLocation = 0, uint32_t startInstanceLocation = 0) = 0;
        virtual void drawIndexedInstanced(GraphicsPrimitiveType primitive, uint32_t indexCountPerInstance, uint32_t instanceCount, 
                                          uint32_t startIndexLocation, int32_t vaseVertexLocation, uint32_t startInstanceLocation) = 0;

    private:

        GraphicsContext(const GraphicsContext&) = delete;
        GraphicsContext& operator=(const GraphicsContext&) = delete;
    };

} // namespace el {
