#pragma once

#include "graphics_types.h"

namespace el {

    // tempolar
    // from apple format;
    enum class VertexFormat
    {
        Invalid = 0,

        UChar2 = 1,
        UChar3 = 2,
        UChar4 = 3,

        Char2 = 4,
        Char3 = 5,
        Char4 = 6,

        UChar2Normalized = 7,
        UChar3Normalized = 8,
        UChar4Normalized = 9,

        Char2Normalized = 10,
        Char3Normalized = 11,
        Char4Normalized = 12,

        UShort2 = 13,
        UShort3 = 14,
        UShort4 = 15,

        Short2 = 16,
        Short3 = 17,
        Short4 = 18,

        UShort2Normalized = 19,
        UShort3Normalized = 20,
        UShort4Normalized = 21,

        Short2Normalized = 22,
        Short3Normalized = 23,
        Short4Normalized = 24,

        Half2 = 25,
        Half3 = 26,
        Half4 = 27,

        Float = 28,
        Float2 = 29,
        Float3 = 30,
        Float4 = 31,

        Int = 32,
        Int2 = 33,
        Int3 = 34,
        Int4 = 35,

        UInt = 36,
        UInt2 = 37,
        UInt3 = 38,
        UInt4 = 39,

        Int1010102Normalized = 40,
        UInt1010102Normalized = 41,
    };

    // from vulkan spec.

    enum GraphicsInputRate
    {
        GraphicsInputRateVertex = 0,
        GraphicsInputRateInstance = 1
    };

    class GraphicsInputBinding final
    {
    public:

        GraphicsInputBinding();
        GraphicsInputBinding(uint32_t binding, uint32_t stride, GraphicsInputRate rate = GraphicsInputRateVertex);

        void setBinding(uint32_t binding);
        uint32_t getBinding() const;

        void setStride(uint32_t stride);
        uint32_t getStride() const;

        void setInputRate(GraphicsInputRate rate);
        GraphicsInputRate getInputRate() const;

    private:

        uint32_t _binding;
        uint32_t _stride;
        GraphicsInputRate _inputRate;
    };

    class GraphicsInputAttribute final
    {
    public:

        GraphicsInputAttribute();
        GraphicsInputAttribute(uint32_t binding, std::string name, VertexFormat format, uint32_t offset = 0);
        GraphicsInputAttribute(uint32_t binding, uint32_t location, VertexFormat format, uint32_t offset = 0);

        void setBinding(uint32_t binding);
        uint32_t getBinding() const;

        void setLocation(uint32_t location);
        uint32_t getLocation() const;

        void setName(std::string name);
        const std::string& getName() const;

        void setFormat(VertexFormat format);
        VertexFormat getFormat() const;

        void setOffset(uint32_t offset);
        uint32_t getOffset() const;


    private:

        uint32_t _binding;
        uint32_t _location;
        std::string _name;
        VertexFormat _format;
        uint32_t _offset;
    };

    class GraphicsInputLayoutDesc final
    {
    public:

        GraphicsInputLayoutDesc();

        void addAttribute(GraphicsInputAttribute attrib);
        void addBinding(GraphicsInputBinding binding);

        void setAttributes(GraphicsInputAttributes attrib);
        const GraphicsInputAttributes& getAttributes() const;

        void setBindings(GraphicsInputBindings bindings);
        const GraphicsInputBindings& getBindings() const;

    private:

        GraphicsInputAttributes _attributes;
        GraphicsInputBindings _bindings;
    };

    class GraphicsInputLayout
    {
    public:

        GraphicsInputLayout();
        virtual ~GraphicsInputLayout();

        virtual const GraphicsInputLayoutDesc& getDesc() const = 0;

    private:

        GraphicsInputLayoutDesc _desc;
    };

} // namespace el {
