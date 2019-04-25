#pragma once

#include "predefine.h"

#if EL_PLAT_APPLE

#include <graphics_data.h>

namespace el {

    class MTLBuffer final : public GraphicsData
    {
    public:

        MTLBuffer();
        ~MTLBuffer();

        bool create(GraphicsDataDesc desc) override;
        void destroy();

        void bind() const;
        
        const GraphicsDataDesc& getDesc() const override;

    private:

        GraphicsDataDesc _desc;
    };

} // namespace el {

#endif // EL_PLAT_APPLE
