#pragma once

#include "graphics_buffer.h"
#include "gl.h"

namespace el {

    class GLBuffer final : public GraphicsBuffer
    {
    public:

        GLBuffer();
        ~GLBuffer();

        bool create(GraphicsBufferDesc desc) override;
        void destroy();

        void bind() const;

    private:

        GraphicsBufferDesc _desc;

        GLenum _target;
        GLenum _usage;
        GLuint _bufferID;
        size_t _bufferSize;
    };

} // namespace el {
