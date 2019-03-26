#pragma once

#include "gl.h"

namespace el {
namespace gl {

	namespace program
	{
		typedef GLuint Handle;
	}

	namespace shader
	{
		typedef GLuint Handle;
		const Handle kUninitialized = 0;

		bool isInitialized(const Handle& handle);
		Handle create(GLenum type, const char* shaderCode);
		void destroy(const program::Handle& phandle, Handle& handle);
	}

} // namespace el {
} // namespace gl {
