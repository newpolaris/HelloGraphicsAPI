#include <SDL.h>
#include <SDL_syswm.h>
#include <Cocoa/Cocoa.h>
#include <mtlpp.hpp>
#include <el_debug.h>

#include <Metal/mtl_types.h>
#include <Metal/mtl_texture.h>
#include <Metal/mtl_shader.h>
#include <Metal/mtl_device.h>
#include <Metal/mtl_context.h>
#include <Metal/Metal.h>
#include <QuartzCore/CAMetalLayer.h>

const char shadersSrc[] = R"""(
#include <metal_stdlib>
using namespace metal;

vertex float4 vertFunc(
                       const device packed_float3* vertexArray [[buffer(0)]],
                       unsigned int vID[[vertex_id]])
{
    return float4(vertexArray[vID], 1.0);
}

fragment half4 fragFunc()
{
    return half4(1.0, 0.0, 0.0, 1.0);
}
)""";

const float vertexData[] =
{
    0.0f,  1.0f, 0.0f,
    -1.0f, -1.0f, 0.0f,
    1.0f, -1.0f, 0.0f,
};

int main()
{
    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window* window = SDL_CreateWindow("sdl sample",
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



    id<MTLDevice> device = MTLCreateSystemDefaultDevice();

    CAMetalLayer* layer = [CAMetalLayer layer];

    id<MTLCommandQueue> commandQueue = [device newCommandQueue];

    NSError* error = nil;
    NSString* objcSource = [NSString stringWithCString:shadersSrc
        encoding:NSUTF8StringEncoding];
    id<MTLLibrary> library = [device newLibraryWithSource:objcSource
        options:nil
        error:&error];
#if !__has_feature(objc_arc)
    [objcSource release];
#endif

    id<MTLFunction> vertexFunction = [library newFunctionWithName:@"vertFunc"];
    id<MTLFunction> fragmentFunction = [library newFunctionWithName:@"fragFunc"];
    [library release];

    while (true)
    {
        bool isQuit = false;
        SDL_Event e;
        while (SDL_PollEvent(&e))
            isQuit = (e.type == SDL_QUIT);
        if (isQuit) break;

#if !__has_feature(objc_arc)
        NSAutoreleasePool *framePool = [[NSAutoreleasePool alloc] init];
#endif

        MTLRenderPipelineDescriptor* descriptor = [MTLRenderPipelineDescriptor new];
        descriptor.colorAttachments[0].pixelFormat = MTLPixelFormatBGRA8Unorm;
        descriptor.depthAttachmentPixelFormat = MTLPixelFormatInvalid;
        descriptor.vertexFunction = vertexFunction;
        descriptor.fragmentFunction = fragmentFunction;

        NSError* error = nullptr;
        id<MTLRenderPipelineState> pipeline = [device newRenderPipelineStateWithDescriptor:descriptor
            error:&error];
        if (error) {
            auto description = [error.localizedDescription cStringUsingEncoding:NSUTF8StringEncoding];
            EL_TRACE("%s", description);
        }

#if !__has_feature(objc_arc)
        [descriptor release];
#endif

        id<CAMetalDrawable> drawable = [layer nextDrawable];
        id<MTLCommandBuffer> commandBuffer = [commandQueue commandBuffer];

        MTLRenderPassDescriptor* renderPassDesc = [MTLRenderPassDescriptor renderPassDescriptor];
        MTLRenderPassColorAttachmentDescriptor* colorAttachment = renderPassDesc.colorAttachments[0];
        [colorAttachment setClearColor:MTLClearColorMake(0.2, 0.4, 0.6, 1.0)];
        [colorAttachment setLoadAction:MTLLoadActionClear];
        [colorAttachment setStoreAction:MTLStoreActionStore];
        [colorAttachment setTexture:drawable.texture];

        id<MTLRenderCommandEncoder> renderEncoder = [commandBuffer renderCommandEncoderWithDescriptor:renderPassDesc];
        [renderEncoder setRenderPipelineState:pipeline];
        [renderEncoder setVertexBytes:vertexData length:sizeof(vertexData) atIndex:0];
        [renderEncoder drawPrimitives:MTLPrimitiveTypeTriangle
            vertexStart:0
                vertexCount:3];
        [renderEncoder endEncoding];
        [commandBuffer presentDrawable:drawable];
        [commandBuffer commit];

#if !__has_feature(objc_arc)
        [pipeline release];
        [framePool drain];
#endif
    }

    id<MTLCommandBuffer> oneOffBuffer = [commandQueue commandBuffer];
    [oneOffBuffer commit];
    [oneOffBuffer waitUntilCompleted];

    [commandQueue release];
    [device release];

    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
