#pragma once

#define EL_ARCH_ARM 0
#define EL_ARCH_X86_32 0
#define EL_ARCH_X86_64 0
#define EL_ARCH_X86 0

#define EL_COMP_GNUC 0
#define EL_COMP_CLANG 0
#define EL_COMP_MSVC 0

#define EL_PLAT_WINDOWS 0
#define EL_PLAT_ANDROID 0
#define EL_PLAT_APPLE 0
#define EL_PLAT_IOS 0
#define EL_PLAT_IOS_DEVICE 0
#define EL_PLAT_IOS_SIMULATOR 0
#define EL_PLAT_OSX 0

// https://www.boost.org/doc/libs/1_66_0/doc/html/predef
#if defined(__arm__) || defined(__arm64) || defined(__thumb__) || \
    defined(__TARGET_ARCH_ARM) || defined(__TARGET_ARCH_THUMB) || \
    defined(_M_ARM)
#   undef EL_ARCH_ARM
#   define EL_ARCH_ARM 1
#endif

#if defined(i386) || defined(__i386__) || \
    defined(__i486__) || defined(__i586__) || \
    defined(__i686__) || defined(__i386) || \
    defined(_M_IX86) || defined(_X86_) || \
    defined(__THW_INTEL__) || defined(__I86__) || \
    defined(__INTEL__)
#   undef EL_ARCH_X86_32
#   define EL_ARCH_X86_32 1
#endif

#if defined(__x86_64) || defined(__x86_64__) || \
    defined(__amd64__) || defined(__amd64) || \
    defined(_M_X64)
#   undef EL_ARCH_X86_64
#   define EL_ARCH_X86_64 1
#endif

#if EL_ARCH_X86_32 || EL_ARCH_X86_64
#   undef EL_ARCH_X86
#   define EL_ARCH_X86 1
#endif

// http://sourceforge.net/apps/mediawiki/predef/index.php?title=Compilers
#if defined(__clang__)
#   undef  EL_COMP_CLANG
#   define EL_COMP_CLANG (__clang_major__ * 10000 + __clang_minor__ * 100 + __clang_patchlevel__)
#elif defined(_MSC_VER)
#   undef  EL_COMP_MSVC
#   define EL_COMP_MSVC _MSC_VER
#elif defined(__GNUC__)
#   undef  EL_COMP_GCC
#   define EL_COMP_GCC (__GNUC__ * 10000 + __GNUC_MINOR__ * 100 + __GNUC_PATCHLEVEL__)
#endif

// https://www.boost.org/doc/libs/1_66_0/doc/html/predef
#if defined(_WIN32) || defined(_WIN64) || defined(__WIN32__) || defined(__WINDOWS__)
#   undef EL_PLAT_WINDOWS
#   define EL_PLAT_WINDOWS 1
#elif defined(__ANDROID__)
#   include <android/api-level.h>
#   undef EL_PLAT_ANDROID
#   define EL_PLAT_ANDROID __ANDROID_API__
#elif  defined(__ENVIRONMENT_IPHONE_OS_VERSION_MIN_REQUIRED__) \
    || defined(__ENVIRONMENT_TV_OS_VERSION_MIN_REQUIRED__)
#   undef  EL_PLAT_IOS
#   define EL_PLAT_IOS 1
#elif defined(__ENVIRONMENT_MAC_OS_X_VERSION_MIN_REQUIRED__)
#   undef  EL_PLAT_OSX
#   define EL_PLAT_OSX __ENVIRONMENT_MAC_OS_X_VERSION_MIN_REQUIRED__
#endif

#if EL_PLAT_IOS
#    include <TargetConditionals.h>
#    if TARGET_OS_SIMULATOR == 1
#        undef EL_PLAT_IOS_SIMULATOR
#        define EL_PLAT_IOS_SIMULATOR BOOST_VERSION_NUMBER_AVAILABLE
#    else
#        undef EL_PLAT_IOS_DEVICE
#        define EL_PLAT_IOS_DEVICE BOOST_VERSION_NUMBER_AVAILABLE
#    endif
#endif

#if EL_PLAT_ISO || EL_PLAT_OSX
#   undef EL_PLAT_APPLE
#   define EL_PLAT_APPLE 1
#endif
