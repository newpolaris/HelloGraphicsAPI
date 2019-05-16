#include "metal_types.h"
#include "metal_context.h"

_EL_NAME_BEGIN

MetalRenderTarget::MetalRenderTarget(MetalContext *context) :
    _context(context),
    _isDefaultRenderTarget(true),
    _level(0)
{
}

MetalRenderTarget::MetalRenderTarget(MetalContext *context, mtlpp::Texture color, mtlpp::Texture depth) :
    _context(context),
    _isDefaultRenderTarget(false),
    _color(color),
    _depth(depth),
    _level(0)
{
}

mtlpp::Texture MetalRenderTarget::getColor() const
{
    if (_isDefaultRenderTarget)
        return aquireSurfaceTexture(_context);
    return _color;
}

mtlpp::Texture MetalRenderTarget::getDepth() const
{
    return _depth;
}

uint32_t MetalRenderTarget::getLevel() const
{
    return _level;
}

bool MetalRenderTarget::isDefaultRenderTarget() const
{
    return _isDefaultRenderTarget;
}

MetalSurface::MetalSurface(MetalContext* context, NativeSurface layer) :
    layer(layer)
{
    setLayerDevice(context, layer);
}

_EL_NAME_END
