#pragma once

#include "predefine.h"

#if EL_PLAT_APPLE

#include <graphics_types.h>
#include <graphics_texture.h>
#include "mtl_types.h"
#include "mtlpp.hpp"

namespace el {

    class MTLTexture final : public GraphicsTexture
    {
    public:

        MTLTexture();
        ~MTLTexture();

        bool create(GraphicsTextureDesc desc);
        void destroy();

        void setDevice(GraphicsDevicePtr device);
        GraphicsDevicePtr getDevice();

        const GraphicsTextureDesc& getTextureDesc() const override;

    private:

        mtlpp::Texture _texture;

        GraphicsTextureDesc _textureDesc;

        MTLDeviceWeakPtr _device;
    };

} // namespace el {

#endif // EL_PLAT_APPLE
