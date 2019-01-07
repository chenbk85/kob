
#ifndef KOB_PROFILE_COMPILER_ALIGN_H_
#define KOB_PROFILE_COMPILER_ALIGN_H_
#include <kob/profile/compiler.h>
#include <kob/profile/compiler_functionality.h>

#ifndef KOB_COMPILER_NO_ALIGNED_NEW
    #if KOB_COMPILER_CPP17_ENABLED
        #define KOB_COMPILER_NO_ALIGNED_NEW 0
    #else
        #define KOB_COMPILER_NO_ALIGNED_NEW 1
    #endif
#endif

#ifndef KOB_OFFSETOF
    #if defined(KOB_COMPILER_GNUC)
        #define KOB_OFFSETOF(struct_, member_) ((size_t)(((uintptr_t)&reinterpret_cast<const volatile char&>((((struct_*)65536)->member_))) - 65536))
    #else
        #define KOB_OFFSETOF(struct_, member_) offsetof(struct_, member_)
    #endif
#endif

#ifndef KOB_SIZEOF_MEMBER
    #if KOB_COMPILER_NO_EXTENDED_SIZEOF
        #define KOB_SIZEOF_MEMBER(struct_, member_) (sizeof(((struct_*)0)->member_))
    #else
        #define KOB_SIZEOF_MEMBER(struct_, member_) (sizeof(struct_::member_))
    #endif
#endif

#ifndef KOB_ALIGN_OF
    #if (defined(KOB_COMPILER_GNUC) && KOB_COMPILER_VERSION > 4003) || defined(KOB_COMPILER_CLANG)
        #define KOB_ALIGN_OF(type) ((size_t)__alignof__(type))
        #define KOB_ALIGN(n)
        #define KOB_PREFIX_ALIGN(n)
        #define KOB_POSTFIX_ALIGN(n) __attribute__((aligned(n)))
        #define KOB_ALIGNED(variable_type, variable, n) variable_type variable __attribute__((aligned(n)))
        #define KOB_PACKED  __attribute__((packed))
    #else
        #define KOB_ALIGN(n)
        #define KOB_PREFIX_ALIGN(n)
        #define KOB_POSTFIX_ALIGN(n)
        #define KOB_ALIGNED(variable_type, variable, n) variable_type variable
        #define KOB_PACKED
    #endif

#endif

#endif //KOB_PROFILE_COMPILER_ALIGN_H_
