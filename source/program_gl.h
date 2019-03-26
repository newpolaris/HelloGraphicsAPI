#pragma once

#include <stdint.h>

#include "gl.h"

namespace el {
namespace gl {

namespace shader {
	typedef GLuint Handle;
}

namespace program
{
#if 0
	struct Handle {
		Handle(uint32_t idx) : _index(idx) {}
		uint32_t _index;
	};
#else
	typedef GLuint Handle;
#endif

	const Handle kUninitialized = Handle(0);

	bool isInitialized(const Handle& h);
	Handle create(const shader::Handle& vertex, const shader::Handle& fragment);
	void destroy(Handle& handle);

#if 0
	inline bool operator==(const program::Handle& a, const program::Handle& b)
	{
		return a._index == b._index;
	}

	inline bool operator!=(const program::Handle& a, const program::Handle& b)
	{
		return !(a == b);
	}
#endif
} // namespace program {

} // namespace el {
} // namespace gl {
