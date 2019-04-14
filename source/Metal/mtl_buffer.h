#pragma once

#include "predefine.h"

#if EL_PLAT_APPLE

#include "graphics_buffer.h"

namespace el {

    class MTLBuffer final : public GraphicsBuffer
    {
    public:

        MTLBuffer();
        ~MTLBuffer();

        bool create(GraphicsBufferDesc desc) override;
        void destroy();

        void bind() const;

    private:

        GraphicsBufferDesc _desc;
    };

} // namespace el {

#endif // EL_PLAT_APPLE
