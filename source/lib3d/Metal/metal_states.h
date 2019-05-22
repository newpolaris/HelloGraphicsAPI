#ifndef __METAL_STATES_H__
#define __METAL_STATES_H__

#include <tuple>
#include "metal_types.h"

namespace el {

    struct MetalPipelineDesc
    {
        id<MTLFunction> vertexFunction;
        id<MTLFunction> fragmentFunction;
        MTLVertexDescriptor *vertexDescriptor;
        MetalPixelFormats colorFormats;
        MetalPixelFormat depthFormat;

        bool operator<(const MetalPipelineDesc &desc) const
        {
            const auto &a = std::tie(vertexFunction, fragmentFunction, colorFormats, depthFormat);
            const auto &b = std::tie(desc.vertexFunction, desc.fragmentFunction, desc.colorFormats, desc.depthFormat);
            return a < b;
        }
    };

    id<MTLRenderPipelineState> aquirePipeline(MetalContext* context, const MetalPipelineDesc &desc);
    void cleanupPipeline(MetalContext* context);

} // namespace el {

#endif // __METAL_STATES_H__
