#ifndef __RENDERER_H__
#define __RENDERER_H__

#include <mtlpp.hpp>
#include <graphics_types.h>

typedef void* SwapchainHandle;

class Renderer
{
public:
    
    Renderer();
    
    mtlpp::Device& getDevice();
    mtlpp::CommandQueue& getCommandQueue();
    
    void setup();
    void setupBlitRenderer();
    void setupMeshRenderer();
    
    void makeCurrent(SwapchainHandle handle);
    void draw();
    
    mtlpp::Device _device;
    mtlpp::CommandQueue _commandQueue;
    mtlpp::Function _blitVertexFunction;
    mtlpp::Function _blitFragmentFunction;
    mtlpp::RenderPipelineState _blitPipelineState;
    mtlpp::Texture _color;
    mtlpp::Texture _depth;
    
    el::GraphicsPixelFormat _surfaceFormat;
    SwapchainHandle _handle;
    const el::GraphicsPixelFormat _colorFormat = el::GraphicsPixelFormatRG11B10Float;
    const el::GraphicsPixelFormat _depthFormat = el::GraphicsPixelFormatDepth32Float;
};

#endif // __RENDERER_H__