#pragma once

#include "graphics_data.h"
#include "gl.h"
#include "gl_types.h"

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

    class GLVertexBuffer final
    {
    public:

        GLVertexBuffer();
        GLVertexBuffer(const GraphicsDataPtr& data, int32_t offset);

        const GLBufferPtr& getData() const;
        int32_t getOffset() const;

    private:

        GLBufferPtr _data;
        int32_t _offset;
    };


} // namespace el {
