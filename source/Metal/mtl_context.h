#pragma once

#include "predefine.h"

#if EL_PLAT_APPLE

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

        void finsh(bool waitForCompletion = false);
        
        void beginRendering();
        void endRendering();
        void setViewport(const Viewport& viewport);
        void setProgram(const GraphicsProgramPtr& program);
        void setTexture(const std::string& name, const GraphicsTexturePtr& texture);
        void setVertexBuffer(const std::string& name, const GraphicsDataPtr& vertexData, uint32_t stride, uint32_t offset);
        void setVertexBuffer(uint32_t binding, const GraphicsDataPtr& vertexData, uint32_t offset = 0);
        void setIndexBuffer(const GraphicsDataPtr& indexData);
        void setUniform(const std::string& name, float v0);
        void setUniform(const std::string& name, const vec3& v0);
        void setUniform(const std::string& name, const quat& q0);
        void setUniform(const std::string& name, const mat4x4& m0);
        void setInputLayout(const GraphicsInputLayoutPtr& inputLayout);
        
        void draw(GraphicsPrimitiveType primitive, uint32_t vertexCount, int32_t vertexStartOffset = 0);
        void drawIndexed(GraphicsPrimitiveType primitive, uint32_t indexCount, uint32_t startIndexLocation = 0);
        void drawIndexed(GraphicsPrimitiveType primitive, uint32_t indexCount, uint32_t startIndexLocation, int32_t baseVertexLocation);
        void drawInstanced(GraphicsPrimitiveType primitive, uint32_t vertexCountPerInstance, uint32_t instanceCount,
                uint32_t startVertexLocation = 0, uint32_t startInstanceLocation = 0);
        void drawIndexedInstanced(GraphicsPrimitiveType primitive, uint32_t indexCountPerInstance, uint32_t instanceCount,
                uint32_t startIndexLocation, int32_t vaseVertexLocation, uint32_t startInstanceLocation);

    private:

        mtlpp::CommandBuffer _commandBuffer;

        MetalDeviceWeakPtr _device;
    };

} // namespace el {

#endif // EL_PLAT_APPLE
