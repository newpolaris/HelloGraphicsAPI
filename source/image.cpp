#include <image.h>
#include <stb/stb_image.h>

using namespace el;

ImageDataPtr ImageData::load(const std::string& filename)
{
    stbi_set_flip_vertically_on_load(true);

    int width = 0, height = 0, components = 0;
    auto imagedata = (stream_t*)stbi_load(filename.c_str(), &width, &height, &components, 0);
    if (!imagedata) return nullptr;

    // 1-byte aligment image
    const streamsize_t length = width * height * components;

    GraphicsPixelFormat format = GraphicsPixelFormat::GraphicsPixelFormatInvalid;
    switch (components)
    {
    case 1: format = GraphicsPixelFormat::GraphicsPixelFormatR8Unorm; break;
    case 2: format = GraphicsPixelFormat::GraphicsPixelFormatRG8Unorm; break;
    case 3: format = GraphicsPixelFormat::GraphicsPixelFormatRGB8Unorm; break;
    case 4: format = GraphicsPixelFormat::GraphicsPixelFormatRGBA8Unorm; break;
    }

    auto container = std::make_shared<ImageData>();
    container->format = format;
    container->stream = std::vector<stream_t>(imagedata, imagedata + length);
    container->width = (int32_t)width;
    container->height = (int32_t)height;

    stbi_image_free(imagedata);

    return container;
}
