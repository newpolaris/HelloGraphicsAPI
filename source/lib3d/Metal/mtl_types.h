#ifndef __MTL_TYPES_H__
#define __MTL_TYPES_H__

#include <graphics_types.h>
#include <mtlpp.hpp>

namespace el {

    class MTLDeviceDesc;

    typedef std::shared_ptr<class MTLDevice> MTLDevicePtr;
    typedef std::shared_ptr<class MTLContext> MTLDeviceContextPtr;

    typedef std::weak_ptr<class MTLDevice> MTLDeviceWeakPtr;

    mtlpp::PixelFormat asPixelFormat(GraphicsPixelFormat format);
    mtlpp::TextureUsage asTextureUsage(GraphicsTextureUsageFlags usage);
    mtlpp::SamplerMinMagFilter asSamplerMinMagFilter(GraphicsFilter filter);;

    mtlpp::SamplerMipFilter asSamplerMipFilter(GraphicsSamplerMipmapMode mode);
    mtlpp::SamplerAddressMode asSamplerAddressMode(GraphicsSamplerAddressMode mode);
    mtlpp::SamplerBorderColor asSamplerBorderColor(GraphicsBorderColor color);
    mtlpp::CompareFunction asCompareFunction(GraphicsCompareOp func);
    mtlpp::StencilOperation asStencilOperation(GraphicsStencilOp func);
    mtlpp::VertexFormat asVertexFormat(GraphicsVertexFormat format);
    mtlpp::VertexStepFunction asVertexStepFunction(GraphicsInputRate func);
};

#endif // __MTL_TYPES_H__
