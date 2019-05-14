#ifndef __MTL_CONTEXT_H__
#define __MTL_CONTEXT_H__

#include <el_predefine.h>

#if EL_BUILD_METAL

#include <map>
#include <mtlpp.hpp>
#include <graphics_context.h>
#include <graphics_depth_stencil.h>
#include <math_types.h>

#include "mtl_types.h"
#include "mtl_device.h"
#include "mtl_depth_stencil.h"

namespace el {

    struct DepthStencilCache final
    {
        typedef std::map<GraphicsDepthStencilDesc, mtlpp::DepthStencilState> HashMap;

        void setDevice(const MTLDevicePtr& device);

        const mtlpp::DepthStencilState& request(const GraphicsDepthStencilDesc&);

        MTLDevicePtr _device;
        HashMap _cache;
    };

    inline 
    void DepthStencilCache::setDevice(const MTLDevicePtr& device)
    {
        _device = device;
    }

    inline
    const mtlpp::DepthStencilState& DepthStencilCache::request(const GraphicsDepthStencilDesc& desc)
    {
        auto it = _cache.find(desc);
        if (it != _cache.end())
            return it->second;

        auto depthStencil = _device->createDepthStencil(desc);
        EL_ASSERT(depthStencil);

        auto metalDepthStencil = std::static_pointer_cast<MTLDepthStencil>(depthStencil);
        return metalDepthStencil->getMetalDepthStencilState();
    }

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
        
        void setDepthTestEnable(bool enable) override;
        void setCullFaceEnable(bool enable) override;
        
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

    typedef void* SwapchainHandle;

    class MetalContext final
    {
    public:

        MetalContext();
        ~MetalContext();

        void setDevice(const GraphicsDevicePtr& device);

        void beginFrame(SwapchainHandle handle);
        void endFrame();
        void beginPass();
        void endPass();
        void present();
        void commit(bool isWaitComplete = false);
        
        void setDepthTestEnable(bool enable);
        void setDepthWriteEnable(bool enable);
        void setDepthCompareOp(GraphicsCompareOp compare);

        void draw(GraphicsPrimitiveType primitive, uint32_t vertexCount, int32_t vertexStartOffset = 0);

        mtlpp::Drawable& getCurrentDrawable();
        mtlpp::CommandBuffer& getCurrentCommandBuffer();
        mtlpp::RenderCommandEncoder& getCurrentCommandEncoder();

    private:

        MetalContext(const MetalContext&);
        MetalContext& operator=(const MetalContext&);

        GraphicsPixelFormat _depthFormat;

        MTLDevicePtr _device;
        mtlpp::Device _metalDevice;
        mtlpp::CommandBuffer _currentCommandBuffer;
        mtlpp::RenderCommandEncoder _currentCommandEncoder;
        mtlpp::Drawable _currentDrawable;
        SwapchainHandle _swapchainHandle;
        
        math::float4 _clearColor;
        float _clearDepth;

        GraphicsDepthStencilDesc _depthStencilDesc;
        DepthStencilCache _depthStencilCache;
    };

} // namespace el {

#endif // EL_BUILD_METAL

#endif // __MTL_CONTEXT_H__
