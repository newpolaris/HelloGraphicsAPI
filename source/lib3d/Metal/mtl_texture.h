#ifndef __MTL_TEXTURE_H__
#define __MTL_TEXTURE_H__

#include <el_predefine.h>

#if EL_BUILD_METAL

#include <graphics_types.h>
#include <graphics_texture.h>
#include "mtl_types.h"
#include <mtlpp.hpp>

namespace el {

    class MTLTexture final : public GraphicsTexture
    {
    public:

        MTLTexture();
        ~MTLTexture();

        bool create(const GraphicsTextureDesc& desc);
        void destroy();

        void setDevice(const GraphicsDevicePtr& device);
        GraphicsDevicePtr getDevice();

        const GraphicsTextureDesc& getDesc() const override;
        
        const mtlpp::Texture& getTexture() const;

    private:

        mtlpp::Texture _texture;
        MTLDeviceWeakPtr _device;
        GraphicsTextureDesc _textureDesc;
    };

} // namespace el {

#endif // EL_BUILD_METAL

#endif // __MTL_TEXTURE_H__
