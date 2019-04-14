#include "mtl_program.h"

#if EL_PLAT_APPLE

using namespace el;

MTLProgram::MTLProgram()
{
}

MTLProgram::~MTLProgram()
{
}

bool MTLProgram::create(GraphicsProgramDesc desc)
{
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

#endif // #if EL_PLAT_APPLE
