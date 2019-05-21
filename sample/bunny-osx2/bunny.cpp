#include <cmath>
#include <SDL.h>
#include <el_debug.h>
#include <el_utility.h>
#include <el_math.h>
#include <graphics_data.h>
#include <graphics_texture.h>

#include <graphics_platform.h>
#include <graphics_driver.h>
#include <native_window_helper.h>

#include "mesh.h"

namespace el {

    float radians(float degrees)
    {
        const float pi = std::acos(-1.f);
        return degrees * pi / 180.f;
    }

    const char vertexShaderSrc[] = R"""(
#include <metal_stdlib>
#pragma clang diagnostic ignored "-Wparentheses-equality"
using namespace metal;
struct xlatMtlShaderInput {
  float3 vPosition [[attribute(0)]];
  float3 vNormal [[attribute(1)]];
};
struct xlatMtlShaderOutput {
  float4 gl_Position [[position]];
  float3 color;
};
struct xlatMtlShaderUniform {
  float uScale;
  float3 uTranslate;
  float4 uOrientation;
  float4x4 uProject;
};
vertex xlatMtlShaderOutput xlatMtlMain (xlatMtlShaderInput _mtl_i [[stage_in]], constant xlatMtlShaderUniform& _mtl_u [[buffer(0)]])
{
  xlatMtlShaderOutput _mtl_o;
  _mtl_o.color = ((_mtl_i.vNormal * 0.5) + float3(0.5, 0.5, 0.5));
  float3 b_1 = 0;
  b_1 = (((_mtl_u.uOrientation.yzx * _mtl_i.vPosition.zxy) - (_mtl_u.uOrientation.zxy * _mtl_i.vPosition.yzx)) + (_mtl_u.uOrientation.w * _mtl_i.vPosition));
  float4 tmpvar_2 = 0;
  tmpvar_2.w = 1.0;
  tmpvar_2.xyz = (((_mtl_i.vPosition + 
    (2.0 * ((_mtl_u.uOrientation.yzx * b_1.zxy) - (_mtl_u.uOrientation.zxy * b_1.yzx)))
  ) * _mtl_u.uScale) + _mtl_u.uTranslate);
  _mtl_o.gl_Position = (_mtl_u.uProject * tmpvar_2);
  return _mtl_o;
}
    )""";
    
    const char fragmentShaderSrc[] = R"""(
#include <metal_stdlib>
using namespace metal;

struct xlatMtlShaderOutput {
    float4 gl_Position [[position]];
    float3 color;
};
    
fragment half4 main0(
   RasterizerData in [[stage_in]]
{
    return half4(in.color, 1.0);
}
    )""";
}

int main()
{
    const std::string objfiles[] = {
		"kitten.obj",
		"rabbit.obj",
		"wolf.obj",
    };
    Geometry geometry;
    for (uint32_t i = 0; i < el::countof(objfiles); i++)
        EL_ASSERT(LoadMesh(&geometry, el::getResourcePath() + objfiles[i]));
    
    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window* window = SDL_CreateWindow("triangle-sample",
            SDL_WINDOWPOS_UNDEFINED,
            SDL_WINDOWPOS_UNDEFINED,
            1280, 768,
            SDL_WINDOW_RESIZABLE);

    EL_ASSERT(window != nullptr);

    void *view = ::getNativeWindow(window);
    void *nativeSurface = setupMetalLayer(view);
    
    auto platform = el::DefaultPlatform::create(el::GraphicsDeviceTypeMetal);
    auto driver = platform->createDriver(nullptr);

    driver->setup(nativeSurface);
    
    el::RenderPassParms params {};
    params.flags.clear = el::GraphicsTargetBufferFlagBitColor;
    params.clearColor = el::math::float4(0.2f, 0.4f, 0.6f, 1.0f);

    auto vertexBuffer = driver->createVertexBuffer(geometry.getVertexData(), sizeof(Vertex) * geometry.getVertexCount());
    auto indexBuffer = driver->createIndexBuffer(geometry.getIndexData(), sizeof(uint32_t), geometry.getIndexCount());

#if 0
    GraphicsDataDesc vertices_buffer_desc;
    vertices_buffer_desc.setDataType(GraphicsDataTypeStorageVertexBuffer);
    vertices_buffer_desc.setStream(geometry.getVertexData());
    vertices_buffer_desc.setElementSize(sizeof(Vertex));
    vertices_buffer_desc.setNumElements(geometry.getVertexCount());

    vertex_buffer = device->createBuffer(vertices_buffer_desc);

    GraphicsDataDesc indices_buffer_desc;
    indices_buffer_desc.setDataType(GraphicsDataTypeStorageIndexBuffer);
    indices_buffer_desc.setStream(geometry.getIndexData());
    indices_buffer_desc.setElementSize(sizeof(uint32_t));
    indices_buffer_desc.setNumElements(geometry.getIndexCount());

    index_buffer = device->createBuffer(indices_buffer_desc);

    GraphicsInputLayoutDesc input_layout_desc;
    input_layout_desc.setAttributes(Vertex::getAttributeDescription());
    input_layout_desc.setBindings(Vertex::getBindingDescription());
    input_layout = device->createInputLayout(input_layout_desc);
#endif

    auto program = driver->createProgram(el::vertexShaderSrc, el::fragmentShaderSrc);
    auto defaultTarget = driver->createDefaultRenderTarget();
    
    el::PipelineState pipelineState {
        program,
    };
    
    struct DrawCommands
    {
        uint32_t vertexCount;
        uint32_t vertexOffset;
    };
    
    while (true)
    {
        bool isQuit = false;
        SDL_Event e;
        while (SDL_PollEvent(&e))
            isQuit = (e.type == SDL_QUIT);
        if (isQuit) break;

#if __has_feature(objc_arc)
        @autoreleasepool {
#endif
            driver->beginFrame();
            driver->beginRenderPass(defaultTarget, params);
            driver->setVertexBuffer(vertexBuffer, 0);
            driver->setFragmentTexture(texture, 0);
            driver->setPipelineState(pipelineState);
            driver->draw(el::GraphicsPrimitiveTypeTriangle, 3, 0);
            driver->endRenderPass();
            driver->commit();
            
#if __has_feature(objc_arc)
        }
#endif
    }
    program = nullptr;
    vertexBuffer = nullptr;
    texture = nullptr;

    driver->cleanup();
    delete driver;
    driver = nullptr;
    
    el::DefaultPlatform::destroy(&platform);
    
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
