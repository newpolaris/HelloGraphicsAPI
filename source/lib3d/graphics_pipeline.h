#ifndef __GRAPHICS_PIPELINE_H__
#define __GRAPHICS_PIPELINE_H__
    
#include <graphics_types.h>

namespace el {
    
    class GraphicsPipelineDesc
    {
    public:

        GraphicsInputLayoutPtr _inputLayout;
        GraphicsShaders _shaders;
    };

    class GraphicsPipeline
    {
    public:

        GraphicsPipeline();
        virtual ~GraphicsPipeline();
        
        virtual const GraphicsPipelineDesc& getPipelineDesc() const = 0;
    };

} // namespace el {

#endif // __GRAPHICS_PIPELINE_H__
