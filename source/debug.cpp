#include "debug.h"

#include <string>
#include <sstream>
#include <stdarg.h>
#include "predefine.h"

#if EL_PLAT_WINDOWS
extern "C" __declspec(dllimport) void __stdcall OutputDebugStringA(const char* _str);
#elif EL_PLAT_APPLE
#    if defined(__OBJC__)
#        import <Foundation/NSObjCRuntime.h>
#    else
#        include <CoreFoundation/CFString.h>
extern "C" void NSLog(CFStringRef _format, ...);
#    endif // defined(__OBJC__)
#else
#    include <stdio.h> // fputs, fflush
#endif // EL_PLAT_WINDOWS

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
#elif EL_PLAT_APPLE
#    if defined(__OBJC__)
    NSLog(@"%s", message);
#    else
    NSLog(CFSTR("%s"), message);
#    endif // defined(__OBJC__)
#else
    fputs(message, stdout);
    fflush(stdout);
#endif // EL_PLAT_WINDOWS
}

void el::debug_break()
{
#if EL_COMP_MSVC
    __debugbreak();
#elif EL_ARCH_ARM
    __builtin_trap();
#elif EL_ARCH_X86 && (EL_COMP_GCC || EL_COMP_CLANG)
    // File violates Native Client safety rules.
    __asm__ ("int $3");
#else // cross platform implementation
    int* int3 = (int*)3L;
    *int3 = 3;
#endif
}
