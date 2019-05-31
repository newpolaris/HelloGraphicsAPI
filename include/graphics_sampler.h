#ifndef __GRAPHICS_SAMPLER_H__
#define __GRAPHICS_SAMPLER_H__

#include <graphics_types.h>

namespace el {

    class GraphicsSamplerDesc
    {
    public:

        GraphicsSamplerDesc();

        GraphicsSamplerAddressMode getAddressModeU() const;
        void setAddressModeU(GraphicsSamplerAddressMode wrap);

        GraphicsSamplerAddressMode getAddressModeV() const;
        void setAddressModeV(GraphicsSamplerAddressMode wrap);

        GraphicsSamplerAddressMode getAddressModeW() const;
        void setAddressModeW(GraphicsSamplerAddressMode wrap);

        GraphicsFilter getMinFilter() const;
        void setMinFilter(GraphicsFilter filter);

        GraphicsFilter getMagFilter() const;
        void setMagFilter(GraphicsFilter filter);

        GraphicsSamplerMipmapMode getSamplerMipmapMode() const;
        void setSamplerMipmapMode(GraphicsSamplerMipmapMode mode);

        float getAnisotropyLevel() const;
        void setAnisotropyLevel(float anisoLevel);

    private:

        GraphicsSamplerAddressMode _addressModeU;
        GraphicsSamplerAddressMode _addressModeV;
        GraphicsSamplerAddressMode _addressModeW;
        GraphicsFilter _minFilter;
        GraphicsFilter _magFilter;
        GraphicsSamplerMipmapMode _samplerMipmapMode;
        float _anisotropyLevel;
    };

    class GraphicsSampler
    {
    public:

        GraphicsSampler();
        virtual ~GraphicsSampler();

        virtual const GraphicsSamplerDesc& getDesc() const = 0;
    };

} // namespace el {

#endif // __GRAPHICS_SAMPLER_H__
