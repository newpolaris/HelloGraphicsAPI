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

    private:

        GraphicsStencilOp _failOp;
        GraphicsStencilOp _passOp;
        GraphicsStencilOp _depthFailOp;
        GraphicsCompareOp _compareOp;
        uint32_t _compareMask;
        uint32_t _writeMask;
    };

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

    private:

        bool _enableDepthTest;
        bool _enableDepthWrite;
        GraphicsCompareOp _depthCompareOp;
        bool _enableStencilTest;
        GraphicsStencilOpState _frontFaceStencil;
        GraphicsStencilOpState _backFaceStencil;
    };

    class GraphicsDepthStencil
    {
    public:

        GraphicsDepthStencil();
        virtual ~GraphicsDepthStencil();

        virtual const GraphicsDepthStencilDesc& getDepthStencilDesc() const = 0;
    };

} // namespace el {

#endif // __GRAPHICS_DEPTH_STENCIL_H__
