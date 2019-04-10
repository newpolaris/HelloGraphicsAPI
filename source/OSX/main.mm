#define METAL_MTLPP_EXAMPLE 0

#include <debug.h>
#include <graphics_types.h>
#include <graphics_device.h>
#include <graphics_texture.h>

#include "mtlpp.hpp"

#if METAL_MTLPP_EXAMPLE

namespace el {
    void _00_init()
    {
        GraphicsDevicePtr device;
        
        GraphicsDeviceDesc deviceDesc;
        deviceDesc.setType(GraphicsDeviceTypeMetal);
        
        device = createDevice(deviceDesc);
        EL_ASSERT(device != nullptr);
    }
    
    void _01_clear()
    {
        const uint32_t width = 4;
        const uint32_t height = 4;
        
        GraphicsDevicePtr device;
        GraphicsDeviceContextPtr context;
        GraphicsTexturePtr texture;
        
        GraphicsDeviceDesc deviceDesc;
        deviceDesc.setType(GraphicsDeviceTypeMetal);
        
        device = createDevice(deviceDesc);
        EL_ASSERT(device != nullptr);

        GraphicsTextureDesc textureDesc;
        textureDesc.setWidth(width);
        textureDesc.setHeight(height);
        textureDesc.setDim(GraphicsTextureDim2D);

        texture = device->createTexture(textureDesc);
        EL_ASSERT(texture != nullptr);

        context = device->createDeviceContext();
        EL_ASSERT(context != nullptr);

        uint32_t data[width * height];
        
    #if 0
        GraphicsFramebufferPtr colorRenderTarget;
        GraphicsFramebufferDesc frameDesc;

        colorRenderTarget = device->createFramebuffer(frameDesc);

        context->setFramebuffer(colorRenderTarget);
        
        // ??
        // glBindFramebuffer(GL_FRAME_BUFFER, some_magic_int);

        // glBindFramebuffer(GL_FRAME_BUFFER, 0);
        // on macbook framebuffer related width / height can't be deduced;

        // id <CAMetalDrawable> drawable = _currentDrawable = [_metalLayer nextDrawable];
        // _renderPassDescriptor = [MTLRenderPassDescriptor renderPassDescriptor];
    
        // create a color attachment every frame since we have to recreate the texture every frame
        // MTLRenderPassColorAttachmentDescriptor *colorAttachment = _renderPassDescriptor.colorAttachments[0];
        // colorAttachment.texture = texture;

        // make sure to clear every frame for best performance
        // colorAttachment.loadAction = MTLLoadActionClear;
        // colorAttachment.clearColor = MTLClearColorMake(0.65f, 0.65f, 0.65f, 1.0f);
    
        // context->setFramebuffer(colorRenderTarget);

        context->begin();
        {
            context->beginRender(_renderPassDesc);
            context->setDepthStencil(_depthState);
            context->pushDebugGroup("DrawCube");
            context->setRenderPipelineState(_pipelineState);
            context->setVertexBuffer(0, _boxMesh.vertexBuffers[0].buffer, _boxMesh.vertexBuffers[0].offset);
            context->setVertexBuffer(1, _dynamicConstantBuffer, sizeof(uniforms_t) * _constantDataBufferIndex);

            MTKSubmesh* submesh = _boxMesh.submeshes[0];

            // Tell the render context we want to draw our primitives
            context->drawIndexed(
                submesh.primitiveType, 
                indexCount:submesh.indexCount,
                indexType:submesh.indexType,
                indexBuffer:submesh.indexBuffer.buffer,
                indexBufferOffset:submesh.indexBuffer.offset);

            context->popDebugGroup();
            context->endRender();
            // context->present(_view.currentDrawable);

            auto rawContext = static_pointer_cast<MTLDeviceContext>(context);
            auto commandBuffer = rawContext->getCommandBuffer();

            mtlpp::BlitCommandEncoder blitCommandEncoder = commandBuffer.BlitCommandEncoder();
            blitCommandEncoder.Synchronize(texture, 0, 0);
            blitCommandEncoder.EndEncoding();
        }
        context->finish(true);

        texture.GetBytes(data, width * 4, mtlpp::Region(0, 0, width, height), 0);
    #endif
        
        for (uint32_t i=0; i<width*height; i++)
        {
            assert(data[i] == 0x000000FF);
        }

    }
    
} // namespace el {

int main()
{
    el::_00_init();
    el::_01_clear();

    return 0;
}

#elif 0

#import <AppKit/NSApplication.h>

int main(int argc, const char *argv[])
{
    return NSApplicationMain(argc, argv);
}

#endif
