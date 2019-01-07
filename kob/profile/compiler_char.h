
#ifndef KOB_PROFILE_COMPILER_CHAR_H_
#define KOB_PROFILE_COMPILER_CHAR_H_

#include <kob/profile/platform.h>
#include <kob/profile/compiler.h>
#include <kob/profile/compiler_functionality.h>

#ifndef KOB_WCHAR_T_NON_NATIVE
    #define KOB_WCHAR_T_NON_NATIVE 0
#endif

#ifndef KOB_WCHAR_MAX
    #if defined(__WCHAR_MAX__)
        #if (__WCHAR_MAX__ == 2147483647) || (__WCHAR_MAX__ == 4294967295)
            #define KOB_WCHAR_MAX 4
        #elif (__WCHAR_MAX__ == 32767) || (__WCHAR_MAX__ == 65535)
            #define KOB_WCHAR_MAX 2
        #elif (__WCHAR_MAX__ == 127) || (__WCHAR_MAX__ == 255)
            #define KOB_WCHAR_MAX 1
        #else
            #define KOB_WCHAR_MAX 4
        #endif
    #elif defined(KOB_PLATEFORM_UNIX)
        #define KOB_WCHAR_MAX 4
    #else
        #define KOB_WCHAR_MAX 2
    #endif
#endif

#ifdef KOB_PLATFORM_OSX
    #define char8_t char
#else
    typedef char char8_t;
#endif

#ifndef KOB_CHAR16_NATIVE
    #if defined(KOB_COMPILER_CLANG) && KOB_COMPILER_CPP11_ENABLED
        #define KOB_CHAR16_NATIVE 1
        #define KOB_CHAR32_NATIVE 1
    #elif defined(KOB_COMPILER_GNUC) && KOB_COMPILER_VERSION > 4004
        #define KOB_CHAR16_NATIVE 1
        #define KOB_CHAR32_NATIVE 1
    #else
        #define KOB_CHAR16_NATIVE 0
        #define KOB_CHAR32_NATIVE 0
    #endif
#endif

#if KOB_CHAR16_NATIVE && KOB_CHAR32_NATIVE
    #define KOB_WCHAR_UNIQUE 1
#else
    #define KOB_WCHAR_UNIQUE 0
#endif

#endif //KOB_PROFILE_COMPILER_CHAR_H_
