#pragma once

#include <string>
#include <chrono>
#include <graphics_device.h>

#include "gl.h"

namespace el {

class GLProfileBusyWait final
{
public:

    using time_point = std::chrono::high_resolution_clock::time_point;

    GLProfileBusyWait();
    ~GLProfileBusyWait();

    void start();
    void end();

    void create();
    void destroy();

    void setDevice(const GraphicsDevicePtr& device);
    GraphicsDevicePtr getDevice();

    void setName(std::string name);
    const std::string& getName() const;

    float getCpuTime() const;
    float getGpuTime() const;

private:

    // TODO: maintain as a member until context integrate into one
    PFNGLGETQUERYOBJECTUI64VPROC _fGetQueryObjectui64v;

    bool _isSupportTimerQuery;

    GLuint _query[2];
    float _cpuTime;
    float _gpuTime;

    std::string _name;
    time_point _cpuTimePoint[2];

    GraphicsDeviceWeakPtr _device;
};

} // namespace el {
