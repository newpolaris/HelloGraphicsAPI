#pragma once

#include "platform.h"

#define EL_CONFIG_DEBUG 0

#if defined(_DEBUG) || !defined(NDEBUG)
#	undef EL_CONFIG_DEBUG
#	define EL_CONFIG_DEBUG 1
#endif
