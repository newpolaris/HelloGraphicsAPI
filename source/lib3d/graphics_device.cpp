#include <graphics_device.h>

#include <OpenGL/gl_device.h>
#include <Metal/mtl_device.h>

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

GraphicsDevicePtr el::createDevice(GraphicsDeviceDesc desc)
{
    if (desc.getType() == GraphicsDeviceTypeOpenGL)
    {
        auto device = std::make_shared<GLDevice>();
        if (device->create(std::move(desc)))
            return device;
    }
#if EL_PLAT_APPLE
    else if (desc.getType() == GraphicsDeviceTypeMetal)
    {
        auto device = std::make_shared<MTLDevice>();
        if (device->create(std::move(desc)))
            return device;
    }
#endif
    return nullptr;
}
