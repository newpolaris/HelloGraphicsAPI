#ifndef __METAL_RESOURCES_H__
#define __METAL_RESOURCES_H__

#include "metal_types.h"

namespace el {

struct MetalProgram final
{
    MetalProgram();
    ~MetalProgram();
    
    bool create(id<MTLDevice> device, const char* vertex, const char* fragment);
    void destroy();

    id<MTLFunction> vertexFunction;
    id<MTLFunction> fragmentFunction;
};

} // namespace el {

#endif // __METAL_RESOURCES_H__
