#include "mtl_render_pipeline.h"

#if EL_BUILD_METAL

#include <el_debug.h>
#include "mtl_device.h"
#include "mtl_types.h"

using namespace el;
using namespace mtlpp;

MTLRenderPipeline::MTLRenderPipeline()
{
}

MTLRenderPipeline::~MTLRenderPipeline()
{
}

struct MetalRenderPipelineDesc
{
    GraphicsInputLayoutPtr inputLayout;
    GraphicsShaderPtr vertexShader;
    GraphicsShaderPtr fragmentShader;
    uint32_t sampleCount;
    GraphicsPixelFormat colorFormat; 
};

#if 0
void request()
{
    mtlpp::Device device;

    RenderPipelineDescriptor descriptor;
    // descriptor.SetVertexDescriptor(desc.getInputLayout());

    // void SetVertexFunction(desc.getVertexShader());
    // void SetFragmentFunction(const Function& fragmentFunction);
    // bool rasterizationEnabled = fragmentShader != nullptr;
    void SetRasterizationEnabled(bool rasterizationEnabled);
        void SetDepthAttachmentPixelFormat(PixelFormat depthAttachmentPixelFormat);
        void SetStencilAttachmentPixelFormat(PixelFormat stencilAttachmentPixelFormat);

    // TODO:
    // void SetInputPrimitiveTopology(PrimitiveTopologyClass inputPrimitiveTopology) MTLPP_AVAILABLE_MAC(10_11);
    // void SetSampleCount(uint32_t sampleCount);
    // void SetAlphaToCoverageEnabled(bool alphaToCoverageEnabled);
    // void SetAlphaToOneEnabled(bool alphaToOneEnabled);

    ns::Error error;
    RenderPipelineReflection reflection;
    PipelineOption pipelineOptions = PipelineOption(PipelineOption::ArgumentInfo | PipelineOption::BufferTypeInfo);
    _pipelineState = device.NewRenderPipelineState(descriptor, pipelineOptions, &reflection, &error);
    if (!_pipelineState)
        return false;

    const auto vertexArgs = reflection.GetVertexArguments();
    for (uint32_t i = 0; i < vertexArgs.GetSize(); i++)
    {
        // vertexArgs[i].GetBufferDataType
    }
    reflection.GetFragmentArguments();

#if 0
    for (MTLArgument *arg in reflectionObj.vertexArguments)
    {
        NSLog(@"Found arg: %@\n", arg.name);

        if (arg.bufferDataType == MTLDataTypeStruct)
        {
            for (MTLStructMember* uniform in arg.bufferStructType.members)
            {
                NSLog(@"uniform: %@ type:%lu, location : %lu", uniform.name, (unsigned long)uniform.dataType, (unsigned long)uniform.offset);         
            }
        }
    }
#endif

}
#endif

bool MTLRenderPipeline::create(const GraphicsPipelineDesc& desc)
{
    _pipelineDesc = desc;
    return true;
}

void MTLRenderPipeline::destroy()
{
    _pipelineState = ns::Handle{};
}

const GraphicsPipelineDesc& MTLRenderPipeline::getPipelineDesc() const
{
    return _pipelineDesc;
}

void MTLRenderPipeline::setDevice(GraphicsDevicePtr device)
{
}

GraphicsDevicePtr MTLRenderPipeline::getDevice()
{
    return GraphicsDevicePtr();
}

#endif // EL_BUILD_METAL
