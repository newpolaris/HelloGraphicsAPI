#include "metal_platform.h"
#include "metal_driver.h"

namespace el {

    PlatformMetal::~PlatformMetal()
    {
    }

    Driver* PlatformMetal::createDriver(void* sharedContext)
    {
        return new MetalDriver(this);
    }

}
