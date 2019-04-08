#pragma once

#include "graphics_buffer.h"
#include "gl.h"

namespace el {

class GLBuffer final : public GraphicsBuffer
{
public:

	GLBuffer();
	~GLBuffer();

	bool create(const GraphicsBufferDesc& desc) override;
	bool create(GLenum target, int32_t size, const int8_t* data, GLenum flag);
	void destroy();

	void bind() const;

private:

	GraphicsBufferDesc _desc;

	GLenum _target;
	GLenum _usage;
	GLuint _bufferID;
};

} // namespace el {
