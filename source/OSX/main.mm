#if 1

#include "mtlpp.hpp"

#include <debug.h>
#include <graphics_types.h>
#include <graphics_device.h>

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
        GraphicsTexturePtr texture;

        // GraphicsCommandQueuePtr commandQueue;
        
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
    }
    
} // namespace el {

int main()
{
    const uint32_t width  = 4;
    const uint32_t height = 4;

    mtlpp::Device device =  mtlpp::Device::CreateSystemDefaultDevice();
    assert(device);

    mtlpp::TextureDescriptor textureDesc = mtlpp::TextureDescriptor::Texture2DDescriptor(
        mtlpp::PixelFormat::RGBA8Unorm, width, height, false);
    textureDesc.SetUsage(mtlpp::TextureUsage::RenderTarget);
    mtlpp::Texture texture = device.NewTexture(textureDesc);
    assert(texture);

    mtlpp::CommandQueue commandQueue = device.NewCommandQueue();
    assert(commandQueue);
    mtlpp::CommandBuffer commandBuffer = commandQueue.CommandBuffer();
    assert(commandBuffer);

    mtlpp::RenderPassDescriptor renderPassDesc;
    mtlpp::RenderPassColorAttachmentDescriptor colorAttachmentDesc = renderPassDesc.GetColorAttachments()[0];
    colorAttachmentDesc.SetTexture(texture);
    colorAttachmentDesc.SetLoadAction(mtlpp::LoadAction::Clear);
    colorAttachmentDesc.SetStoreAction(mtlpp::StoreAction::Store);
    colorAttachmentDesc.SetClearColor(mtlpp::ClearColor(1.0, 0.0, 0.0, 0.0));
    renderPassDesc.SetRenderTargetArrayLength(1);

    mtlpp::RenderCommandEncoder renderCommandEncoder = commandBuffer.RenderCommandEncoder(renderPassDesc);
    assert(renderCommandEncoder);
    renderCommandEncoder.EndEncoding();

    mtlpp::BlitCommandEncoder blitCommandEncoder = commandBuffer.BlitCommandEncoder();
    blitCommandEncoder.Synchronize(texture, 0, 0);
    blitCommandEncoder.EndEncoding();

    commandBuffer.Commit();
    commandBuffer.WaitUntilCompleted();

    uint32_t data[width * height];
    texture.GetBytes(data, width * 4, mtlpp::Region(0, 0, width, height), 0);

    for (uint32_t i=0; i<width*height; i++)
    {
        assert(data[i] == 0x000000FF);
    }

    return 0;
}
#else

#import <AppKit/NSApplication.h>

int main(int argc, const char *argv[])
{
    return NSApplicationMain(argc, argv);
}
#endif
