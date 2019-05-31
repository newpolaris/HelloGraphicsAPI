#ifndef __GRAPHICS_DEPTH_STENCIL_H__
#define __GRAPHICS_DEPTH_STENCIL_H__

#include <graphics_types.h>

namespace el {

    class GraphicsStencilOpState final
    {
        public:

            GraphicsStencilOpState();

            void setFailOp(GraphicsStencilOp func);
            GraphicsStencilOp getFailOp() const;

            void setPassOp(GraphicsStencilOp func);
            GraphicsStencilOp getPassOp() const;

            void setDepthFailOp(GraphicsStencilOp func);
            GraphicsStencilOp getDepthFailOp() const;

            void setCompareOp(GraphicsCompareOp func);
            GraphicsCompareOp getCompareOp() const;

            void setCompareMask(uint32_t mask);
            uint32_t getCompareMask() const;

            void setWriteMask(uint32_t mask);
            uint32_t getWriteMask() const;

            friend bool operator==(const GraphicsStencilOpState&, const GraphicsStencilOpState&);
            friend bool operator<(const GraphicsStencilOpState&, const GraphicsStencilOpState&);

        private:

            static const uint32_t defaultMask;

            GraphicsStencilOp _failOp;
            GraphicsStencilOp _passOp;
            GraphicsStencilOp _depthFailOp;
            GraphicsCompareOp _compareOp;
            uint32_t _compareMask;
            uint32_t _writeMask;
    };

    inline bool operator==(const GraphicsStencilOpState& a, const GraphicsStencilOpState& b)
    {
        return (a._failOp == b._failOp)
            && (a._passOp == b._passOp)
            && (a._depthFailOp == b._depthFailOp)
            && (a._compareOp == b._compareOp)
            && (a._compareMask == b._compareMask)
            && (a._writeMask == b._writeMask);
    }

    inline bool operator<(const GraphicsStencilOpState &a, const GraphicsStencilOpState &b)
    {
        return std::tie(a._failOp, a._passOp, a._depthFailOp, a._compareOp, a._compareMask, a._writeMask)
             < std::tie(b._failOp, b._passOp, b._depthFailOp, b._compareOp, b._compareMask, b._writeMask);
    }

    // From vulkan sepc.
    class GraphicsDepthStencilDesc final
    {
        public:

            GraphicsDepthStencilDesc();

            void setDepthTestEnable(bool enable);
            bool getDepthTestEnable() const;

            void setDepthWriteEnable(bool enable);
            bool getDepthWriteEnable() const;

            void setDepthCompareOp(GraphicsCompareOp func);
            GraphicsCompareOp getDepthCompareOp() const;

            void setStencilTestEnable(bool enable);
            bool getStencilTestEnable() const;

            void setFrontFaceStencil(const GraphicsStencilOpState& state);
            const GraphicsStencilOpState& getFrontFaceStencil() const;

            void setBackFaceStencil(const GraphicsStencilOpState& state);
            const GraphicsStencilOpState& getBackFaceStencil() const;

            friend bool operator==(const GraphicsDepthStencilDesc&, const GraphicsDepthStencilDesc&);
            friend bool operator<(const GraphicsDepthStencilDesc&, const GraphicsDepthStencilDesc&);

        private:

            bool _enableDepthTest;
            bool _enableDepthWrite;
            GraphicsCompareOp _depthCompareOp;
            bool _enableStencilTest;
            GraphicsStencilOpState _frontFaceStencil;
            GraphicsStencilOpState _backFaceStencil;
    };

    inline bool operator==(const GraphicsDepthStencilDesc& a, const GraphicsDepthStencilDesc& b)
    {
        return (a._enableDepthTest == b._enableDepthTest)
            && (a._enableDepthWrite == b._enableDepthWrite)
            && (a._depthCompareOp == b._depthCompareOp)
            && (a._enableStencilTest == b._enableStencilTest)
            && (a._frontFaceStencil == b._frontFaceStencil)
            && (a._backFaceStencil == b._backFaceStencil);
    }

    inline bool operator!=(const GraphicsDepthStencilDesc& a, const GraphicsDepthStencilDesc& b)
    {
        return !(a == b);
    }

    inline bool operator<(const GraphicsDepthStencilDesc &a, const GraphicsDepthStencilDesc &b)
    {
        return std::tie(a._enableDepthTest, a._enableDepthWrite, a._depthCompareOp, a._enableStencilTest, a._frontFaceStencil, a._backFaceStencil)
             < std::tie(b._enableDepthTest, b._enableDepthWrite, b._depthCompareOp, b._enableStencilTest, b._frontFaceStencil, a._backFaceStencil);
    }

    class GraphicsDepthStencil
    {
        public:

            GraphicsDepthStencil();
            virtual ~GraphicsDepthStencil();

            virtual const GraphicsDepthStencilDesc& getDepthStencilDesc() const = 0;
    };

} // namespace el {

#endif // __GRAPHICS_DEPTH_STENCIL_H__
