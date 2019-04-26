#include "mtl_shader.h"

#include "debug.h"
#include "mtlpp.hpp"
#include "mtl_device.h"
#include "graphics_types.h"

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
	auto device = _device.lock();
	if (!device) return false;

    ns::Error error;
    _library = device->getDevice().NewLibrary(shaderCode, mtlpp::CompileOptions(), &error);
    if (!_library) {
        EL_TRACE("Failed to created pipeline state, error %s", 
				 error.GetLocalizedDescription().GetCStr());
		debug_break();
		return false;
	}

    _function = _library.NewFunction("main");
    if (!_library) {
		debug_break();
		return false;
	}
    return true;
}

void MTLShader::destroy()
{
}

const GraphicsShaderDesc& MTLShader::getDesc() const
{
    return _desc;
}

void MTLShader::setDevice(GraphicsDevicePtr device)
{
    _device = std::static_pointer_cast<MTLDevice>(std::move(device));
}

GraphicsDevicePtr MTLShader::getDevice()
{
    return _device.lock();
}

#endif // #if EL_PLAT_APPLE
