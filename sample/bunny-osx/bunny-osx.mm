#include <SDL.h>
#include <SDL_syswm.h>
#include <Cocoa/Cocoa.h>
#include <QuartzCore/CAMetalLayer.h>
#include <Metal/Metal.h>
#include <mtlpp.hpp>
#include <el_debug.h>

#undef EL_BUILD_METAL
#define EL_BUILD_METAL 1

#include <Metal/mtl_types.h>
#include <Metal/mtl_texture.h>
#include <Metal/mtl_shader.h>
#include <Metal/mtl_depth_stencil.h>
#include <Metal/mtl_device.h>
#include <Metal/mtl_context.h>

bool execute()
{
    while (true)
    {
        bool isQuit = false;
        SDL_Event e;
        while (SDL_PollEvent(&e))
            isQuit = (e.type == SDL_QUIT);
        if (isQuit) break;
    }
    return true;
}

int main()
{
    SDL_Init(SDL_INIT_VIDEO);
    
    SDL_Window* window = SDL_CreateWindow("dpeth sample",
                                          SDL_WINDOWPOS_UNDEFINED,
                                          SDL_WINDOWPOS_UNDEFINED,
                                          1280, 768,
                                          SDL_WINDOW_RESIZABLE);
    
    EL_ASSERT(window != nullptr);

    SDL_SysWMinfo info;
    SDL_GetVersion(&info.version);
    SDL_GetWindowWMInfo(window, &info);
    
    NSView* view = [info.info.cocoa.window contentView];

    const auto colorFormat = el::GraphicsPixelFormatBGRA8Unorm;
    const auto depthFormat = el::GraphicsPixelFormatDepth32Float;
    
    auto device = mtlpp::Device::CreateSystemDefaultDevice();

    CAMetalLayer* layer = [CAMetalLayer layer];
    layer.device = (__bridge id<MTLDevice>)device.GetPtr();
    layer.opaque = true;
    layer.drawableSize = [view convertSizeToBacking:view.bounds.size];
    layer.pixelFormat = (MTLPixelFormat)el::asPixelFormat(colorFormat);
    
    view.wantsLayer = YES;
    view.layer = layer;

    while (true)
    {
        bool isQuit = false;
        SDL_Event e;
        while (SDL_PollEvent(&e))
            isQuit = (e.type == SDL_QUIT);
        if (isQuit) break;

        auto drawable = [layer nextDrawable];
        auto commandQueue = device.NewCommandQueue();
        auto commandBuffer = commandQueue.CommandBuffer();

        mtlpp::Texture color(ns::Handle{(__bridge void*)drawable.texture});

        mtlpp::RenderPassDescriptor renderPassDesc;
        auto colorAttachment = renderPassDesc.GetColorAttachments()[0];
        colorAttachment.SetClearColor(mtlpp::ClearColor(0.5f, 0.5f, 0.5f, 1.0f));
        colorAttachment.SetLoadAction(mtlpp::LoadAction::Clear);
        colorAttachment.SetStoreAction(mtlpp::StoreAction::Store);
        colorAttachment.SetTexture(color);
        colorAttachment.SetLevel(0);
        colorAttachment.SetSlice(0);

        auto renderEncoder = commandBuffer.RenderCommandEncoder(renderPassDesc);
        renderEncoder.EndEncoding();

        commandBuffer.Present(ns::Handle{(__bridge void*)drawable});
        commandBuffer.Commit();
        commandBuffer.WaitUntilCompleted();
    }

    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}