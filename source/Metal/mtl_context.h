#pragma once

#include "predefine.h"

#if EL_BUILD_METAL

#include "mtlpp.hpp"
#include "mtl_types.h"
#include "graphics_context.h"

namespace el {

    class MTLContext final : public GraphicsContext
    {
    public:

        MTLContext();
        ~MTLContext();

        bool create();
        void destroy();

        void setDevice(GraphicsDevicePtr device);
        GraphicsDevicePtr getDevice();

        void beginRendering() override;
        void endRendering() override;
        void finsh(bool waitForCompletion = false);
        
        void setDepthTest(bool isEnable) override;
        void setCullFace(bool isEnable) override;
        
        void setViewport(const Viewport& viewport) override;
        void setProgram(const GraphicsProgramPtr& program) override;
        void setTexture(const std::string& name, const GraphicsTexturePtr& texture) override;
        void setVertexBuffer(const std::string& name, const GraphicsDataPtr& vertexData, uint32_t stride, uint32_t offset) override;
        void setVertexBuffer(uint32_t binding, const GraphicsDataPtr& vertexData, uint32_t offset) override;
        void setIndexBuffer(const GraphicsDataPtr& indexData) override;
        void setUniform(const std::string& name, float v0) override;
        void setUniform(const std::string& name, const vec3& v0) override;
        void setUniform(const std::string& name, const quat& q0) override;
        void setUniform(const std::string& name, const mat4x4& m0) override;
        void setInputLayout(const GraphicsInputLayoutPtr& inputLayout) override;
        
        void draw(GraphicsPrimitiveType primitive, uint32_t vertexCount, int32_t vertexStartOffset) override;
        void drawIndexed(GraphicsPrimitiveType primitive, uint32_t indexCount, uint32_t startIndexLocation) override;
        void drawIndexed(GraphicsPrimitiveType primitive, uint32_t indexCount, uint32_t startIndexLocation, int32_t baseVertexLocation) override;
        void drawInstanced(GraphicsPrimitiveType primitive, uint32_t vertexCountPerInstance, uint32_t instanceCount,
                uint32_t startVertexLocation, uint32_t startInstanceLocation) override;
        void drawIndexedInstanced(GraphicsPrimitiveType primitive, uint32_t indexCountPerInstance, uint32_t instanceCount,
                uint32_t startIndexLocation, int32_t vaseVertexLocation, uint32_t startInstanceLocation) override;

    private:

        mtlpp::CommandBuffer _commandBuffer;

        MTLDeviceWeakPtr _device;
    };

} // namespace el {

#endif // EL_BUILD_METAL
