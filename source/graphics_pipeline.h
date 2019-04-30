#pragma once
    
#include "graphics_types.h"

namespace el {
    
    class GraphicsPipelineDesc
    {
    public:

    };

    class GraphicsPipeline
    {
    public:

        GraphicsPipeline();
        virtual ~GraphicsPipeline();
        
        virtual const GraphicsPipelineDesc& getDesc() const = 0;
    };

} // namespace el {
