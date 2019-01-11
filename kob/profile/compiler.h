//
// Created by liyinbin on 2018/10/8.
//

#ifndef KOB_PROFILE_COMPILER_H_
#define KOB_PROFILE_COMPILER_H_

#ifndef KOB_COMPILER_HAS_FEATURE
    #if defined(__clang__)
        #define KOB_COMPILER_HAS_FEATURE(x) __has_feature(x)
    #else
        #define KOB_COMPILER_HAS_FEATURE(x) 0
    #endif
#endif

#ifndef KOB_COMPILER_HAS_BUILDIN
    #if defined(__clang__)
        #define KOB_COMPILER_HAS_BUILDIN(x) __buildin(x)
    #else
        #define KOB_COMPILER_HAS_BUILDIN(x) 0
    #endif
#endif

#if !defined(KOB_COMPILER_CPP11_ENABLED) && defined(__cplusplus)
    #if (__cplusplus >= 201103L)
        #define KOB_COMPILER_CPP11_ENABLED 1
    #elif (__GNUC__) && defined(__GXX_EXPERIMENTAL_CXX0X__)
        #define KOB_COMPILER_CPP11_ENABLED 1
    #else
        #define KOB_COMPILER_CPP11_ENABLED 0
    #endif
#endif

#if !defined(KOB_COMPILER_CPP14_ENABLED) && defined(__cplusplus)
    #if (__cplusplus >= 201402L)
        #define KOB_COMPILER_CPP14_ENABLED 1
    #else
        #define KOB_COMPILER_CPP14_ENABLED 0
    #endif
#endif

#if !defined(KOB_COMPILER_CPP17_ENABLED) && defined(__cplusplus)
    #if (__cplusplus >= 201703L)
        #define KOB_COMPILER_CPP17_ENABLED 1
    #else
        #define KOB_COMPILER_CPP17_ENABLED 0
    #endif
#endif

#if defined(__clang__)
    #define KOB_COMPILER_CLANG 1
    #define KOB_COMPILER_VERSION (__clang_major__ * 100 + __clang_minor__)
    #define KOB_COMPILER_NAME "clang"
#elif defined(__GNUC__)
    #define KOB_COMPILER_GNUC 1
    #define KOB_COMPILER_VERSION (__GNUC__ * 1000 + __GNUC_MINOR__)
    #define KOB_COMPILER_NAME "gnuc"
#else
    #error "unkown compiler"
#endif


#endif //KOB_PROFILE_COMPILER_H_
