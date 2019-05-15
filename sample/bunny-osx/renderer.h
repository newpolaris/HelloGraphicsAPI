#ifndef __RENDERER_H__
#define __RENDERER_H__

#include <mtlpp.hpp>
#include <graphics_types.h>

typedef void* SwapchainHandle;

namespace el
{
    class Renderer
    {
    public:
        Renderer();

        mtlpp::Device &getDevice();
        mtlpp::CommandQueue &getCommandQueue();

        void setup();
        void setupBlitRenderer();
        void setupMeshRenderer();

        void makeCurrent(SwapchainHandle handle);
        void draw();

        std::shared_ptr<class MetalDriver> _driver;

        mtlpp::Device _device;
        mtlpp::Function _blitVertexFunction;
        mtlpp::Function _blitFragmentFunction;
        mtlpp::RenderPipelineState _blitPipelineState;
        mtlpp::Texture _color;
        mtlpp::Texture _depth;

        GraphicsPixelFormat _surfaceFormat;
        SwapchainHandle _handle;
        const GraphicsPixelFormat _colorFormat = GraphicsPixelFormatRG11B10Float;
        const GraphicsPixelFormat _depthFormat = GraphicsPixelFormatDepth32Float;
    };
} // namespace el

#endif // __RENDERER_H__
