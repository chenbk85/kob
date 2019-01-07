
#ifndef KOB_PROFILE_COMPILER_TRAITS_H_
#define KOB_PROFILE_COMPILER_TRAITS_H_
#include <kob/profile/compiler.h>

#ifndef KOB_MAY_ALIAS_AVAILABLE
    #if defined(KOB_COMPILER_GNUC) && KOB_COMPILER_VERSION >= 3003
        #define KOB_MAY_ALIAS_AVAILABLE 2
    #else
        #define KOB_MAY_ALIAS_AVAILABLE 0
    #endif
#endif

#ifndef KOB_MAY_ALIAS
    #if KOB_MAY_ALIAS_AVAILABLE
        #define KOB_MAY_ALIAS __attribute__((__may_alias__))
    #else
        #define KOB_MAY_ALIAS
    #endif
#endif

#ifndef KOB_PURE
    #if defined(KOB_COMPILER_GNUC)
        #define KOB_PURE __attribute__((pure))
    #else
        #define KOB_PURE
    #endif
#endif

#ifndef KOB_WEAK
    #define KOB_WEAK __attribute__((weak))
#endif

#ifndef KOB_UNUSED
        #define KOB_UNUSED(x) (void)x
#endif

#ifndef KOB_EMPTY
    #define  KOB_EMPTY (void)0
#endif

#ifndef KOB_OVERRIDE
    #if KOB_COMPILER_NO_OVERRIDE
        #define KOB_OVERRIDE
    #else
        #define KOB_OVERRIDE override
    #endif
#endif

#ifndef KOB_INHERITANCE_FINAL
    #if KOB_COMPILER_NO_INHERITANCE_FINAL
        #define KOB_INHERITANCE_FINAL
    #else
        #define KOB_INHERITANCE_FINAL final
    #endif
#endif

#ifndef KOB_FINAL_OVERRIDE
    #define KOB_FINAL_OVERRIDE KOB_OVERRIDE KOB_INHERITANCE_FINAL
#endif

#ifndef KOB_CONSTEXPR
    #if KOB_COMPILER_NO_CONSTEXPR
        #define KOB_CONSTEXPR
    #else
        #define KOB_CONSTEXPR constexpr
    #endif
#endif

#ifndef KOB_FUNC_CONSTEXPR
    #if KOB_COMPILER_CPP14_ENABLED
        #define KOB_FUNC_CONSTEXPR constexpr
    #else
        #define KOB_FUNC_CONSTEXPR
    #endif
#endif

#ifndef KOB_CONSTEXPR_OR_CONST
    #if KOB_COMPILER_NO_CONSTEXPR
        #define KOB_CONSTEXPR_OR_CONST const
    #else
        #define KOB_CONSTEXPR_OR_CONST constexpr
    #endif
#endif

#ifndef KOB_EXTERN_TEMPLATE
    #if KOB_COMPILER_NO_EXTERN_TEMPLATE
        #define KOB_EXTERN_TEMPLATE(declaration)
    #else
        #define KOB_EXTERN_TEMPLATE(declaration)  extern template declaration
    #endif
#endif

#ifndef KOB_NOEXCEPT
    #if KOB_COMPILER_NO_NOEXCEPT
        #define KOB_NOEXCEPT
        #define KOB_NOEXCEPT_IF(predicate)
        #define KOB_NOEXCEPT_EXPR(expression) false
    #else
        #define KOB_NOEXCEPT noexcept
        #define KOB_NOEXCEPT_IF(predicate) noexcept((predicate))
        #define KOB_NOEXCEPT_EXPR(expression) noexcept((expression))
    #endif
#endif

#ifndef KOB_NORETURN
    #if KOB_COMPILER_NO_NORETURN
        #define KOB_NORETURN
    #else
        #define KOB_NORETURN [[noreturn]]
    #endif
#endif

#ifndef KOB_MAY_UNUSED
    #if KOB_COMPILER_NO_MAYBE_UNUSED
        #define KOB_MAY_UNUSED
    #else
        #define KOB_MAY_UNUSED [[maybe_unused]]
    #endif
#endif

#ifndef KOB_NON_COPYABLE
    #if KOB_COMPILER_NO_DELETED_FUNCTIONS
        #define KOB_NON_COPYABLE(ClassName_) \
            private:                                      \
				ClassName_(const ClassName_&);                  \
				void operator=(const ClassName_&);
    #else
        #define KOB_NON_COPYABLE(ClassName_) \
				ClassName_(const ClassName_&) = delete;         \
				void operator=(const ClassName_&) = delete;
    #endif
#endif

#ifndef KOB_FUNCTION_DELETE
    #if KOB_COMPILER_NO_DELETED_FUNCTIONS
        #define KOB_FUNCTION_DELETE
    #else
        #define KOB_FUNCTION_DELETE = delete
    #endif
#endif

#ifndef KOB_DISABLE_DEFAULT_CTOR
    #define KOB_DISABLE_DEFAULT_CTOR(ClassName) ClassName() KOB_FUNCTION_DELETE
    #define KOB_DISABLE_COPY_CTOR(ClassName) ClassName(const (ClassName) &) KOB_FUNCTION_DELETE
    #define KOB_DISABLE_MOVE_CTOR(ClassName) ClassName((ClassName)&&) KOB_FUNCTION_DELETE
    #define KOB_DISABLE_ASSIGNMENT_OPERATOR(ClassName) ClassName& operator=(const (ClassName) &) KOB_FUNCTION_DELETE
    #define KOB_DISABLE_MOVE_OPERATOR(ClassName) ClassName& operator=(ClassName&&) KOB_FUNCTION_DELETE
#endif

#endif //KOB_PROFILE_COMPILER_TRAITS_H_
