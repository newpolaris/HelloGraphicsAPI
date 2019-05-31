#ifndef __GRAPHICS_PIPELINE_H__
#define __GRAPHICS_PIPELINE_H__
    
#include <el/graphics_types.h>

// temp
#include <el/graphics_input_layout.h>

namespace el {
    
    typedef std::shared_ptr<struct MetalProgram> MetalProgramPtr;
    
    struct PipelineState
    {
        MetalProgramPtr program;
        GraphicsInputLayoutDesc inputLayout;
    };
    
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
