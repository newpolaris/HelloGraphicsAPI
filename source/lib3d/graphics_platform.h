#ifndef __GRAPHICS_PLATFORM_H__
#define __GRAPHICS_PLATFORM_H__

#include <graphics_types.h>

namespace el {
    class Driver;

    class DefaultPlatform
    {
    public:

        virtual ~DefaultPlatform();

        static DefaultPlatform* create(GraphicsDeviceType hint);
        static void destroy(DefaultPlatform** platform);

        virtual Driver* createDriver(void* sharedContext) = 0;
    };
} // namespace el {

#endif // __GRAPHICS_PLATFORM_H__
