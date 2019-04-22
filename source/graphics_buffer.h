#pragma once

#include "graphics_types.h"

namespace el {

    class GraphicsBufferDesc final
    {
    public:

        GraphicsBufferDesc();

        void setData(const stream_t* data);
        const stream_t* getData() const;

        streamsize_t getDataSize() const;

        void setNumElements(size_t numElements);
        size_t getNumElements() const;

        void setElementSize(size_t elementSize);
        size_t getElementSize() const;

        void setDataType(GraphicsDataType dataType);
        GraphicsDataType getDataType() const;

        void setUsage(GraphicsUsageFlags usage);
        GraphicsUsageFlags getUsage() const;

    private:

        const stream_t* _data;
        size_t _numElements;
        size_t _elementSize;
        GraphicsDataType _dataType;
        GraphicsUsageFlags _usage;
    };

    class GraphicsBuffer
    {
    public:

        GraphicsBuffer();
        virtual ~GraphicsBuffer();

        virtual bool create(GraphicsBufferDesc desc) = 0;

        virtual const GraphicsBufferDesc& getDesc() const = 0;

    private:

        GraphicsBuffer(const GraphicsBuffer&) = delete;
        GraphicsBuffer& operator=(const GraphicsBuffer&) = delete;
    };

} // namespace el {
