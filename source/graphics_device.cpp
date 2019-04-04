#include "graphics_device.h"

#include "gl_device.h"

using namespace el;

GraphicsDeviceDesc::GraphicsDeviceDesc() : 
	_type(GraphicsDeviceTypeOpenGL)
{
}

void GraphicsDeviceDesc::setType(GraphicsDeviceType type)
{
	_type = type;
}

GraphicsDeviceType GraphicsDeviceDesc::getType() const
{
	return _type;
}

GraphicsDevice::GraphicsDevice()
{
}

GraphicsDevice::~GraphicsDevice()
{
}

GraphicsDevicePtr el::createDevice(const GraphicsDeviceDesc& desc)
{
	if (desc.getType() == GraphicsDeviceTypeOpenGL)
	{
		auto device = std::make_shared<GLDevice>();
		if (device->create(desc))
			return device;
	} 
	else if (desc.getType() == GraphicsDeviceTypeMetal)
	{
#if 0
		auto device = std::make_shared<MTLDevice>();
		if (device->create(desc))
			return device;
#endif
	}
	return nullptr;
}