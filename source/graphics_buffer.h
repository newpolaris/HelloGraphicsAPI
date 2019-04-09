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

        void setDataType(GraphicsDataType dataType);
        GraphicsDataType getDataType() const;

        void setUsage(GraphicsUsageFlags usage);
        GraphicsUsageFlags getUsage() const;

    private:

        const char* _data;
        size_t _dataSize;
        GraphicsDataType _dataType;
        GraphicsUsageFlags _usage;
    };

    class GraphicsBuffer
    {
    public:

        GraphicsBuffer();
        virtual ~GraphicsBuffer();

        virtual bool create(const GraphicsBufferDesc& desc) = 0;

    private:

        GraphicsBuffer(const GraphicsBuffer&) = delete;
        GraphicsBuffer& operator=(const GraphicsBuffer&) = delete;
    };

} // namespace el {
