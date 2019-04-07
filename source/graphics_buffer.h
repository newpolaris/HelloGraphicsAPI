#pragma once

#include "graphics_types.h"

namespace el {

    class GraphicsBufferDesc final
    {
    public:

        GraphicsBufferDesc();

		void setData(const char* data);
		const char* getData() const;

		void setDataSize(size_t size);
		size_t getDataSize() const;

    private:

		const char* _data;
		size_t _dataSize;
    };

    class GraphicsBuffer
    {
    public:

        GraphicsBuffer();
        virtual ~GraphicsBuffer();

		virtual bool create(const GraphicsBufferDesc& desc) = 0;

    private:

        const GraphicsBuffer(const GraphicsBuffer&) = delete;
        GraphicsBuffer& operator=(const GraphicsBuffer&) = delete;
    };

} // namespace el {
