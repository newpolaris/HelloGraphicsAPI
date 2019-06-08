#ifndef __EL_PLATFORM_NSGL_H__
#define __EL_PLATFORM_NSGL_H__

#include "platform.h"

namespace el {

struct PlatformNSGL : public Platform
{
    PlatformNSGL();

    virtual bool create(void* window) override;
    virtual void destroy() override;
    virtual void swapBuffer() override;
    virtual void makeCurrent() override;

private:

    struct PlatformNSGLImpl* _impl;

};

} // namespace el

#endif // __EL_PLATFORM_NSGL_H__
