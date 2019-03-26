#pragma once

#include "platform.h"

#define EL_CONFIG_DEBUG 0

#ifdef _DEBUG
#	undef EL_CONFIG_DEBUG
#	define EL_CONFIG_DEBUG 1
#endif
