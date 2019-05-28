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

#include <Metal/Metal.h>
#include <metal/metal_driver.h>
#include <metal/metal_context.h>
#include <linmath.h>
#include <metal/metal_resources.h>

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
        packed_float3 uTranslate;
        packed_float4 uOrientation;
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
    
    vertex main0_out main0(main0_in input [[stage_in]],
                           constant transforms* _56 [[buffer(0)]],
                           constant float4x4& uProject [[buffer(1)]],
                           ushort iid [[instance_id]])
    {
        main0_out output = {};
        output.color = (input.vNormal * 0.5) + float3(0.5);
        constant transforms& trans = _56[0];
        float4 param = trans.uOrientation;
        float3 param_1 = input.vPosition;
        output.gl_Position = uProject * float4((rotate_position(param, param_1) * trans.uScale) + trans.uTranslate, 1.0);
        return output;
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
    
// __declspec(align(16))
    
#if EL_PLAT_IOS
    struct alignas(16)
#else
    struct alignas(256)
#endif
    transforms
    {
        float uScale;
        math::float3 uTranslate;
        math::float4 uOrientation;
    };

} // namespace el {

// rendererUpdateUniforms(
//        this,
//        _render->m_uniformBuffer[draw.m_uniformIdx],
//        draw.m_uniformBegin,
//        draw.m_uniformEnd);

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

    auto vertexBuffer = driver->createVertexBuffer(geometry.getVertexData(), geometry.getVertexCount(), sizeof(el::Vertex));
    auto indexBuffer = driver->createIndexBuffer(geometry.getIndexData(), geometry.getIndexCount(), sizeof(uint32_t));

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

        draws[i].translate[0] = urd(eng) * 20.0f - 10.f;
        draws[i].translate[1] = urd(eng) * 20.0f - 10.f;
        draws[i].translate[2] = urd(eng) * -20.0f - fNear;
        draws[i].scale = urd(eng) + 0.5f;
        quat orient;
        quat_rotate(orient, angle, axis);
        draws[i].orientation = el::math::float4(orient[0], orient[1], orient[2], orient[3]);
        draws[i].meshIndex = static_cast<uint32_t>(urd(eng) * geometry.meshes.size());
    }

    auto program = driver->createProgram(el::vertexShaderSrc, el::fragmentShaderSrc);
    auto defaultTarget = driver->createDefaultRenderTarget();

    el::PipelineState pipelineState;
    pipelineState.program = program;
    pipelineState.inputLayout = layoutDesc;
    
    // column major;
    mat4x4 adjust;
    mat4x4_identity(adjust);
    adjust[2][2] = 0.5f;
    adjust[3][2] = 0.5f;

    id<MTLDevice> device = MTLCreateSystemDefaultDevice();
    id<MTLBuffer> uniform = [device newBufferWithLength:sizeof(el::transforms)*draws.size()
                                                options:MTLResourceStorageModeShared];
    
    el::transforms* pointer = (el::transforms*)uniform.contents;
    for (auto& draw : draws)
    {
        pointer->uScale = draw.scale;
        pointer->uTranslate = draw.translate;
        pointer->uOrientation = draw.orientation;
        pointer += 1;
    }
    // [uniform didModifyRange:NSRang]
    
    using namespace el;
    el::MetalDriver* metal = (el::MetalDriver*)driver;
    el::MetalContext* context = metal->_context;
    
#if 1
    MTLIndirectCommandBufferDescriptor *indirectDesc = [[MTLIndirectCommandBufferDescriptor new] autorelease];
    indirectDesc.commandTypes = MTLIndirectCommandTypeDrawIndexed;
    indirectDesc.maxVertexBufferBindCount = 0;
    indirectDesc.maxFragmentBufferBindCount = 0;
    id<MTLIndirectCommandBuffer> _indirectCommandBuffer = [device newIndirectCommandBufferWithDescriptor:indirectDesc
                                                                                         maxCommandCount:3000
                                                                                                 options:0];
    
    //  Encode a draw command for each object drawn in the indirect command buffer.
    for (int objIndex = 0; objIndex < draws.size(); objIndex++)
    {
         const auto& mesh = geometry.meshes[draws[objIndex].meshIndex];
        
        id<MTLIndirectRenderCommand> ICBCommand = [_indirectCommandBuffer indirectRenderCommandAtIndex:objIndex];
        
        [ICBCommand drawIndexedPrimitives:MTLPrimitiveTypeTriangle
                               indexCount:mesh.indexCount
                                indexType:MTLIndexTypeUInt32
                              indexBuffer:indexBuffer->buffer
                        indexBufferOffset:mesh.indexOffset * indexBuffer->getDesc().getElementSize()
                            instanceCount:1
                               baseVertex:mesh.vertexOffset
                             baseInstance:objIndex];
    }
#endif
#if 1
    id<MTLBuffer> indirect = [device newBufferWithLength:sizeof(MTLDrawIndexedPrimitivesIndirectArguments)*draws.size()
                                                 options:MTLResourceStorageModeShared];
    auto arguments = (MTLDrawIndexedPrimitivesIndirectArguments*)indirect.contents;
    for (auto i = 0; i < draws.size(); i++)
    {
        const auto& mesh = geometry.meshes[draws[i].meshIndex];
        
        arguments->indexCount = mesh.indexCount;
        arguments->instanceCount = 1;
        arguments->indexStart = mesh.indexOffset;
        arguments->baseVertex = mesh.vertexOffset;
        arguments->baseInstance = i;
        arguments++;
    }
#endif
    
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
        mat4x4 project, uProject;
        mat4x4_perspective(project, el::radians(70.f), aspect, fNear, fFar);
        mat4x4_mul(uProject, adjust, project);
        
        // context[i]->setViewport(Viewport(0, 0, width, height));

        
#if __has_feature(objc_arc)
        @autoreleasepool {
#endif
            driver->beginFrame();
            driver->beginRenderPass(defaultTarget, params);
            driver->setPipelineState(pipelineState);
            [context->currentRenderEncoder setVertexBytes:uProject length:sizeof(uProject) atIndex:1];
            [context->currentRenderEncoder setVertexBuffer:uniform offset:0 atIndex:0];
            driver->setVertexBuffer(vertexBuffer, 0, 0);
            
            [context->currentRenderEncoder setCullMode:MTLCullModeFront];
            
#if 0
            [context->currentRenderEncoder useResource:vertexBuffer->buffer usage:MTLResourceUsageRead];
            [context->currentRenderEncoder useResource:indexBuffer->buffer usage:MTLResourceUsageRead];
            [context->currentRenderEncoder useResource:uniform usage:MTLResourceUsageRead];
            
            [context->currentRenderEncoder executeCommandsInBuffer:_indirectCommandBuffer
                                                         withRange:NSMakeRange(0, draws.size())];
#else
            for (uint32_t i = 0; i < draws.size(); i++)
            {
                const auto& mesh = geometry.meshes[draws[i].meshIndex];
                [context->currentRenderEncoder setVertexBufferOffset:i*sizeof(transforms) atIndex:1];

    #if 1
                [context->currentRenderEncoder drawIndexedPrimitives:MTLPrimitiveTypeTriangle
                                                          indexCount:mesh.indexCount
                                                           indexType:MTLIndexTypeUInt32
                                                         indexBuffer:indexBuffer->buffer
                                                   indexBufferOffset:mesh.indexOffset * indexBuffer->getDesc().getElementSize()
                                                       instanceCount:1
                                                          baseVertex:mesh.vertexOffset
                                                        baseInstance:i];
    #endif
    #if 0
                driver->draw(el::GraphicsPrimitiveTypeTriangle,
                                    indexBuffer,
                                    mesh.indexCount,
                                    mesh.indexOffset);
    #endif
            }
#endif
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
