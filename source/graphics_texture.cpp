#include <stdint.h>
#include "platform.h"

#if EL_PLAT_OSX || EL_PLAT_IOS

#define GL_CHECK(_call) _call;

struct GraphicsDescriptor
{
	GraphicsDescriptor() 
	{
	}

	uint32_t width;
	uint32_t height;
	uint32_t depth;
	int8_t* data;
};

class GraphicsTexture
{
public:

	GraphicsTexture();
};

GraphicsTexture::GraphicsTexture()
{
}

#include <glad/glad.h>

class TexutreOpenGL : public GraphicsTexture
{
public:

	TexutreOpenGL();
	void Create();
	void Destory();

	const GraphicsDescriptor& getDescriptor();

private:

	GraphicsDescriptor _descriptor;
	GLuint _id;
};

TexutreOpenGL::TexutreOpenGL() :
	_id(0)
{
}

void TexutreOpenGL::Create()
{
	GL_CHECK(glGenTextures(1, &_id));
}

void TexutreOpenGL::Destory()
{
	GL_CHECK(glDeleteTextures(1, &_id));
	_id = 0;
}

#include <mtlpp.hpp>

class TextureMTL : public GraphicsTexture
{
public:

	TextureMTL();
	void Create();
	void Destory();

	mtlpp::Texture _texture;
	mtlpp::SamplerState _sampler;
};

TextureMTL::TextureMTL()
{
}

void TextureMTL::Create()
{
	mtlpp::Device device = mtlpp::Device::CreateSystemDefaultDevice();
	assert(device);

	mtlpp::TextureDescriptor textureDesc = mtlpp::TextureDescriptor::Texture2DDescriptor(
		mtlpp::PixelFormat::RGBA8Unorm, 400, 400, false);
	textureDesc.SetUsage(mtlpp::TextureUsage::ShaderRead);
	mtlpp::Texture _texture = device.NewTexture(textureDesc);
	assert(_texture);

	mtlpp::SamplerDescriptor samplerDesc;
	samplerDesc.SetMinFilter(mtlpp::SamplerMinMagFilter::Linear);
	_sampler = device.NewSamplerState(samplerDesc);

	// Test usage
#if 0
	mtlpp::CommandQueue queue = device.NewCommandQueue();
	mtlpp::CommandBuffer commandBuffer = queue.CommandBuffer();

	mtlpp::RenderPassDescriptor renderDescriptor;
	uint32_t length = renderDescriptor.GetColorAttachments().GetSize();
	if (length > 0)
	{
		mtlpp::RenderPassColorAttachmentDescriptor colorAttachmentDesc = renderDescriptor.GetColorAttachments()[0];
		colorAttachmentDesc.SetLoadAction(mtlpp::LoadAction::Clear);
		colorAttachmentDesc.SetStoreAction(mtlpp::StoreAction::Store);
	}
	mtlpp::RenderCommandEncoder encoder = commandBuffer.RenderCommandEncoder(renderDescriptor);
#endif
}


#if 0
	FMetalSampler State = Samplers.Find(Initializer);
	if (!State.GetPtr())
	{
		mtlpp::SamplerDescriptor Desc;
		switch(Initializer.Filter)
		{
			case SF_AnisotropicLinear:
			case SF_AnisotropicPoint:
				Desc.SetMinFilter(mtlpp::SamplerMinMagFilter::Linear);
				Desc.SetMagFilter(mtlpp::SamplerMinMagFilter::Linear);
				Desc.SetMipFilter(mtlpp::SamplerMipFilter::Linear);
				break;
			case SF_Trilinear:
				Desc.SetMinFilter(mtlpp::SamplerMinMagFilter::Linear);
				Desc.SetMagFilter(mtlpp::SamplerMinMagFilter::Linear);
				Desc.SetMipFilter(mtlpp::SamplerMipFilter::Linear);
				break;
			case SF_Bilinear:
				Desc.SetMinFilter(mtlpp::SamplerMinMagFilter::Linear);
				Desc.SetMagFilter(mtlpp::SamplerMinMagFilter::Linear);
				Desc.SetMipFilter(mtlpp::SamplerMipFilter::Nearest);
				break;
			case SF_Point:
				Desc.SetMinFilter(mtlpp::SamplerMinMagFilter::Nearest);
				Desc.SetMagFilter(mtlpp::SamplerMinMagFilter::Nearest);
				Desc.SetMipFilter(mtlpp::SamplerMipFilter::Nearest);
				break;
		}
		Desc.SetMaxAnisotropy(GetMetalMaxAnisotropy(Initializer.Filter, Initializer.MaxAnisotropy));
		Desc.SetSAddressMode(TranslateWrapMode(Initializer.AddressU));
		Desc.SetTAddressMode(TranslateWrapMode(Initializer.AddressV));
		Desc.SetRAddressMode(TranslateWrapMode(Initializer.AddressW));
		Desc.SetLodMinClamp(Initializer.MinMipLevel);
		Desc.SetLodMaxClamp(Initializer.MaxMipLevel);
		Desc.SetCompareFunction(TranslateSamplerCompareFunction(Initializer.SamplerComparisonFunction));
#if PLATFORM_MAC
		Desc.SetBorderColor(Initializer.BorderColor == 0 ? mtlpp::SamplerBorderColor::TransparentBlack : mtlpp::SamplerBorderColor::OpaqueWhite);
#endif
		
		State = Device.NewSamplerState(Desc);
		
		Samplers.Add(Initializer, State);
	}
	return State;
}
#endif

void TextureMTL::Destory()
{
}

#endif // #if EL_PLAT_OSX || EL_PLAT_IOS
