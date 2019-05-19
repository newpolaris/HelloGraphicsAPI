#ifndef __METLA_TYPES_H__
#define __METLA_TYPES_H__

#include <Metal/Metal.h>
#include <QuartzCore/CAMetalLayer.h>
#include <el_debug.h>
#include <el_platform.h>
#include <math_types.h>

namespace el {
    struct MetalDriver;
    struct MetalProgram;

    inline MTLClearColor asMTLClearColor(const math::float4 &color)
    {
        return MTLClearColorMake(color.r, color.g, color.b, color.a);
    }

}

#endif // __METLA_TYPES_H__
