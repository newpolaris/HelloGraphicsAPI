#include "graphics_depth_stencil.h"

using namespace el;

const uint32_t defaultMask = static_cast<uint32_t>(-1);

// Default vaules are referenced from directx-11

GraphicsStencilOpState::GraphicsStencilOpState() : 
    _failOp(GraphicsStencilOp::GraphicsStencilOpKeep),
    _passOp(GraphicsStencilOp::GraphicsStencilOpKeep),
    _depthFailOp(GraphicsStencilOp::GraphicsStencilOpKeep),
    _compareOp(GraphicsCompareOp::GraphicsCompareOpAlways),
    _compareMask(defaultMask),
    _writeMask(defaultMask)
{
}

void GraphicsStencilOpState::setFailOp(GraphicsStencilOp func)
{
    _failOp = func;
}

GraphicsStencilOp GraphicsStencilOpState::getFailOp() const
{
    return _failOp;
}

void GraphicsStencilOpState::setPassOp(GraphicsStencilOp func)
{
    _passOp = func;
}

GraphicsStencilOp GraphicsStencilOpState::getPassOp() const
{
    return _passOp;
}

void GraphicsStencilOpState::setDepthFailOp(GraphicsStencilOp func)
{
    _depthFailOp = func;
}

GraphicsStencilOp GraphicsStencilOpState::getDepthFailOp() const
{
    return _depthFailOp;
}

void GraphicsStencilOpState::setCompareOp(GraphicsCompareOp func)
{
    _compareOp = func;
}

GraphicsCompareOp GraphicsStencilOpState::getCompareOp() const
{
    return _compareOp;
}

void GraphicsStencilOpState::setCompareMask(uint32_t mask)
{
    _compareMask = mask;
}

uint32_t GraphicsStencilOpState::getCompareMask() const
{
    return _compareMask;
}

void GraphicsStencilOpState::setWriteMask(uint32_t mask)
{
    _writeMask = mask;
}

uint32_t GraphicsStencilOpState::getWriteMask() const
{
    return _writeMask;
}

GraphicsDepthStencilDesc::GraphicsDepthStencilDesc() :
    _enableDepthTest(true),
    _enableDepthWrite(true),
    _depthCompareOp(GraphicsCompareOp::GraphicsCompareOpLess),
    _enableStencilTest(false)
{
}

void GraphicsDepthStencilDesc::setDepthTestEnable(bool enable)
{
    _enableDepthTest = enable;
}

bool GraphicsDepthStencilDesc::getDepthTestEnable() const
{
    return _enableDepthTest;
}

void GraphicsDepthStencilDesc::setDepthWriteEnable(bool enable)
{
    _enableDepthWrite = enable;
}

bool GraphicsDepthStencilDesc::getDepthWriteEnable() const
{
    return _enableDepthWrite;
}

void GraphicsDepthStencilDesc::setDepthCompareOp(GraphicsCompareOp func)
{
    _depthCompareOp = func;
}

GraphicsCompareOp GraphicsDepthStencilDesc::getDepthCompareOp() const
{
    return _depthCompareOp;
}

void GraphicsDepthStencilDesc::setStencilTestEnable(bool enable)
{
    _enableStencilTest = enable;
}

bool GraphicsDepthStencilDesc::getStencilTestEnable() const
{
    return _enableStencilTest;
}

void GraphicsDepthStencilDesc::setFrontFaceStencil(const GraphicsStencilOpState& state)
{
    _frontFaceStencil = state;
}

const GraphicsStencilOpState& GraphicsDepthStencilDesc::getFrontFaceStencil() const
{
    return _frontFaceStencil;
}

void GraphicsDepthStencilDesc::setBackFaceStencil(const GraphicsStencilOpState& state)
{
    _backFaceStencil = state;
}

const GraphicsStencilOpState& GraphicsDepthStencilDesc::getBackFaceStencil() const
{
    return _backFaceStencil;
}

GraphicsDepthStencil::GraphicsDepthStencil()
{
}

GraphicsDepthStencil::~GraphicsDepthStencil()
{
}
