#include <OpenGL/gl.h>

class StationBreak
{
public:

    void initStation();
    void setTexture();
    void nextStation();
    void GL_Init();
};

// Two image type exist
// up side down, RGBA
// down side up, BGRA
void StationBreak::initStation()
{
    glViewport();

    m_vertex = shader::create(GL_VERTEX_SHADER, glslvertex);
    m_fragment = shader::create(GL_FRAGMENT_SHADER, glslfragment);

    m_program = program::create(m_vertex, m_fragment);

    m_aPositionHandle = glGetAttribLocation(m_program, "a_Position");
    m_aTexHandle = glGetAttribLocation(m_program, "a_texCoord");

    m_uMVPMatrixHandle = glGetUniformLocation(m_program, "u_MVPMatrix");
    m_2DScale = glGetUniformLocation(m_program, "u_2DScale");

    glUseProgram(m_program);
    glUniform1f(m_fWidthLocation, m_fWidth);
    glUniform1f(m_fHeightLocation, m_fHeight);

    if (m_nTex1 == -1)
    {
        glGenTextures(1, &m_nTex1);
        glGenTextures(1, &m_nTex2);
        glGenTextures(1, &m_nFrmTex1);
        glGenTextures(1, &m_nFrmTex2);
    }
#ifdef IPHONE
    if (m_frameBuffer == -1)
    {
        glGenFramebuffers(1, &m_frameBuffer);
        glGenRenderbuffers(1, &m_depthBuffer);
        glGenRenderbuffers(1, &m_renderBuffer);
    }

    glBindFramebuffer(GL_FRAMEBUFFER, m_frameBuffer);

    // TODO: aquire size
    // glBindRenderbuffer(GL_RENDERBUFFER, m_depthBuffer);
    // glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, m_lpti->biWidth, m_lpti->biHeight);
    // glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_depthBuffer);

    glBindRenderbuffer(GL_RENDERBUFFER, m_renderBuffer);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, m_renderBuffer);
#endif

    glClearColor();
    setTexture();
    nextStation();
}

void StationBreak::setCurrentBackTexture()
{
    glBindTexture(GL_TEXTURE_2D, m_nTex1);	// Current page
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    gl:TexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
}

void StationBreak::setTexture()
{
#ifdef IPHONE
    glBindFramebuffer(GL_FRAMEBUFFER, m_frameBuffer);
#endif

    setCurrentBackTexture();

    glBindTexture(GL_TEXTURE_2D, m_nTex2); // Next page
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, nWidth, nHeight, 0, GL_RGBA, GL_TEXTURE_DATATYPE_A, data);

	// 2nd image
    glBindTexture(GL_TEXTURE_2D, m_nFrmTex1);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    gl::TexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, nWidth, nHeight, 0, GL_RGBA, GL_TEXTURE_DATATYPE_A, data);

    glBindTexture(GL_TEXTURE_2D, m_nFrmTex2);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, nWidth, nHeight, 0, GL_RGBA, GL_TEXTURE_DATATYPE_A, data);
}

void StationBreak::GL_Init()
{
    glUniform3f(m_IncColorLocation, 0.f, 0.f, 0.f);
    glUniform1f(m_MulColorLocation, 1.f);
    glUniform1f(m_AlphaLocation, 1.f);
    glUniform1f(m_LightRatioLocation, 0.f);

    glClear(GL_COLOR_BUFFER_BIT);
}

void StationBreak::nextStation()
{
    glUseProgram(m_program);

    GL_Init();

    glViewport();

    glEnableVertexAttribArray(m_aPositionHandle);
    glEnableVertexAttribArray(m_aTexHandle);

    // Auto Transition N's Waiting and setting for next page play
    if (m_isWaiting && !m_isAnimating)
        return AutoTransition(pEvent);

    // setting GL for Animation Rendering
    if (m_isAnimating)
        return nextFrameStation();

    // setting GL for Transition Rendering 
    setGLTransition(bRenderingEnd);

}

void StationBreak::AutoTransition()
{
    setCurrentBackTexture();
    glBindTexture(GL_TEXTURE_2D, m_nTex1);
    DrawPage();
    gl::DisableVertexAttribArray(m_aPositionHandle);
    gl::DisableVertexAttribArray(m_aTexHandle)
}

void StationBreak::DrawPage()
{
    float squareCoords[] = {
        0.f, m_fHeight, 0.f,				// Position 0
        0.f, 1.f,							// TexCoord 0
        0.f, 0.f, 0.f,						// Position 1
        0.f, 0.f,							// TexCoord 1 
        m_fWidth, 0.f, 0.f,					// Position 2
        1.f, 0.f,							// TexCoord 2
        m_fWidth, m_fHeight, 0.f,			// Position 3
        1.f, 1.f							// TexCoord 3
    };
    SubDrawTexture(squareCoords);
}

void StationBreak::SubDrawTexture(float* vtxs)
{
    // Load the coordinate
    glVertexAttribPointer(m_aPositionHandle, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(BrFLOAT), vtxs);
    glVertexAttribPointer(m_aTexHandle, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(BrFLOAT), &vtxs[3]);

    // Drawing
    if (bTriangleFan) {
        gl::DrawArrays(GL_TRIANGLE_FAN, 0, 4);
    } else {
        std::array<uint16_t, 6> indices = { 0, 1, 2, 0, 2, 3 };
        gl::DrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_SHORT, indices.data());
    }
}

void StationBreak::setGLTransition()
{
    m_fPortion;
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

#ifdef IPHONE
    gl::BindFramebuffer(GL_FRAMEBUFFER, m_frameBuffer);
#endif	

    Tran_FADE();
}

void StationBreak::Tran_FADE()
{
    glUniform1f(m_AlphaLocation, delta1);
    glBindTexture(GL_TEXTURE_2D, m_nTex1);
    DrawPage();
    glUniform1f(m_AlphaLocation, delta2);
    glBindTexture(GL_TEXTURE_2D, m_nTex2);
    DrawPage(BrFALSE);
}
