#pragma once

#include "predefine.h"

#if EL_PLAT_APPLE

#include "graphics_types.h"
#include "graphics_texture.h"
#include "mtlpp.hpp"

namespace el {
    
    class MTLTexture final : public GraphicsTexture
    {
    public:

        MTLTexture();
        ~MTLTexture();

        bool create(GraphicsTextureDesc desc);
        void destroy();
        
        const GraphicsTextureDesc& getTextureDesc() const override;

    private:

        GraphicsTextureDesc _textureDesc;
    };

} // namespace el {

#endif // EL_PLAT_APPLE
