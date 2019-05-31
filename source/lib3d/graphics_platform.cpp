#include <el/graphics_platform.h>
#include <Metal/metal_platform.h>

namespace el {

    DefaultPlatform::~DefaultPlatform()
    {
    }

    DefaultPlatform* DefaultPlatform::create(GraphicsDeviceType hint)
    {
        return new PlatformMetal;
    }

    void DefaultPlatform::destroy(DefaultPlatform** platform)
    {
        delete *platform;
        *platform = nullptr;
    }
}
