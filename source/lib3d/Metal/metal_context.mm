#include "metal_context.h"

_EL_NAME_BEGIN

id<CAMetalDrawable> aquireDrawable(MetalContext* context)
{
    EL_ASSERT(context);
    EL_ASSERT(context->layer);

    if (context->currentDrawable == nil)
        context->currentDrawable = [context->layer nextDrawable];
    return context->currentDrawable;
}

_EL_NAME_END