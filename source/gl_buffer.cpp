#include "gl_buffer.h"

using namespace el;

GLBuffer::GLBuffer() :
	_target(0),
	_bufferID(0)
{
}

GLBuffer::~GLBuffer()
{
}

bool GLBuffer::create(const GraphicsBufferDesc& desc)
{
	return create(GL_ARRAY_BUFFER, desc.getDataSize(), (int8_t*)desc.getData(), GL_STATIC_DRAW);
}

bool GLBuffer::create(GLenum target, int32_t size, const int8_t* data, GLenum flag)
{
	_target = target;

	GL_CHECK(glGenBuffers(1, &_bufferID));
	if (GLAD_GL_KHR_debug) // GLEW_KHR_debug
		glObjectLabel(GL_BUFFER, _bufferID, -1, "Vertex Array Buffer object");
	GL_CHECK(glBindBuffer(_target, _bufferID));
	GL_CHECK(glBufferData(_target, size, data, flag));

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
