#include <cmath>
#include <SDL.h>
#include <el_debug.h>
#include <el_utility.h>
#include <el_math.h>
#include <graphics_data.h>
#include <graphics_texture.h>
#include <graphics_platform.h>
#include <graphics_driver.h>
#include <graphics_input_layout.h>
#include <graphics_pipeline.h>
#include <native_window_helper.h>

#include "mesh.h"

namespace el {

    float radians(float degrees)
    {
        const float pi = std::acos(-1.f);
        return degrees * pi / 180.f;
    }

    const char vertexShaderSrc[] = R"""(
#pragma clang diagnostic ignored "-Wmissing-prototypes"
    
#include <metal_stdlib>
#include <simd/simd.h>
    
    using namespace metal;
    
    struct transforms
    {
        float uScale;
        float3 uTranslate;
        float4 uOrientation;
        float4x4 uProject;
    };
    
    struct main0_out
    {
        float3 color [[user(locn0)]];
        float4 gl_Position [[position]];
    };
    
    struct main0_in
    {
        float3 vPosition [[attribute(0)]];
        float3 vNormal [[attribute(1)]];
        float2 vTexcoord [[attribute(2)]];
    };
    
    float3 rotate_position(thread const float4& quat, thread const float3& v)
    {
        return v + (cross(quat.xyz, cross(quat.xyz, v) + (v * quat.w)) * 2.0);
    }
    
    vertex main0_out main0(main0_in in [[stage_in]], constant transforms& _56 [[buffer(0)]])
    {
        main0_out out = {};
        out.color = (in.vNormal * 0.5) + float3(0.5);
        float4 param = _56.uOrientation;
        float3 param_1 = in.vPosition;
        // out.gl_Position = _56.uProject * float4((rotate_position(param, param_1) * _56.uScale) + _56.uTranslate, 1.0);
        out.gl_Position = float4(param_1, 1.0);
        return out;
    }
)""";
    
    const char fragmentShaderSrc[] = R"""(
#include <metal_stdlib>
using namespace metal;

struct main0_out
{
    float3 color [[user(locn0)]];
    float4 gl_Position [[position]];
};
    
fragment half4 main0(
   main0_out in [[stage_in]])
{
    return half4(float4(in.color, 1.0));
}
)""";
    
} // namespace el {

int main()
{
    const std::string objfiles[] = {
		"kitten.obj",
		"rabbit.obj",
		"wolf.obj",
    };
    el::Geometry geometry;
    for (uint32_t i = 0; i < el::countof(objfiles); i++)
        EL_ASSERT(LoadMesh(&geometry, EL_DEFINE_RESOURCE_PATH + objfiles[i]));
    
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

    auto vertexBuffer = driver->createVertexBuffer(geometry.getVertexData(), sizeof(el::Vertex) * geometry.getVertexCount());
    auto indexBuffer = driver->createIndexBuffer(geometry.getIndexData(), geometry.getIndexCount()*sizeof(uint32_t), sizeof(uint32_t));

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
#endif

    el::GraphicsInputLayoutDesc layoutDesc;
    layoutDesc.setAttributes(el::Vertex::getAttributeDescription());
    layoutDesc.setBindings(el::Vertex::getBindingDescription());

    auto program = driver->createProgram(el::vertexShaderSrc, el::fragmentShaderSrc);
    auto defaultTarget = driver->createDefaultRenderTarget();

    el::PipelineState pipelineState;
    pipelineState.program = program;
    pipelineState.inputLayout = layoutDesc;
    
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
            driver->setPipelineState(pipelineState);

            const auto& mesh = geometry.meshes[0];

            driver->setVertexBuffer(vertexBuffer, 0, mesh.vertexOffset);
            driver->draw(el::GraphicsPrimitiveTypeTriangle, 
                         indexBuffer,
                         mesh.indexCount,
                         mesh.indexOffset);

            driver->endRenderPass();
            driver->commit();
            
#if __has_feature(objc_arc)
        }
#endif
    }
    program = nullptr;
    vertexBuffer = nullptr;

    driver->cleanup();
    delete driver;
    driver = nullptr;
    
    el::DefaultPlatform::destroy(&platform);
    
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
