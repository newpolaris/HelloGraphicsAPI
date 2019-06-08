#ifndef __EL_BACKEND_PLATFORM_H__
#define __EL_BACKEND_PLATFORM_H__

namespace el {

struct Platform
{
    virtual ~Platform() {}
    virtual bool create(void* window) = 0;
    virtual void destroy() = 0;
    virtual void swapBuffer() = 0;
    virtual void makeCurrent() = 0;
};

} // namespace el

#endif // __EL_BACKEND_PLATFORM_H__