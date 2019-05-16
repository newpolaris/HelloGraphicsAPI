#ifndef __METAL_TYPES_H__
#define __METAL_TYPES_H__
 
#include <graphics_types.h>
#include <math_types.h>
#include <mtlpp.hpp>

namespace el
{

    typedef void *NativeSurface;
    typedef void *NativeContext;
    
    typedef std::shared_ptr<struct MetalSurface> MetalSurfacePtr;
    
    struct MetalContext;
    
    struct MetalRenderPipelineDesc
    {
        ns::String label;
        mtlpp::Function vertexFunction;
        mtlpp::Function fragmentFunction;
    };
    
    class MetalRenderTarget
    {
    public:
        
        explicit MetalRenderTarget(MetalContext* context);
        MetalRenderTarget(MetalContext* context, mtlpp::Texture color, mtlpp::Texture depth);
        
        bool isDefaultRenderTarget() const;
        mtlpp::Texture getColor() const;
        mtlpp::Texture getDepth() const;
        uint32_t getLevel() const;
        
        MetalContext* _context;
        bool _isDefaultRenderTarget;
        mtlpp::Texture _color;
        mtlpp::Texture _depth;
        uint32_t _level;
    };
    
    struct MetalSurface
    {
        MetalSurface(MetalContext* context, NativeSurface layer);
        
        NativeSurface layer;
    };
    
    struct PassDescriptor
    {
        PassDescriptor()
        {
            clearColor = math::float4(0.f, 0.f, 0.f, 0.f);
            clearDepth = 0.0;
            loadColor = mtlpp::LoadAction::Clear;
            storeColor = mtlpp::StoreAction::Store;
            loadDepth = mtlpp::LoadAction::Clear;
            storeDepth = mtlpp::StoreAction::DontCare;
        }
        
        PassDescriptor(math::float4 color, float depth)
        {
            clearColor = color;
            clearDepth = depth;
            loadColor = mtlpp::LoadAction::Clear;
            storeColor = mtlpp::StoreAction::Store;
            loadDepth = mtlpp::LoadAction::Clear;
            storeDepth = mtlpp::StoreAction::DontCare;
        }
        
        math::float4 clearColor;
        float clearDepth;
        mtlpp::LoadAction loadColor;
        mtlpp::StoreAction storeColor;
        mtlpp::LoadAction loadDepth;
        mtlpp::StoreAction storeDepth;
    };

} // namespac el

#endif // __METAL_TYPES_H__
