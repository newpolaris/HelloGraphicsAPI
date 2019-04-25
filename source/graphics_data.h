#pragma once

#include "graphics_types.h"

namespace el {

    class GraphicsDataDesc final
    {
    public:

        GraphicsDataDesc();

        void setData(const stream_t* data);
        const stream_t* getData() const;

        streamsize_t getDataSize() const;

        void setNumElements(size_t numElements);
        size_t getElementCount() const;

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

    class GraphicsData
    {
    public:

        GraphicsData();
        virtual ~GraphicsData();

        virtual bool create(GraphicsDataDesc desc) = 0;

        virtual const GraphicsDataDesc& getDesc() const = 0;

    private:

        GraphicsData(const GraphicsData&) = delete;
        GraphicsData& operator=(const GraphicsData&) = delete;
    };

} // namespace el {
