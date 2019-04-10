#pragma once

#include "platform.h"

#define EL_CONFIG_DEBUG 0

#if defined(_DEBUG) || !defined(NDEBUG)
#	undef EL_CONFIG_DEBUG
#	define EL_CONFIG_DEBUG 1
#endif

// target: ios and android legacy
#define EL_BUILD_DEVICE_ES2 1
#define EL_BUILD_DEVICE_ES3 1
// target: osx legacy support (framebuffer and vao supported by ext)
#define EL_BUILD_DEVICE_OPENGL 1
// target: 4.3 over; not supported by osx (4.1)
#define EL_BUILD_DEVICE_OPENGLCORE 1
#define EL_BUILD_DEVICE_METAL 1
