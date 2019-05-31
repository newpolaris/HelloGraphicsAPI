#ifndef SlideContext_h
#define SlideContext_h

#include "el_math.h"

struct Driver;

struct SlideVertexUniform
{
    // el::math::float4x4 u_MVPMatrix;
    float u_fWidth;
    float u_fHeight;
    float u_cameradist;
};

struct SlideFragmentUniform
{
    float ratio;
    el::math::float3 u_darkHSL;
    el::math::float3 u_lightHSL;
    el::math::float3 u_desatHSL;
    el::math::float3 u_compHSL;
    el::math::float3 u_comp2HSL;
    el::math::float3 u_contHSL;
    
    el::math::float3 u_inccolour;
    
    float u_mulcolour;
    float u_alpha;
    //Lightness
    float u_lightratio;
    // Black&White Color
    float u_bwratio;
    // Complementary Color
    float u_cmratio;
    // Contrasting Color
    float u_cnratio;
};

enum Texture { kTex = 0, };

struct BlendState
{
    bool isBlend;
};

struct SlideContext
{
    void setUp();
    void cleanUp();
    
    bool isBackendCompatible() const;
    
    void createDriver();
    void createShader();
    void createTexture(uint32_t index, uint32_t width, uint32_t height, void* data);
    void bindShaders();
    void bindTexture(uint8_t slot, uint32_t index);
    void clearScreen();
    void draw();
    
    // render state
    bool isClearColor;
    BlendState blend;
    
    el::math::float4 clearColor;
    el::math::float4 viewport;
    SlideVertexUniform vUniform;
    SlideFragmentUniform fUniform;
    int backend;
    
    Driver* _driver;
};

#endif /* SlideContext_h */
