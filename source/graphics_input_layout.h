#pragma once

#include "graphics_types.h"

namespace el {

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

    enum class GraphicsVertexInputRate
    {
        GraphicsVertexInputRateVertex = 0,
        GraphicsVertexInputRateInstance = 1
    };

    class GraphicsVertexInputBindingDesc final
    {
    public:

        GraphicsVertexInputBindingDesc();

        void setBinding(uint32_t binding);
        uint32_t getBinding() const;

        void setStride(uint32_t stride);
        uint32_t getStride() const;

    private:

        uint32_t _binding;
        uint32_t _stride;
        GraphicsVertexInputRate _inputRate;
    };

    class GraphicsVertexInputAttributeDesc final
    {
    public:
    };

    class GraphicsInputLayoutDesc final
    {
    public:

        GraphicsInputLayoutDesc();

        void setAttributes(GraphicsVertexInputAttributeDescs descs);
        const GraphicsVertexInputAttributeDescs& getAttributes() const;

        void setBindings(GraphicsVertexInputBindingDescs descs);
        const GraphicsVertexInputBindingDescs& getBindings() const;

    private:

        GraphicsVertexInputAttributeDescs _attributes;
        GraphicsVertexInputBindingDescs _bindings;
    };

    class GraphicsInputLayout
    {
    public:

        GraphicsInputLayout();
        virtual ~GraphicsInputLayout();

        bool create(GraphicsInputLayoutDesc desc);
        void destory();

        virtual const GraphicsInputLayoutDesc& getDesc();

    private:

        GraphicsInputLayoutDesc _desc;
    };

} // namespace el {
