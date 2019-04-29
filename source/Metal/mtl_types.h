#include "graphics_types.h"
#include "mtlpp.hpp"

namespace el {

    class MTLDeviceDesc;

    typedef std::shared_ptr<class MTLDevice> MTLDevicePtr;
    typedef std::shared_ptr<class MTLContext> MTLDeviceContextPtr;

    typedef std::weak_ptr<class MTLDevice> MTLDeviceWeakPtr;

    mtlpp::PixelFormat asPixelFormat(GraphicsPixelFormat format);
    mtlpp::TextureUsage asTextureUsage(GraphicsTextureUsageFlags usage);
};
