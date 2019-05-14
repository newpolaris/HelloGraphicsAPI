#include "gl_input_layout.h"

using namespace el;

GLInputLayout::GLInputLayout()
{
}

GLInputLayout::~GLInputLayout()
{
}

bool GLInputLayout::create(GraphicsInputLayoutDesc desc)
{
    _desc = std::move(desc);

    return true;
}

void GLInputLayout::destroy()
{
}

const GraphicsInputLayoutDesc& GLInputLayout::getDesc() const
{
    return _desc;
}
