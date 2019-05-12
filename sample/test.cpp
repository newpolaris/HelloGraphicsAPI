#include <glad/glad.h>

namespace gl
{
    void Enable(GLenum cap);
    void Disalbe(GLenum cap);

    void BlendFunc(GLenum func);

	void GenTextures(GLsizei n, GLuint *textures);
	void DeleteTextures(GLsizei n, const GLuint *textures);

    static GraphicsBlendStateDesc _blendDesc;
}

void gl::Enable(GLenum cap)
{
    switch (cap)
    {
    case GL_DEPTH_TEST:
        context->setDepthEnable(true);
        break;
    case GL_BLEND:
        _blendDesc.blend[0].setEnable(true);
        break;
    }
}

void gl::BlendFunc(GLenum func)
{
    switch (func)
    {
    case BLEND_SRC_OVER_ALPH:
        _blendDesc.blend[0].setBlendFunc(asBlendFuc(func));
        break;
    }
}

void gl::DrawElements(...)
{
}

void gl::BindProgram(...)
{
}

class RendererOpenGL
{
public:

    void glEnable();
    void glDisalbe();
};




class RendererMetal
{
public:

    void setDepth();
    void setDepthWriteEnable(true);
    void setDepthCompareOp(GraphicsCompare compare);

private:

};

int main()
{
}
