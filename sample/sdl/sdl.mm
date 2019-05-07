#include <el_debug.h>
#include <el_platform.h>
#include <SDL.h>
#include <SDL_syswm.h>
#include <Cocoa/Cocoa.h>
#include <Metal/Metal.h>
#include <QuartzCore/CAMetalLayer.h>

@interface MetalView : NSView
@property (assign, nonatomic) CAMetalLayer* metalLayer;
@end

@implementation MetalView

+ (Class)layerClass
{
    return [CAMetalLayer class];
}

- (instancetype)initWithFrame:(NSRect)frameRect
{
    if ((self = [super initWithFrame:frameRect]))
    {
#if EL_PLAT_IOS
        self.autoresizingMask = UIViewAutoresizingFlexibleWidth |
                                UIViewAutoresizingFlexibleHeight;
        
        self.contentScaleFactor = [UIScreen mainScreen].nativeScale;
        self.backgroundColor = nil;
        
        _metalLayer = (CAMetalLayer*)self.layer;
#else
        self.wantsLayer = YES;
        self.layer = _metalLayer = [CAMetalLayer layer];
#endif

        [self updateDrawableSize];
    }
    return self;
}

- (void)updateDrawableSize
{
    CGSize size = self.bounds.size;
#if EL_PLAT_IOS
    size.width *= self.contentScaleFactor;
    size.height *= self.contentScaleFactor;
#else
    size.width *= self.window.screen.backingScaleFactor;
    size.height *= self.window.screen.backingScaleFactor;
#endif
    _metalLayer.drawableSize = size;
}

#if EL_PLAT_IOS
- (void)layoutSubviews
{
    [super layoutSubViews];
    [self updateDrawableSize];
}
#endif

@end


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

    MetalView* metalView = [[MetalView alloc] initWithFrame:view.frame];
    [view addSubview:metalView];
    
    id<MTLDevice> device = MTLCreateSystemDefaultDevice();
    
    CAMetalLayer* layer = [metalView metalLayer];
    layer.device = device;
    layer.opaque = true;
    
    id<MTLCommandQueue> queue = [device newCommandQueue];
    
    MTLRenderPassDescriptor *renderDesc = [MTLRenderPassDescriptor renderPassDescriptor];
    MTLRenderPassColorAttachmentDescriptor *colorAttachment = renderDesc.colorAttachments[0];
    colorAttachment.clearColor = MTLClearColorMake(0.5f, 0.5f, 0.5f, 1.0f);
    colorAttachment.loadAction = MTLLoadActionClear;
    colorAttachment.storeAction = MTLStoreActionStore;
    
    while (true)
    {
        bool isQuit = false;
        SDL_Event e;
        while (SDL_PollEvent(&e))
            isQuit = (e.type == SDL_QUIT);
        if (isQuit) break;
        
        MTLViewport viewport = (MTLViewport){
            0.0, 0.0,
            layer.drawableSize.width,
            layer.drawableSize.height,
            0.0, 1.0};
        
        id<CAMetalDrawable> drawable = [layer nextDrawable];
        colorAttachment.texture = drawable.texture;
        
        id<MTLCommandBuffer> commands = [queue commandBuffer];
        id<MTLRenderCommandEncoder> encoder = [commands renderCommandEncoderWithDescriptor:renderDesc];
        [encoder setViewport:viewport];
        [encoder endEncoding];
        [commands presentDrawable:drawable];
        [commands commit];
    }
    [metalView removeFromSuperview];
    SDL_DestroyWindow(window);
    SDL_Quit();
    
    return 0;
}
