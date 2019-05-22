#include <cmath>
#include <random>
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
    
    struct transforms
    {
        float uScale;
        math::float3 uTranslate;
        math::float4 uOrientation;
        mat4x4 uProject;
    };

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

    const float fNear = 0.1f;
    const float fFar = 1000.f;
    std::default_random_engine eng(10);
    std::uniform_real_distribution<float> urd(0, 1);
    const uint32_t draw_count = 2000;
    std::vector<el::MeshDraw> draws(draw_count);
    for (uint32_t i = 0; i < draw_count; i++) {
        vec3 axis;
        axis[0] = urd(eng)*2 - 1;
        axis[1] = urd(eng)*2 - 1;
        axis[2] = urd(eng)*2 - 1;

        const float angle = el::radians(urd(eng) * 90.f);

        draws[i].translate[0] = urd(eng) * 20.f - 10.f;
        draws[i].translate[1] = urd(eng) * 20.f - 10.f;
        draws[i].translate[2] = urd(eng) * -20.f - fNear;
        draws[i].scale = urd(eng) + 0.5f;
        quat_rotate(draws[i].orientation, angle, axis); 
        draws[i].meshIndex = static_cast<uint32_t>(urd(eng) * geometry.meshes.size());
    }

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
    
    mat4x4 adjust;
    mat4x4_identity(adjust);
    adjust[2][2] = 0.5;
    adjust[2][3] = 0.5;
    
    el::transforms transform;
    auto uniformTrans = driver->createUniform(sizeof(transform));
    
    while (true)
    {
        bool isQuit = false;
        SDL_Event e;
        while (SDL_PollEvent(&e))
            isQuit = (e.type == SDL_QUIT);
        if (isQuit) break;

        int w, h;
        SDL_GetWindowSize(window, &w, &h);

        const float aspect = static_cast<float>(w) / h;
        mat4x4 project;
        mat4x4_perspective(project, el::radians(70.f), aspect, fNear, fFar);
        mat4x4 adjusted;
        
        mat4x4_mul(transform.uProject, adjust, project);
        
        // context[i]->setUniform("uProject", project);
        // context[i]->setViewport(Viewport(0, 0, width, height));

#if __has_feature(objc_arc)
        @autoreleasepool {
#endif
            driver->beginFrame();
            driver->beginRenderPass(defaultTarget, params);
            driver->setPipelineState(pipelineState);

            const auto& mesh = geometry.meshes[0];

            // const auto& mesh = geometry.meshes[draw.meshIndex];
            // context[i]->setUniform("uScale", draw.scale);
            // context[i]->setUniform("uTranslate", draw.translate);
            // context[i]->setUniform("uOrientation", draw.orientation);
            driver->setUniform(0, uniformTrans);
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
