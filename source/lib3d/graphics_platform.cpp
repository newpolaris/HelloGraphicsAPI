#include <el/graphics_platform.h>
#if _BUILD_METAL
#include <Metal/metal_platform.h>
#endif

namespace el {

    DefaultPlatform::~DefaultPlatform()
    {
    }

    DefaultPlatform* DefaultPlatform::create(GraphicsDeviceType hint)
    {
#if _BUILD_METAL
        return new PlatformMetal;
#else
        return nullptr;
#endif
    }

    void DefaultPlatform::destroy(DefaultPlatform** platform)
    {
        delete *platform;
        *platform = nullptr;
    }
}
