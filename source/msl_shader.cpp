#include "msl_shader.h"

#if EL_PLAT_APPLE

using namespace el;

MSLShader::MSLShader()
{
}

MSLShader::~MSLShader()
{
}

bool MSLShader::create(GraphicsShaderStageFlagBits stage, const char* shaderCode)
{
    mtlpp::Device device = mtlpp::Device::CreateSystemDefaultDevice();

    _library = device.NewLibrary(shaderCode, mtlpp::CompileOptions(), nullptr);
    assert(_library);

    _function = _library.NewFunction("vertFunc");
    assert(_function);
    return true;
}

void MSLShader::destroy()
{
}

const GraphicsShaderDesc& MSLShader::getDesc() const
{
    return _desc;
}

#endif // #if EL_PLAT_APPLE
