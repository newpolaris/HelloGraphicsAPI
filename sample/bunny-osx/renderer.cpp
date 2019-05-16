#include "renderer.h"

#include <el_debug.h>
#include "metal_driver.h"

#include <Metal/mtl_types.h>

_EL_NAME_BEGIN

Renderer::Renderer()
{
}

mtlpp::Device& Renderer::getDevice()
{
    return _device;;
}

void Renderer::setup()
{
    _driver = std::make_shared<MetalDriver>();
    _device = _driver->getDevice();
    
    setupBlitRenderer();
    setupMeshRenderer();
}

void Renderer::setupBlitRenderer()
{
    const char vertexShaderSrc[] = R"""(
#include <metal_stdlib>
using namespace metal;

typedef struct
{
    packed_float3 position;
    packed_float2 texcoord;
} vertex_t;

typedef struct
{
    float4 clipSpacePosition [[position]];
    float2 textureCoordinate;
} RasterizerData;

vertex RasterizerData main0(unsigned int vID[[vertex_id]])
{
    float x = -1.0 + float((vID & 2) << 1);
    float y = -1.0 + float((vID & 1) << 2);

    RasterizerData data;
    data.clipSpacePosition = float4(x, y, 0.0, 1.0);
    data.textureCoordinate = float2(x, y) * 0.5 + 0.5;
    return data;
}
    )""";
    
    const char fragmentShaderSrc[] = R"""(
#include <metal_stdlib>
using namespace metal;

typedef struct
{
    float4 clipSpacePosition [[position]];
    float2 textureCoordinate;
} RasterizerData;

fragment half4 main0(
                     RasterizerData in [[stage_in]],
                     texture2d<half> colorTexture [[texture(0)]])
{
    constexpr sampler textureSampler (mag_filter::nearest,
                                      min_filter::nearest);
    // Sample the texture to obtain a color
    const half4 colorSample = colorTexture.sample(textureSampler, in.textureCoordinate);
    
    // We return the color of the texture
    return colorSample;
}
    )""";

    ns::Error error;
    auto vertexLibrary = _device.NewLibrary(vertexShaderSrc, mtlpp::CompileOptions(), &error);
    if (!vertexLibrary)
        EL_TRACE("Failed to created pipeline state, error %s", error.GetLocalizedDescription().GetCStr());
    EL_ASSERT(vertexLibrary);
    auto fragmentLibrary = _device.NewLibrary(fragmentShaderSrc, mtlpp::CompileOptions(), &error);
    if (!fragmentLibrary)
        EL_TRACE("Failed to created pipeline state, error %s", error.GetLocalizedDescription().GetCStr());
    EL_ASSERT(fragmentLibrary);
    
    EL_ASSERT(vertexLibrary && fragmentLibrary);
    _blitVertexFunction = vertexLibrary.NewFunction("main0");
    EL_ASSERT(_blitVertexFunction);
    _blitFragmentFunction = fragmentLibrary.NewFunction("main0");
    EL_ASSERT(_blitFragmentFunction);
}

void Renderer::setupMeshRenderer()
{
}

void Renderer::makeCurrent(NativeSurface surface)
{
    _driver->makeCurrent(surface);
}

void Renderer::resizeTexture(uint32_t width, uint32_t height)
{
    mtlpp::TextureUsage textureUsage = (mtlpp::TextureUsage)(mtlpp::TextureUsage::ShaderRead | mtlpp::TextureUsage::RenderTarget);
    auto colorTextureDesc = mtlpp::TextureDescriptor();
    colorTextureDesc.SetPixelFormat(el::asPixelFormat(_colorFormat));
    colorTextureDesc.SetMipmapLevelCount(1);
    colorTextureDesc.SetUsage(textureUsage);
    colorTextureDesc.SetStorageMode(mtlpp::StorageMode::Private);
    colorTextureDesc.SetWidth(width);
    colorTextureDesc.SetHeight(height);
    _color = _device.NewTexture(colorTextureDesc);

    auto depthTextureDesc = mtlpp::TextureDescriptor();
    depthTextureDesc.SetPixelFormat(el::asPixelFormat(_depthFormat));
    depthTextureDesc.SetMipmapLevelCount(1);
    depthTextureDesc.SetUsage(mtlpp::TextureUsage::RenderTarget);
    depthTextureDesc.SetStorageMode(mtlpp::StorageMode::Private);
    depthTextureDesc.SetWidth(width);
    depthTextureDesc.SetHeight(height);
    _depth = _device.NewTexture(depthTextureDesc);
}

void Renderer::draw()
{
    _driver->beginFrame();

    auto renderTarget = _driver->createDefaultRenderTarget();
    auto swapchainImage = renderTarget.getColor();
    
    resizeTexture(swapchainImage.GetWidth(), swapchainImage.GetHeight());
    
    auto commandBuffer = _driver->getCurrentCommandBuffer();
    
    MetalRenderTarget geometryTarget(_color, _depth);
    PassDescriptor renderPassDesc(math::float4(0.2f, 0.5f, 0.7f, 1.0f), 1.0);
    
    _driver->beginRenderPass(geometryTarget, renderPassDesc, "geometryPass");
    // renderEncoder.SetVertexData(const void *bytes, uint32_t length, uint32_t index);
    // renderEncoder.SetRenderPipelineState(renderPipelineState);
    // renderEncoder.DrawIndexed((mtlpp::PrimitiveType::Triangle, uint32_t indexCount, IndexType indexType, const Buffer &indexBuffer, uint32_t indexBufferOffset);
    _driver->endRenderPass();
    
    PassDescriptor blitPassDesc;
    blitPassDesc.loadColor = mtlpp::LoadAction::DontCare;
    blitPassDesc.storeColor = mtlpp::StoreAction::DontCare;
    
    _driver->beginRenderPass(renderTarget, blitPassDesc, "blitPass");

    MetalRenderPipelineDesc blitPipelineDesc;
    blitPipelineDesc.label = "blitPipeline";
    blitPipelineDesc.vertexFunction = _blitVertexFunction;
    blitPipelineDesc.fragmentFunction = _blitFragmentFunction;
    
    _driver->setRenderPipelineDesc(blitPipelineDesc);
    _driver->setFragmentTexture(_color, 0);
    _driver->draw(GraphicsPrimitiveTypeTriangle, 3);
    _driver->endRenderPass();
    _driver->present();
    _driver->commit();
    _driver->endFrame();
}

_EL_NAME_END
