//
// Created by 李寅斌 on 2018/12/11.
//

#ifndef KOB_KSl_DETAIL_TYPE_COMPOUND_H_
#define KOB_KSl_DETAIL_TYPE_COMPOUND_H_
#include <kob/ksl/detail/config.h>

namespace ksl {
    //extent
    template<typename T, unsigned N>
    struct extent_help : public integral_constant<size_t, 0> {};

    template<typename T, unsigned I>
    struct extent_help<T[I], 0> : public integral_constant<size_t, I> {};

    template<typename T, unsigned N, unsigned I>
    struct extent_help<T[I], N> : public extent_help<T, N - 1> { };

    template<typename T, unsigned N>
    struct extent_help<T[], N> : public extent_help<T, N - 1> {};

    template<typename T, unsigned N = 0> // extent uses unsigned instead of size_t.
    struct extent : public extent_help<T, N> { };

    //is_array
    template<typename T>
    struct is_array : public false_type {};

    template<typename T>
    struct is_array<T[]> : public true_type {};

    template<typename T, size_t N>
    struct is_array<T[N]> : public true_type {};

    #if KSL_VARIABLE_TEMPLATES_ENABLED
        template<typename T>
        KOB_CONSTEXPR bool is_array_v = is_array<T>::value;
    #endif

    //is_array_of_known_bounds
    template<typename T>
    struct is_array_of_known_bounds
            : public integral_constant<bool, extent<T>::value != 0> {};

    //is_array_of_unknown_bounds
    template<typename T>
    struct is_array_of_unknown_bounds
            : public integral_constant<bool, is_array<T>::value && (extent<T>::value == 0)> {};

    //is_member_function_pointer
    template <typename T> struct is_mem_fun_pointer_value : public false_type{};

    template <typename R, typename T> struct is_mem_fun_pointer_value<R (T::*)()> : public true_type{};
    template <typename R, typename T> struct is_mem_fun_pointer_value<R (T::*)() const> : public true_type{};
    template <typename R, typename T> struct is_mem_fun_pointer_value<R (T::*)() volatile> : public true_type{};
    template <typename R, typename T> struct is_mem_fun_pointer_value<R (T::*)() const volatile> : public true_type{};

    template <typename R, typename T, typename Arg0> struct is_mem_fun_pointer_value<R (T::*)(Arg0)> : public true_type{};
    template <typename R, typename T, typename Arg0> struct is_mem_fun_pointer_value<R (T::*)(Arg0) const> : public true_type{};
    template <typename R, typename T, typename Arg0> struct is_mem_fun_pointer_value<R (T::*)(Arg0) volatile> : public true_type{};
    template <typename R, typename T, typename Arg0> struct is_mem_fun_pointer_value<R (T::*)(Arg0) const volatile> : public true_type{};

    template <typename R, typename T, typename Arg0, typename Arg1> struct is_mem_fun_pointer_value<R (T::*)(Arg0, Arg1)> : public true_type{};
    template <typename R, typename T, typename Arg0, typename Arg1> struct is_mem_fun_pointer_value<R (T::*)(Arg0, Arg1) const> : public true_type{};
    template <typename R, typename T, typename Arg0, typename Arg1> struct is_mem_fun_pointer_value<R (T::*)(Arg0, Arg1) volatile> : public true_type{};
    template <typename R, typename T, typename Arg0, typename Arg1> struct is_mem_fun_pointer_value<R (T::*)(Arg0, Arg1) const volatile> : public true_type{};

    template <typename R, typename T, typename Arg0, typename Arg1, typename Arg2> struct is_mem_fun_pointer_value<R (T::*)(Arg0, Arg1, Arg2)> : public true_type{};
    template <typename R, typename T, typename Arg0, typename Arg1, typename Arg2> struct is_mem_fun_pointer_value<R (T::*)(Arg0, Arg1, Arg2) const> : public true_type{};
    template <typename R, typename T, typename Arg0, typename Arg1, typename Arg2> struct is_mem_fun_pointer_value<R (T::*)(Arg0, Arg1, Arg2) volatile> : public true_type{};
    template <typename R, typename T, typename Arg0, typename Arg1, typename Arg2> struct is_mem_fun_pointer_value<R (T::*)(Arg0, Arg1, Arg2) const volatile> : public true_type{};

    template <typename R, typename T, typename Arg0, typename Arg1, typename Arg2, typename Arg3> struct is_mem_fun_pointer_value<R (T::*)(Arg0, Arg1, Arg2, Arg3)> : public true_type{};
    template <typename R, typename T, typename Arg0, typename Arg1, typename Arg2, typename Arg3> struct is_mem_fun_pointer_value<R (T::*)(Arg0, Arg1, Arg2, Arg3) const> : public true_type{};
    template <typename R, typename T, typename Arg0, typename Arg1, typename Arg2, typename Arg3> struct is_mem_fun_pointer_value<R (T::*)(Arg0, Arg1, Arg2, Arg3) volatile> : public true_type{};
    template <typename R, typename T, typename Arg0, typename Arg1, typename Arg2, typename Arg3> struct is_mem_fun_pointer_value<R (T::*)(Arg0, Arg1, Arg2, Arg3) const volatile> : public true_type{};

    template <typename R, typename T, typename Arg0, typename Arg1, typename Arg2, typename Arg3, typename Arg4> struct is_mem_fun_pointer_value<R (T::*)(Arg0, Arg1, Arg2, Arg3, Arg4)> : public true_type{};
    template <typename R, typename T, typename Arg0, typename Arg1, typename Arg2, typename Arg3, typename Arg4> struct is_mem_fun_pointer_value<R (T::*)(Arg0, Arg1, Arg2, Arg3, Arg4) const> : public true_type{};
    template <typename R, typename T, typename Arg0, typename Arg1, typename Arg2, typename Arg3, typename Arg4> struct is_mem_fun_pointer_value<R (T::*)(Arg0, Arg1, Arg2, Arg3, Arg4) volatile> : public true_type{};
    template <typename R, typename T, typename Arg0, typename Arg1, typename Arg2, typename Arg3, typename Arg4> struct is_mem_fun_pointer_value<R (T::*)(Arg0, Arg1, Arg2, Arg3, Arg4) const volatile> : public true_type{};

    template <typename R, typename T, typename Arg0, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5> struct is_mem_fun_pointer_value<R (T::*)(Arg0, Arg1, Arg2, Arg3, Arg4, Arg5)> : public true_type{};
    template <typename R, typename T, typename Arg0, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5> struct is_mem_fun_pointer_value<R (T::*)(Arg0, Arg1, Arg2, Arg3, Arg4, Arg5) const> : public true_type{};
    template <typename R, typename T, typename Arg0, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5> struct is_mem_fun_pointer_value<R (T::*)(Arg0, Arg1, Arg2, Arg3, Arg4, Arg5) volatile> : public true_type{};
    template <typename R, typename T, typename Arg0, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5> struct is_mem_fun_pointer_value<R (T::*)(Arg0, Arg1, Arg2, Arg3, Arg4, Arg5) const volatile> : public true_type{};

    template <typename R, typename T, typename Arg0, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6> struct is_mem_fun_pointer_value<R (T::*)(Arg0, Arg1, Arg2, Arg3, Arg4, Arg5, Arg6)> : public true_type{};
    template <typename R, typename T, typename Arg0, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6> struct is_mem_fun_pointer_value<R (T::*)(Arg0, Arg1, Arg2, Arg3, Arg4, Arg5, Arg6) const> : public true_type{};
    template <typename R, typename T, typename Arg0, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6> struct is_mem_fun_pointer_value<R (T::*)(Arg0, Arg1, Arg2, Arg3, Arg4, Arg5, Arg6) volatile> : public true_type{};
    template <typename R, typename T, typename Arg0, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6> struct is_mem_fun_pointer_value<R (T::*)(Arg0, Arg1, Arg2, Arg3, Arg4, Arg5, Arg6) const volatile> : public true_type{};

    template <typename R, typename T, typename Arg0, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7> struct is_mem_fun_pointer_value<R (T::*)(Arg0, Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7)> : public true_type{};
    template <typename R, typename T, typename Arg0, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7> struct is_mem_fun_pointer_value<R (T::*)(Arg0, Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7) const> : public true_type{};
    template <typename R, typename T, typename Arg0, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7> struct is_mem_fun_pointer_value<R (T::*)(Arg0, Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7) volatile> : public true_type{};
    template <typename R, typename T, typename Arg0, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7> struct is_mem_fun_pointer_value<R (T::*)(Arg0, Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7) const volatile> : public true_type{};

    template <typename T>
    struct is_member_function_pointer : public integral_constant<bool, is_mem_fun_pointer_value<T>::value>{};

    //is_member_pointer
    template <typename T>
    struct is_member_pointer
            : public integral_constant<bool, is_member_function_pointer<T>::value>{};

    template <typename T, typename U>
    struct is_member_pointer<U T::*>
            : public true_type{};
    //is_member_object_pointer
    template<typename T>
    struct is_member_object_pointer : public integral_constant<bool,
            is_member_pointer<T>::value &&
            !is_member_function_pointer<T>::value
    > {};

    template <typename T> struct is_pointer_helper : public false_type{};

    template <typename T> struct is_pointer_helper<T*>                : public true_type{};
    template <typename T> struct is_pointer_helper<T* const>          : public true_type{};
    template <typename T> struct is_pointer_helper<T* volatile>       : public true_type{};
    template <typename T> struct is_pointer_helper<T* const volatile> : public true_type{};

    template <typename T>
    struct is_pointer_value : public type_and<is_pointer_helper<T>::value, type_not<is_member_pointer<T>::value>::value> {};

    template <typename T>
    struct is_pointer : public integral_constant<bool, is_pointer_value<T>::value>{};

    #if KSL_VARIABLE_TEMPLATES_ENABLED
        template<typename T>
        KOB_CONSTEXPR bool is_pointer_v = is_pointer<T>::value;
    #endif

    //is_convertible
    template<typename From, typename To, bool = is_void<From>::value || is_function<To>::value || is_array<To>::value >
    struct is_convertible_helper // Anything is convertible to void. Nothing is convertible to a function or an array.
    { static const bool value = is_void<To>::value; };

    template<typename From, typename To>
    class is_convertible_helper<From, To, false>
    {
        template<typename To1>
        static void ToFunction(To1);    // We try to call this function with an instance of From. It is valid if From can be converted to To.

        template<typename /*From1*/, typename /*To1*/>
        static no_type is(...);

        template<typename From1, typename To1>
        static decltype(ToFunction<To1>(declval<From1>()), yes_type()) is(int);

    public:
        static const bool value = sizeof(is<From, To>(0)) == 1;
    };

    template<typename From, typename To>
    struct is_convertible
            : public integral_constant<bool, is_convertible_helper<From, To>::value> {};

    #if KSL_VARIABLE_TEMPLATES_ENABLED
        template<typename From, typename To>
        KOB_CONSTEXPR bool is_convertible_v = is_convertible<From, To>::value;
    #endif

    //is_union
    template <typename T>
    struct is_union : public integral_constant<bool, __is_union(T)>{};

    //is_class
    template <typename T>
    struct is_class : public integral_constant<bool, __is_class(T)>{};

    //is_enum
    template <typename T>
    struct is_enum : public integral_constant<bool, __is_enum(T)>{};

    //is_polymorphic
    template <typename T>
    struct is_polymorphic : public integral_constant<bool, __is_polymorphic(T)>{};

    //is_object
    template <typename T>
    struct is_object : public integral_constant<bool,
            !is_reference<T>::value && !is_void<T>::value && !is_function<T>::value>{};

    //is_scalar
    template <typename T>
    struct is_scalar : public integral_constant<bool,
            is_arithmetic<T>::value || is_enum<T>::value || is_pointer<T>::value ||
            is_member_pointer<T>::value ||
            is_null_pointer<T>::value> {};

    template <typename T> struct is_scalar<T*>                : public true_type {};
    template <typename T> struct is_scalar<T* const>          : public true_type {};
    template <typename T> struct is_scalar<T* volatile>       : public true_type {};
    template <typename T> struct is_scalar<T* const volatile> : public true_type {};

    //is_compound
    template <typename T>
    struct is_compound : public integral_constant<bool, !is_fundamental<T>::value>{};

    //decay

    template<typename T>
    struct decay
    {
        typedef typename remove_reference<T>::type U;

        typedef typename conditional<
                is_array<U>::value,
                typename remove_extent<U>::type*,
                typename conditional<
                        is_function<U>::value,
                        typename add_pointer<U>::type,
                        typename remove_cv<U>::type
                >::type
        >::type type;
    };

    #if KSL_TEMPLATE_ALIASES_ENABLED
        template<typename T>
        using decay_t = typename decay<T>::type;
    #endif

    //common_type
    template<typename... T>
    struct common_type;

    template<typename T>
    struct common_type<T>
    { typedef typename decay<T>::type type; }; // Question: Should we use T or decay_t<T> here? The C++11 Standard specifically (20.9.7.6,p3) specifies that it be without decay, but libc++ uses decay.

    template<typename T, typename U>
    struct common_type<T, U> {
        typedef typename decay<decltype(true ? declval<T>() : declval<U>())>::type type; // The type of a tertiary expression is set by the compiler to be the common type of the two result types.
    };

    template<typename T, typename U, typename... V>
    struct common_type<T, U, V...> {
        typedef typename common_type<typename common_type<T, U>::type, V...>::type type;
    };

    #if KSL_TEMPLATE_ALIASES_ENABLED
        template <typename... T>
        using common_type_t = typename common_type<T...>::type;
    #endif

    //is_final
    #if KOB_COMPILER_HAS_FEATURE(is_final)
        template <typename T>
        struct is_final : public integral_constant<bool, __is_final(T)> {};
    #else
        template <typename T>
        struct is_final : public false_type {};
    #endif
}
#endif //KOB_KSl_DETAIL_TYPE_COMPOUND_H_
