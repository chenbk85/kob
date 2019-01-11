//
// Created by liyinbin on 2018/10/8.
//

#ifndef KOB_PROFILE_COMPILER_OPTIMIZATION_H_
#define KOB_PROFILE_COMPILER_OPTIMIZATION_H_
#include <kob/profile/compiler.h>
#include <kob/profile/compiler_warning.h>

#ifndef KOB_LIKELY
    #if defined(KOB_COMPILER_GNUC)
        #if defined(__cplusplus)
            #define KOB_LIKELY(x) __builtin_expect(!!(x), true)
            #define KOB_UNLIKELY(x) __builtin_expect(!!(x), false)
        #else
            #define KOB_LIKELY(x) __builtin_expect(!!(x), 1)
            #define KOB_UNLIKELY(x) __builtin_expect(!!(x), 0)
        #endif
    #else
        #define KOB_LIKELY(x) (x)
        #define KOB_UNLIKELY(x) (x)
    #endif
#endif

#ifndef KOB_RESTRICT
    #if defined(KOB_COMPILER_GNUC)
        #define KOB_RESTRICT __restrict
    #elif defined(KOB_COMPILER_CLANG)
        #define KOB_RESTRICT __restrict
    #else
        #define KOB_RESTRICT
    #endif
#endif

#ifndef KOB_DEPRECATED
    #if KOB_COMPILER_CPP14_ENABLED
        #define KOB_DEPRECATED [[deprecated]]
    #elif defined(KOB_COMPILER_GNUC) || defined(KOB_COMPILER_CLANG)
        #define KOB_DEPRECATED __attribute__((deprecated))
    #else
        #define KOB_DEPRECATED
    #endif
#endif

#ifndef KOB_DEPRECATED_MSG
#if KOB_COMPILER_CPP14_ENABLED
        #define KOB_DEPRECATED_MSG(m) [[deprecated(#m)]]
    #else
        #define KOB_DEPRECATED_MSG(m)
    #endif
#endif

#ifndef KOB_FORCE_INLINE
    #if (defined(KOB_COMPILER_GNUC) && KOB_COMPILER_VERSION >=3001) || defined(KOB_COMPILER_CLANG)
        #if defined(__cplusplus)
            #define KOB_FORCE_INLINE inline __attribute__((always_inline))
        #else
            #define KOB_FORCE_INLINE __inline__ __attribute__((always_inline))
        #endif
    #else
        #if defined(_cplusplus)
            #define KOB_FORCE_INLINE inline
        #else
            #define #define KOB_FORCE_INLINE __inline
        #endif
    #endif
#endif

#ifndef KOB_NO_INLINE
    #if defined(KOB_COMPILER_GNUC) || defined(KOB_COMPILER_CLANG)
        #define KOB_NO_INLINE __atttribute__((noinline))
    #else
        #define KOB_NO_INLINE
    #endif
#endif

#ifndef KOB_OPTIMIZE_OFF
    #if defined(KOB_COMPILER_GNUC) && (KOB_COMPILER_VERSION > 4004) && (defined(__i386__) || defined(__x86_64__))
        #define KOB_OPTIMIZE_OFF()            \
				_Pragma("GCC push_options")      \
				_Pragma("GCC optimize 0")
    #elif defined(KOB_COMPILER_CLANG) && (EA_COMPILER_VERSION >= 380)
            #define EA_OPTIMIZE_OFF() \
            KOB_DISABLE_CLANG_WARNING(-Wunknown-pragmas) \
            _Pragma("clang optimize off") \
			KOB_RESTORE_CLANG_WARNING()
    #else
        #define EA_OPTIMIZE_OFF()
    #endif
#endif

#ifndef KOB_OPTIMIZE_ON
    #if defined(KOB_COMPILER_GNUC) && (KOB_COMPILER_VERSION > 4004) && (defined(__i386__) || defined(__x86_64__))
        #define KOB_OPTIMIZE_ON()  _Pragma("GCC pop_options")
    #elif defined(KOB_COMPILER_CLANG) && (EA_COMPILER_VERSION >= 380)
        #define KOB_OPTIMIZE_ON() \
            KOB_DISABLE_CLANG_WARNING(-Wunknown-pragmas) \
            _Pragma("clang optimize on") \
            KOB_RESTORE_CLANG_WARNING()
    #else
        #define KOB_OPTIMIZE_ON()
    #endif
#endif

#endif //KOB_PROFILE_COMPILER_OPTIMIZATION_H_
