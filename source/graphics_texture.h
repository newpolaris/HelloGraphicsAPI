#pragma once

#include "graphics_types.h"

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

        void setDim(GraphicsTextureDim target);
        GraphicsTextureDim getDim() const;

        uint32_t getWrapS() const;
        void setWrapS(uint32_t wrap);

        uint32_t getWrapT() const;
        void setWrapT(uint32_t wrap);

        uint32_t getWrapR() const;
        void setWrapR(uint32_t wrap);

        uint32_t getMinFilter() const;
        void setMinFilter(uint32_t filter);

        uint32_t getMagFilter() const;
        void setMagFilter(uint32_t filter);

        float getAnisotropyLevel() const;
        void setAnisotropyLevel(float anisoLevel);

		void setStream(stream_t* stream);
		const stream_t* getStream() const;

		void setStreamSize(streamsize_t stream);
		streamsize_t getStreamSize() const;

        void setPixelFormat(GraphicsPixelFormat format);
        GraphicsPixelFormat getPixelFormat() const;

	private:

		uint32_t _width;
		uint32_t _height;
		uint32_t _depth;
        uint32_t _level;
        uint32_t _wrapS;
        uint32_t _wrapT;
        uint32_t _wrapR;
        uint32_t _minFilter;
        uint32_t _magFilter;
        float _anisotropyLevel;

		stream_t* _stream;
		streamsize_t _streamSize;
		GraphicsTextureDim _dim;
        GraphicsPixelFormat _pixelFormat;
	};

    class GraphicsTexture
    {
    public:

        GraphicsTexture();
        virtual ~GraphicsTexture();

        virtual const GraphicsTextureDesc& getTextureDesc() const = 0;

    private:

        GraphicsTexture(const GraphicsTexture&) = delete;
        GraphicsTexture& operator=(const GraphicsTexture&) = delete;
    };

} // namespace el {
