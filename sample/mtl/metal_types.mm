#include "metal_types.h"

_EL_NAME_BEGIN

MTLClearColor asMTLClearColor(const math::float4 &color)
{
    return MTLClearColorMake(color.r, color.g, color.b, color.a);
}

MTLLoadAction asLoadAction(const RenderPassParms& params, GraphicsTargetBufferFlagBits bit)
{
    if (params.flags.clear & bit)
        return MTLLoadActionClear;
    if (params.flags.loadDiscard & bit)
        return MTLLoadActionDontCare;
    return MTLLoadActionLoad;
}

MTLStoreAction asStoreAction(const RenderPassParms& params, GraphicsTargetBufferFlagBits bit)
{
    if (params.flags.storeDiscard & bit)
        return MTLStoreActionDontCare;
    return MTLStoreActionStore;
}

_EL_NAME_END