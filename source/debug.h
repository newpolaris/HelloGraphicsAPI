#pragma once

#include "predefine.h"

#include <cassert>

namespace el {
	void trace(const char* format, ...);
	void debug_break();
	void debug_output(const char* message);
}

#if EL_CONFIG_DEBUG
#	define EL_TRACE _EL_TRACE
#   define EL_WARN_IF _EL_WARN_IF
#else
#	define EL_TRACE(_format, ...)   ((void)(_format))
#   define EL_WARN_IF(_cond, ...)   ((void)(_cond))
#endif

#define _EL_TRACE(_format, ...)	\
	do { \
		el::trace("EL_TRACE: %s:%d \n" _format, __FILE__, __LINE__, ##__VA_ARGS__); \
	} while (0)

#define _EL_WARN_IF(_cond, _format, ...) \
	do { \
        if (_cond) \
            EL_TRACE(_format, ##__VA_ARGS__); \
    } while (0)

#if EL_CONFIG_DEBUG
#   if !EL_CONFIG_DEBUG_BREAK
#       define EL_ASSERT(_call) assert(_call)
#   else
#       define EL_ASSERT(_call) (void)( \
                (!!(_call)) || \
                (debug_break(), 0) \
                )
#   endif
#else
#   define EL_ASSERT(_call) ((void)(_call))
#   define EL_WARN(_cond, _format, ...) ((void)(_cond))
#endif
