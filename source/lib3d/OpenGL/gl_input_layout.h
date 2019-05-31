#ifndef __GL_INPUT_LAYOUT_H__
#define __GL_INPUT_LAYOUT_H__

#include <el/graphics_input_layout.h>

namespace el {

    class GLInputLayout final : public GraphicsInputLayout
    {
    public:

        GLInputLayout();
        ~GLInputLayout();

        bool create(GraphicsInputLayoutDesc desc);
        void destroy();

        const GraphicsInputLayoutDesc& getDesc() const override;

    private:

        GraphicsInputLayoutDesc _desc;
    };

} // namespace el {

#endif // __GL_INPUT_LAYOUT_H__
