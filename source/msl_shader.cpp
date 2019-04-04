#include "predefine.h"

#if EL_PLAT_IOS || EL_PLAT_OSX

#include "msl_shader.h"

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

#endif // #if EL_PLAT_IOS || EL_PLAT_OSX

