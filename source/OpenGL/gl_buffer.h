#pragma once

#include "graphics_data.h"
#include "gl.h"

namespace el {

    class GLBuffer final : public GraphicsData
    {
    public:

        GLBuffer();
        ~GLBuffer();

        bool create(GraphicsDataDesc desc) override;
        void destroy();

        void bind() const;

        const GraphicsDataDesc& getDesc() const override;

    private:

        GraphicsDataDesc _desc;

        GLenum _target;
        GLenum _usage;
        GLuint _bufferID;
    };

} // namespace el {
