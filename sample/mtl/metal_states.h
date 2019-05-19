#ifndef __METAL_STATES_H__
#define __METAL_STATES_H__

#include "metal_types.h"
#include <tuple>

namespace el {

    struct PipelineDesc
    {
        id<MTLFunction> vertexFunction;
        id<MTLFunction> fragmentFunction;

        bool operator<(const PipelineDesc& desc) const
        {
            const auto& a = std::tie(vertexFunction, fragmentFunction);
            const auto& b = std::tie(desc.vertexFunction, desc.fragmentFunction);
            return a < b;
        }
    };

    id<MTLRenderPipelineState> aquirePipeline(id<MTLDevice> device, const PipelineDesc& desc);
    void cleanupPipeline();

} // namespace el {

#endif // __METAL_STATES_H__
