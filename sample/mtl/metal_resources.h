#ifndef __METAL_RESOURCES_H__
#define __METAL_RESOURCES_H__

#include "metal_types.h"

namespace el {

    struct MetalProgram final
    {
        MetalProgram();
        ~MetalProgram();
        
        bool create(id<MTLDevice> device, const char* vertex, const char* fragment);
        void destroy();

        id<MTLFunction> vertexFunction;
        id<MTLFunction> fragmentFunction;

    private:

        MetalProgram(const MetalProgram&);
        MetalProgram& operator=(const MetalProgram&);
    };

    struct MetalRenderTarget final
    {
        MetalRenderTarget(MetalContext& context);
        MetalRenderTarget(MetalContext& context, id<MTLTexture> color);
        MetalRenderTarget(MetalContext& context, id<MTLTexture> color, id<MTLTexture> depth);
        ~MetalRenderTarget();
        
        id<MTLTexture> getColor();
        id<MTLTexture> getDepth();
        
        bool isDefault;
        MetalContext& context;
        id<MTLTexture> color;
        id<MTLTexture> depth;
    
    private:

        MetalRenderTarget(const MetalRenderTarget&);
        MetalRenderTarget& operator=(const MetalRenderTarget&);
    };
    
} // namespace el {

#endif // __METAL_RESOURCES_H__
