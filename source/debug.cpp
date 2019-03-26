#include "debug.h"

#include <string>
#include <sstream>
#include <stdarg.h>
#include "predefine.h"

#if EL_PLAT_WINDOWS 
extern "C" __declspec(dllimport) void __stdcall OutputDebugStringA(const char* _str);
#endif

void el::trace(const char* format, ...)
{
	const int kLength = 1024;
	char buffer[kLength + 1];
	try {
		va_list argList;
		va_start(argList, format);
		int len = vsnprintf(buffer, kLength, format, argList);
		va_end(argList);
		if (len > kLength)
			len = kLength;
		buffer[kLength] = '\0';

		debug_output(buffer);
	}
	catch (...) {
	}
}

void el::debug_output(const char* message)
{
#if EL_PLAT_WINDOWS 
	OutputDebugStringA(message);
#endif
}

void el::debug_break()
{
#if EL_COMP_MSVC
	__debugbreak();
#elif EL_ARCH_ARM
	__builtin_trap();
#eleif
	__asm int 3;
#endif
}
