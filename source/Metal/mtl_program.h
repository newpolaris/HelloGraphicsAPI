#pragma once

#include "predefine.h"

#if EL_PLAT_APPLE

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

        bool create(GraphicsProgramDesc desc);
        void destory();

        void use() const;

        const GraphicsProgramDesc& getProgramDesc() const override;
        const GraphicsVertexAttributes& getVertexAttributes() const override;

    private:

        GraphicsProgramDesc _programDesc;
        GraphicsVertexAttributes _vertexAttributes;
    };

} // namespace el {

#endif // EL_PLAT_APPLE
