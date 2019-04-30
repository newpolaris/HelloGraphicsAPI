#pragma once

#include "graphics_types.h"

namespace el {

}

namespace el {

    class GraphicsTextureDesc final
    {
    public:

        GraphicsTextureDesc();

        void setWidth(uint32_t width);
        uint32_t getWidth() const;

        void setHeight(uint32_t height);
        uint32_t getHeight() const;

        void setDepth(uint32_t depth);
        uint32_t getDepth() const;

        void setLevels(uint32_t levels);
        uint32_t getLevels() const;

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

		void setStream(stream_t* stream);
		const stream_t* getStream() const;

		void setStreamSize(streamsize_t stream);
		streamsize_t getStreamSize() const;

        void setDim(GraphicsTextureDim target);
        GraphicsTextureDim getDim() const;

        void setPixelFormat(GraphicsPixelFormat format);
        GraphicsPixelFormat getPixelFormat() const;

        void setPixelAlignment(GraphicsPixelAlignment align);
        GraphicsPixelAlignment getPixelAlignment() const;

        void setTextureUsage(GraphicsTextureUsageFlags usage);
        GraphicsTextureUsageFlags getTextureUsage() const;

	private:

		uint32_t _width;
		uint32_t _height;
		uint32_t _depth;
        uint32_t _levels;
        GraphicsSamplerAddressMode _addressModeU;
        GraphicsSamplerAddressMode _addressModeV;
        GraphicsSamplerAddressMode _addressModeW;
        GraphicsFilter _minFilter;
        GraphicsFilter _magFilter;
        GraphicsSamplerMipmapMode _samplerMipmapMode;
        float _anisotropyLevel;

		stream_t* _stream;
		streamsize_t _streamSize;
		GraphicsTextureDim _dim;
        GraphicsPixelFormat _pixelFormat;
        GraphicsPixelAlignment _pixelAlignment;
        GraphicsTextureUsageFlags _textureUsage;
	};

    class GraphicsTexture
    {
    public:

        GraphicsTexture();
        virtual ~GraphicsTexture();

        virtual const GraphicsTextureDesc& getTextureDesc() const = 0;

    private:

        GraphicsTexture(const GraphicsTexture&);
        GraphicsTexture& operator=(const GraphicsTexture&);
    };

} // namespace el {
