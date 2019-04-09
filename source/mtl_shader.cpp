#include "mtl_shader.h"

#if EL_PLAT_APPLE

using namespace el;

MTLShader::MTLShader()
{
}

MTLShader::~MTLShader()
{
}

bool MTLShader::create(GraphicsShaderStageFlagBits stage, const char* shaderCode)
{
    mtlpp::Device device = mtlpp::Device::CreateSystemDefaultDevice();

    _library = device.NewLibrary(shaderCode, mtlpp::CompileOptions(), nullptr);
    assert(_library);

    _function = _library.NewFunction("vertFunc");
    assert(_function);
    return true;
}

void MTLShader::destroy()
{
}

const GraphicsShaderDesc& MTLShader::getDesc() const
{
    return _desc;
}

#endif // #if EL_PLAT_APPLE
