#ifndef __METAL_PLATFORM_H__
#define __METAL_PLATFORM_H__

#include <el/graphics_platform.h>

namespace el {

    class MetalPlatform : public DefaultPlatform
    {
    };

    class PlatformMetal final : public MetalPlatform
    {
    public:

        ~PlatformMetal() override;

        Driver* createDriver(void* sharedContext) override;
    };

} // namespace el {

#endif // __METAL_PLATFORM_H__
