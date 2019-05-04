#ifndef __PLATFORM_DRIVER_H__
#define __PLATFORM_DRIVER_H__

namespace el
{

    struct PlatformDriver
    {
        PlatformDriver();

        virtual bool create(void* window) = 0;
        virtual void destroy() = 0;
        virtual void swapBuffer() = 0;
    };

}

#endif // __PLATFORM_DRIVER_H__
