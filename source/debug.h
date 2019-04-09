#pragma once

#include "predefine.h"

#include <cassert>

namespace el {
	void trace(const char* format, ...);
	void debug_break();
	void debug_output(const char* message);
}

#if EL_CONFIG_DEBUG
#	define EL_TRACE		_EL_TRACE
#else
#	define EL_TRACE(_format, ...)	((void)(_format))
#endif

#define _EL_TRACE(_format, ...)	\
	do { \
		el::trace("%s:%d | EL_TRACE: " _format "\n", __FILE__, __LINE__, ##__VA_ARGS__); \
	} while (0)

#if EL_CONFIG_DEBUG
#    define EL_ASSERT(_call) assert(_call)
#else
#    define EL_ASSERT(_call) ((void)(_call))
#endif
