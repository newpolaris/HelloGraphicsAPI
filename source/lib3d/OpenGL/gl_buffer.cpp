#include <OpenGL/gl_buffer.h>
#include <el_debug.h>
#include <OpenGL/gl_types.h>

using namespace el;

GLBuffer::GLBuffer() :
    _target(0),
    _usage(0),
    _bufferID(0)
{
}

GLBuffer::~GLBuffer()
{
}

bool GLBuffer::create(GraphicsDataDesc desc)
{
    auto type = desc.getDataType();
    if (type == GraphicsDataTypeStorageVertexBuffer)
        _target = GL_ARRAY_BUFFER;
    else if (type == GraphicsDataTypeStorageIndexBuffer)
        _target = GL_ELEMENT_ARRAY_BUFFER;
    else {
        EL_TRACE("Unsupported data type");
        debug_break();
        return false;
    }

    _usage = GL_STATIC_DRAW;

    auto usage = desc.getUsage();
    if (usage & GraphicsUsageFlagDynamicStorageBit)
    {
        _usage = GL_DYNAMIC_DRAW;
        if (usage & GraphicsUsageFlagReadBit)
            _usage = GL_DYNAMIC_READ;
    } 

    const streamsize_t dataSize = desc.getStreamSize();

    GL_CHECK(glGenBuffers(1, &_bufferID));
    GL_CHECK(glBindBuffer(_target, _bufferID));
    GL_CHECK(glBufferData(_target, dataSize, desc.getStream(), _usage));

    if (GLAD_GL_KHR_debug) // GLEW_KHR_debug
        GL_CHECK(glObjectLabel(GL_BUFFER, _bufferID, -1, "Vertex Array Buffer object"));

    _desc = std::move(desc);

    return true;
}

void GLBuffer::destroy()
{
    GL_CHECK(glDeleteBuffers(1, &_bufferID));
    _bufferID = 0;
    _target = 0;
}

void GLBuffer::bind() const
{
    GL_CHECK(glBindBuffer(_target, _bufferID));
}

const GraphicsDataDesc& GLBuffer::getDesc() const
{
    return _desc;
}

GLVertexBuffer::GLVertexBuffer() :
    _offset(0)
{
}

GLVertexBuffer::GLVertexBuffer(const GraphicsDataPtr& data, int32_t offset) :
    _offset(offset)
{
    _data = std::static_pointer_cast<GLBuffer>(data);
}

const GLBufferPtr& GLVertexBuffer::getData() const
{
    return _data;
}

int32_t GLVertexBuffer::getOffset() const
{
    return _offset;
}
