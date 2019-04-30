#pragma once

#include "predefine.h"

#if EL_BUILD_METAL

#include "graphics_types.h"
#include "graphics_program.h"
#include "mtlpp.hpp"
#include "linmath.h"

namespace el {

    class MTLProgram final : public GraphicsProgram
    {
    public:

        MTLProgram();
        ~MTLProgram();

        bool create(const GraphicsProgramDesc& desc);
        void destory();

        const GraphicsProgramDesc& getProgramDesc() const override;
        const GraphicsVertexAttributes& getVertexAttributes() const override;

    private:

        GraphicsVertexAttributes _vertexAttributes;
        GraphicsProgramDesc _programDesc;
    };

} // namespace el {

#endif // EL_BUILD_METAL
