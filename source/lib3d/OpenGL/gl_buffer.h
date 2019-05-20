#ifndef __GL_BUFFER_H__
#define __GL_BUFFER_H__

#include <graphics_data.h>
#include <graphics_types.h>
#include "gl_types.h"
#include "gl_headers.h"

namespace el {

    class GLBuffer final : public GraphicsData
    {
    public:

        GLBuffer();
        ~GLBuffer();

        bool create(GraphicsDataDesc desc);
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

#endif // __GL_BUFFER_H__
