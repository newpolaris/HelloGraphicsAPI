#include <Metal/mtl_program.h>

#if EL_BUILD_METAL

using namespace el;

MTLProgram::MTLProgram()
{
}

MTLProgram::~MTLProgram()
{
}

bool MTLProgram::create(const GraphicsProgramDesc& desc)
{
    mtlpp::Device device;

    _programDesc = std::move(desc);
    return true;
}

void MTLProgram::destory()
{
}

const GraphicsProgramDesc& MTLProgram::getProgramDesc() const
{
    return _programDesc;
}

const GraphicsVertexAttributes& MTLProgram::getVertexAttributes() const
{
    return _vertexAttributes;
}

#endif // #if EL_BUILD_METAL
