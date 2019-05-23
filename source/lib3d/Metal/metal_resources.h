#ifndef __METAL_RESOURCES_H__
#define __METAL_RESOURCES_H__

#include "metal_types.h"
#include <graphics_data.h>
#include <graphics_texture.h>

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

    struct MetalBuffer final : public GraphicsData
    {
        MetalBuffer();
        ~MetalBuffer();
        
        bool create(id<MTLDevice> device, const GraphicsDataDesc &desc);
        void destroy();

        const GraphicsDataDesc &getDesc() const override;

        GraphicsDataDesc desc;
        id<MTLBuffer> buffer;
    };
    
    struct MetalUniformBuffer final : public GraphicsData
    {
        MetalUniformBuffer();
        ~MetalUniformBuffer();
        
        bool create(id<MTLDevice> device, const GraphicsDataDesc &desc);
        void destroy();
        
        void update(const stream_t* stream);
        void update(const stream_t* stream, size_t offset, size_t length);
        
        const GraphicsDataDesc &getDesc() const override;
        
        GraphicsDataDesc desc;
        id<MTLBuffer> buffer;
    };

    struct MetalTexture final : public GraphicsTexture
    {
        MetalTexture();
        ~MetalTexture();
        
        bool create(id<MTLDevice> device, const GraphicsTextureDesc &desc);
        void destroy();
        
        const GraphicsTextureDesc &getDesc() const override;
        
        GraphicsTextureDesc desc;
        id<MTLTexture> texture;
    };
    
} // namespace el

#endif // __METAL_RESOURCES_H__
