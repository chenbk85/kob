
#ifndef KOB_PROFILE_COMPILER_WARNING_H_
#define KOB_PROFILE_COMPILER_WARNING_H_
#include <kob/profile/compiler.h>

#ifndef KOB_DISABLE_GCC_WARNING
    #if defined(KOB_COMPILER_GNUC)
        #define KOB_GCC_HELP0(x) #x
        #define KOB_GCC_HELP1(x) KOB_GCC_HELP0(GCC diagnostic ignored x)
        #define KOB_GCC_HELP(x) KOB_GCC_HELP1(#x)
    #endif

    #if defined(KOB_COMPILER_GNUC) && (KOB_COMPILER_VERSION >= 4006)
        #define KOB_DISABLE_GCC_WARNING(w) \
            _Pragma("GCC diagnostic push")  \
            _Pragma(KOB_GCC_HELP(w))
    #elif defined(KOB_COMPILER_GNUC) && (KOB_COMPILER_VERSION >= 4004)
        #define KOB_DISABLE_GCC_WARNING(w) \
            _Pragma(KOB_GCC_HELP(w))
    #else
        #define KOB_DISABLE_GCC_WARNING(w)
    #endif
#endif

#ifndef KOB_RESTORE_GCC_WARNING
    #if defined(KOB_COMPILER_GNUC) && (KOB_COMPILER_VERSION >= 4006)
        #define KOB_RESTORE_GCC_WARNING() \
            _Pragma("GCC diagnostic pop")
    #else
        #define KOB_RESTORE_GCC_WARNING()
    #endif
#endif

#ifndef KOB_DISABLE_CLANG_WARNING
    #if defined(KOB_COMPILER_CLANG)
        #define KOB_CLANG_HELP0(x) #x
        #define KOB_CLANG_HELP1(x) KOB_CLANG_HELP0(clang diagnostic ignored x)
        #define KOB_CLANG_HELP(x) KOB_CLANG_HELP1(#x)

        #define KOB_DISABLE_CLANG_WARNING(w) \
            _Pragma("clang diagnostic push")  \
            _Pragma(KOB_CLANG_HELP(w))
    #else
        #define KOB_DISABLE_CLANG_WARNING(w)
    #endif
#endif

#ifndef KOB_RESTORE_CLANG_WARNING
    #if defined(KOB_COMPILER_CLANG)
        #define KOB_RESTORE_CLANG_WARNING() \
            _Pragma("clang diagnostic pop")
    #else
        #define KOB_RESTORE_CLANG_WARNING()
    #endif
#endif

#ifndef KOB_DISABLE_WARNING
    #if defined(KOB_COMPILER_GNUC)
        #define KOB_DISABLE_WARNING(w) KOB_DISABLE_GCC_WARNING(w)
    #elif defined(KOB_COMPILER_CLANG)
        #define KOB_DISABLE_WARNING(w) KOB_DISABLE_CLANG_WARNING(w)
    #else
        #define KOB_DISABLE_WARNING(w)
    #endif
#endif

#ifndef KOB_RESTORE_WARNING
    #if defined(KOB_COMPILER_GNUC)
        #define KOB_RESTORE_WARNING() KOB_RESTORE_GCC_WARNING()
    #elif defined(KOB_COMPILER_CLANG)
        #define KOB_RESTORE_WARNING() KOB_RESTORE_CLANG_WARNING()
    #else
        #define KOB_RESTORE_WARNING()
    #endif
#endif


#endif //KOB_PROFILE_COMPILER_WARNING_H_
