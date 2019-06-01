#ifndef __GRAPHICS_CONTEXT_H__
#define __GRAPHICS_CONTEXT_H__

#include <graphics_types.h>
#include <linmath.h>
#include <el_debug.h>

namespace el {

    class GraphicsContext
    {
    public:

        GraphicsContext();
        virtual ~GraphicsContext();

        virtual void beginRendering() = 0;
        virtual void endRendering() = 0;
        virtual void setDepthTestEnable(bool enable) = 0;
        virtual void setCullFaceEnable(bool enable) = 0;
        virtual void setViewport(const Viewport& viewport) = 0;
        virtual void setProgram(const GraphicsProgramPtr& program) = 0;
        virtual void setTexture(const std::string& name, const GraphicsTexturePtr& texture) = 0;
        virtual void setVertexBuffer(const std::string& name, const GraphicsDataPtr& vertexData, uint32_t stride, uint32_t offset) = 0;
        virtual void setVertexBuffer(uint32_t binding, const GraphicsDataPtr& vertexData, uint32_t offset = 0) = 0;
        virtual void setIndexBuffer(const GraphicsDataPtr& indexData) = 0;
        virtual void setUniform(const std::string& name, float v0) = 0;
        virtual void setUniform(const std::string& name, const vec3& v0) = 0;
        virtual void setUniform(const std::string& name, const quat& q0) = 0;
        virtual void setUniform(const std::string& name, const mat4x4& m0) = 0;
        virtual void setInputLayout(const GraphicsInputLayoutPtr& inputLayout) = 0;

        virtual void draw(GraphicsPrimitiveType primitive, uint32_t vertexCount, int32_t vertexStartOffset = 0) = 0;
        virtual void drawIndexed(GraphicsPrimitiveType primitive, uint32_t indexCount, uint32_t startIndexLocation = 0) = 0;
        virtual void drawIndexed(GraphicsPrimitiveType primitive, uint32_t indexCount, uint32_t startIndexLocation, int32_t baseVertexLocation) = 0;
        virtual void drawInstanced(GraphicsPrimitiveType primitive, uint32_t vertexCountPerInstance, uint32_t instanceCount,
		                           uint32_t startVertexLocation = 0, uint32_t startInstanceLocation = 0) = 0;
        virtual void drawIndexedInstanced(GraphicsPrimitiveType primitive, uint32_t indexCountPerInstance, uint32_t instanceCount, 
                                          uint32_t startIndexLocation, int32_t vaseVertexLocation, uint32_t startInstanceLocation) = 0;

    private:

        GraphicsContext(const GraphicsContext&);
        GraphicsContext& operator=(const GraphicsContext&);
    };

} // namespace el {

#endif // __GRAPHICS_CONTEXT_H__
