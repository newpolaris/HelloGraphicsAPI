#ifndef __EL_IMAGE_H__
#define __EL_IMAGE_H__

#include <graphics_types.h>

namespace el {

    typedef std::shared_ptr<class ImageData> ImageDataPtr;

    class ImageData
    {
    public:

        static ImageDataPtr load(const std::string& filename);

        uint32_t width;
        uint32_t height;
        uint32_t depth;
        std::vector<stream_t> stream;
        GraphicsPixelFormat format;
    };
}

#endif // __EL_IMAGE_H__
