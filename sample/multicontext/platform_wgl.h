#ifndef __EL_PLATFORM_WGL_H__
#define __EL_PLATFORM_WGL_H__

#include "platform.h"

namespace el {

struct PlatformWGL : public Platform
{
    PlatformWGL();

    virtual bool create(void* window) override;
    virtual void destroy() override;
    virtual void swapBuffer() override;
    virtual void makeCurrent() override;

private:

    struct PlatformCocoaGLImpl* _impl;
};

} // namespace el

#endif // __EL_PLATFORM_WGL_H__
