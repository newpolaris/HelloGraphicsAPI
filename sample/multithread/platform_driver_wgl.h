#ifndef __PLATFORM_DRIVER_WGL_H__
#define __PLATFORM_DRIVER_WGL_H__

#include "platform_driver.h"
#include <Windows.h>

namespace el 
{

    struct PlatformDriverWGL : public PlatformDriver
    {
        PlatformDriverWGL();

        virtual bool create(void* window) override;
        virtual void destroy() override;
        virtual void swapBuffer() override;
        virtual bool makeCurrent() override;

    private:

        HDC _hdc;
        HGLRC _context;
        HWND _hwnd;
    };

}

#endif // __PLATFORM_DRIVER_WGL_H__
