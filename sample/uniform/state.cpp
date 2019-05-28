#include <iostream>
#include <map>

struct DepthState
{
};

struct SlideContext
{
    DepthState depth;
    CullMode cull;
    Shader vertexShader;
    Shader framgentShader;
};

// per-operation tracking
class StateTracker
{
    void invalidate() { isInitalized = false; }
    bool check(const State& candidate) const {
        if (isInitalized) return true;
        return state != candidate;
    }
    bool update(const State& candidate) {
        state = candidate;
        isDirty = true;
        isInitalized = true;
    }
    void upload() {

        glEnable(GL_DEPTH_TEST);
        glDisable(GL_DEPTH_TEST);

        isDirty = false;
    }

    bool isDirty;
    bool isInitalized;
    State state;
};

//  context[0] = device->createContext();
//  context[0]->setDepthTestEnable(false);
//  context[0]->setCullFaceEnable(false);
//  context[0]->setProgram(program); 
//  context[0]->setTexture("texture", texture);
//  context[0]->setVertexBuffer("vPos", vertex_buffer, sizeof(vertices[0]), 0);
//  context[0]->setIndexBuffer(index_buffer);

int main()
{
    SlideContext* m_Context;
    m_Context.depth.compare = LESS;
    m_Context.depth.enable = false;
    m_Context.unifrom.distance = 0.5;

    // Howto ?
    DrawArgs args = { vertices/index/ }
    DrawIndexed(driver, *m_Context, args);

    m_Context.depth.enable = false;

    return 0;
}