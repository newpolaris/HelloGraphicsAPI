#pragma once

#include "platform.h"

#define EL_CONFIG_DEBUG 0

#if defined(_DEBUG) || !defined(NDEBUG)
#   undef EL_CONFIG_DEBUG
#   define EL_CONFIG_DEBUG 1
#endif
#define EL_CONFIG_DEBUG_BREAK 1

// target: osx legacy support (framebuffer and vao supported)
#define EL_BUILD_OPENGL 1
// target: ios and android legacy
#define EL_BUILD_OPENGL_ES2 0
#define EL_BUILD_OPENGL_ES3 0
// target: 4.3 over; not supported by osx (4.1)
#define EL_BUILD_OPENGL_CORE 0
#define EL_BUILD_DEVICE_METAL 0
