#include "renderer.h"

#include <el_debug.h>
#include "metal_context.h"
#include "metal_driver.h"

#include <Metal/mtl_types.h>

_EL_NAME_BEGIN

Renderer::Renderer()
{
    _surfaceFormat = el::GraphicsPixelFormatInvalid;
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

void Renderer::makeCurrent(SwapchainHandle handle)
{
    _handle = handle;
}

void Renderer::draw()
{
    _driver->beginFrame();

    ns::Error error;

    auto drawable = el::aquireCurrentDrawable(_handle);
    auto swapchainImage = el::getDrawableTexture(drawable);
    
    if (!_color ||
        swapchainImage.GetWidth() != _color.GetWidth() ||
        swapchainImage.GetHeight() != _color.GetHeight())
    {
        mtlpp::TextureUsage textureUsage = (mtlpp::TextureUsage)(mtlpp::TextureUsage::ShaderRead | mtlpp::TextureUsage::RenderTarget);
        auto colorTextureDesc = mtlpp::TextureDescriptor();
        colorTextureDesc.SetPixelFormat(el::asPixelFormat(_colorFormat));
        colorTextureDesc.SetMipmapLevelCount(1);
        colorTextureDesc.SetUsage(textureUsage);
        colorTextureDesc.SetStorageMode(mtlpp::StorageMode::Private);
        colorTextureDesc.SetWidth(swapchainImage.GetWidth());
        colorTextureDesc.SetHeight(swapchainImage.GetHeight());
        _color = _device.NewTexture(colorTextureDesc);
        
        auto depthTextureDesc = mtlpp::TextureDescriptor();
        depthTextureDesc.SetPixelFormat(el::asPixelFormat(_depthFormat));
        depthTextureDesc.SetMipmapLevelCount(1);
        depthTextureDesc.SetUsage(mtlpp::TextureUsage::RenderTarget);
        depthTextureDesc.SetStorageMode(mtlpp::StorageMode::Private);
        depthTextureDesc.SetWidth(swapchainImage.GetWidth());
        depthTextureDesc.SetHeight(swapchainImage.GetHeight());
        _depth = _device.NewTexture(depthTextureDesc);
    }
    
    auto commandBuffer = _driver->getCurrentCommandBuffer();
    
    _driver->beginRenderPass();

    mtlpp::RenderPassDescriptor renderPassDesc;
    {
        auto colorAttachment = renderPassDesc.GetColorAttachments()[0];
        colorAttachment.SetClearColor(mtlpp::ClearColor(0.2f, 0.5f, 0.7f, 1.0f));
        colorAttachment.SetLoadAction(mtlpp::LoadAction::Clear);
        colorAttachment.SetStoreAction(mtlpp::StoreAction::Store);
        colorAttachment.SetTexture(_color);
        colorAttachment.SetLevel(0);
        colorAttachment.SetSlice(0);
        renderPassDesc.SetRenderTargetArrayLength(1);
        
        auto depthAttachment = renderPassDesc.GetDepthAttachment();
        depthAttachment.SetClearDepth(1.0);
        depthAttachment.SetTexture(_depth);
        depthAttachment.SetLoadAction(mtlpp::LoadAction::Clear);
        depthAttachment.SetStoreAction(mtlpp::StoreAction::DontCare);
    }
    
    auto renderEncoder = commandBuffer.RenderCommandEncoder(renderPassDesc);
    renderEncoder.SetLabel("renderEncoder");
    // renderEncoder.SetVertexData(const void *bytes, uint32_t length, uint32_t index);
    // renderEncoder.SetRenderPipelineState(renderPipelineState);
    // renderEncoder.DrawIndexed((mtlpp::PrimitiveType::Triangle, uint32_t indexCount, IndexType indexType, const Buffer &indexBuffer, uint32_t indexBufferOffset);
    renderEncoder.EndEncoding();

    _driver->endRenderPass();
    _driver->beginRenderPass();

    mtlpp::RenderPassDescriptor blitPassDesc;
    {
        auto colorAttachment = blitPassDesc.GetColorAttachments()[0];
        colorAttachment.SetLoadAction(mtlpp::LoadAction::DontCare);
        colorAttachment.SetStoreAction(mtlpp::StoreAction::DontCare);
        colorAttachment.SetTexture(swapchainImage);
        colorAttachment.SetLevel(0);
        colorAttachment.SetSlice(0);
        blitPassDesc.SetRenderTargetArrayLength(1);
    }

    auto surfaceFormat = el::getSwapchainPixelFormat(_handle);
    if (_surfaceFormat != surfaceFormat)
    {
        mtlpp::RenderPipelineDescriptor blitPipelineDesc;
        blitPipelineDesc.SetLabel("blitPipeline");
        blitPipelineDesc.SetVertexFunction(_blitVertexFunction);
        blitPipelineDesc.SetFragmentFunction(_blitFragmentFunction);
        {
            auto colorAttachment = blitPipelineDesc.GetColorAttachments()[0];
            colorAttachment.SetPixelFormat(el::asPixelFormat(surfaceFormat));
        }
        _blitPipelineState = _device.NewRenderPipelineState(blitPipelineDesc, &error);
        _surfaceFormat = surfaceFormat;
    }

    auto blitEncoder = commandBuffer.RenderCommandEncoder(blitPassDesc);
    blitEncoder.SetLabel("blitEncoder");
    blitEncoder.SetRenderPipelineState(_blitPipelineState);
    blitEncoder.SetFragmentTexture(_color, 0);
    blitEncoder.Draw(mtlpp::PrimitiveType::Triangle, 0, 3);
    blitEncoder.EndEncoding();
    _driver->endRenderPass();

    commandBuffer.Present(drawable);
    // _driver->present();
    _driver->commit();
    _driver->endFrame();
}

_EL_NAME_END
