#pragma once

#include <graphics_input_layout.h>

namespace el {

    class GLInputLayout final : public GraphicsInputLayout
    {
    public:

        GLInputLayout();
        ~GLInputLayout();

        bool create(GraphicsInputLayoutDesc desc);
        void destory();

        const GraphicsInputLayoutDesc& getDesc() const override;

    private:

        GraphicsInputLayoutDesc _desc;
    };

} // namespace el {
