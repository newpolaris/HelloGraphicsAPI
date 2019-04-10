#include "graphics_types.h"

namespace el {

	class MTLDeviceDesc;

    typedef std::shared_ptr<class MTLDevice> MetalDevicePtr;
    typedef std::weak_ptr<class MTLDevice> MetalDeviceWeakPtr;
    typedef std::shared_ptr<class MTLDeviceContext> MetalDeviceContextPtr;
};
