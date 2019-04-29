#include "mtl_reder_pipeline.h"

#if EL_BUILD_METAL

#include "debug.h"
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

bool MTLRenderPipeline::create(const GraphicsPipelineDesc& desc)
{
    ns::Error error = nullptr;

#if 0
    mltpp::device device;
    device.
        MTLRenderPipelineReflection* reflectionObj;
    MTLPipelineOption option = MTLPipelineOptionBufferTypeInfo | MTLPipelineOptionArgumentInfo;
    id <MTLRenderPipelineState> pso = [device newRenderPipelineStateWithDescriptor : pipelineStateDescriptor options : option reflection : &reflectionObj error : &error];

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

#endif // EL_BUILD_METAL
