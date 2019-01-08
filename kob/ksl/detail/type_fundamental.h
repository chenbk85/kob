//
// Created by 李寅斌 on 2018/12/11.
//

#ifndef KOB_KSL_DETAIL_TYPE_FUNDAMENTAL_H_
#define KOB_KSL_DETAIL_TYPE_FUNDAMENTAL_H_
#include <kob/ksl/detail/config.h>

namespace ksl {

    //is_void
    template <typename T> struct is_void : public false_type{};

    template <> struct is_void<void>                : public true_type{};
    template <> struct is_void<void const>          : public true_type{};
    template <> struct is_void<void volatile>       : public true_type{};
    template <> struct is_void<void const volatile> : public true_type{};

    #if KSL_VARIABLE_TEMPLATES_ENABLED
        template <class T>
        KOB_CONSTEXPR bool is_void_v = is_void<T>::value;
    #endif

    //has_void_arg
    template <typename ...Args>
    struct has_void_arg;

    template <>
    struct has_void_arg<>
            : public false_type {};

    template <typename A0, typename ...Args>
    struct has_void_arg<A0, Args...> { 
        static const bool value = (is_void<A0>::value ||
                has_void_arg<Args...>::value);
    };

    //is_null_pointer
    template <typename T>
    struct is_null_pointer : public is_same<typename remove_cv<T>::type, decltype(nullptr)> {};

    #if KSL_VARIABLE_TEMPLATES_ENABLED
        template <class T>
		KOB_CONSTEXPR bool is_null_pointer_v = is_null_pointer<T>::value;
    #endif

    //is_integral
    template <typename T> struct is_integral_helper           : public false_type{};

    template <> struct is_integral_helper<unsigned char>      : public true_type{};
    template <> struct is_integral_helper<unsigned short>     : public true_type{};
    template <> struct is_integral_helper<unsigned int>       : public true_type{};
    template <> struct is_integral_helper<unsigned long>      : public true_type{};
    template <> struct is_integral_helper<unsigned long long> : public true_type{};

    template <> struct is_integral_helper<signed char>        : public true_type{};
    template <> struct is_integral_helper<signed short>       : public true_type{};
    template <> struct is_integral_helper<signed int>         : public true_type{};
    template <> struct is_integral_helper<signed long>        : public true_type{};
    template <> struct is_integral_helper<signed long long>   : public true_type{};

    template <> struct is_integral_helper<bool>               : public true_type{};
    template <> struct is_integral_helper<char>               : public true_type{};
    #if defined(KOB_CHAR16_NATIVE) && KOB_CHAR16_NATIVE
        template <> struct is_integral_helper<char16_t>       : public true_type{};
    #endif
    #if defined(KOB_CHAR32_NATIVE) && KOB_CHAR32_NATIVE
        template <> struct is_integral_helper<char32_t>       : public true_type{};
    #endif
    #if KOB_WCHAR_T_NON_NATIVE == 0
        template <> struct is_integral_helper<wchar_t>        : public true_type{};
    #endif

    template <typename T>
    struct is_integral : public is_integral_helper<typename remove_cv<T>::type>{};

    #if KSL_VARIABLE_TEMPLATES_ENABLED
        template <class T>
        KOB_CONSTEXPR bool is_integral_v = is_integral<T>::value;
    #endif

    //is_floating_point
    template <typename T> struct is_floating_point_helper    : public false_type{};

    template <> struct is_floating_point_helper<float>       : public true_type{};
    template <> struct is_floating_point_helper<double>      : public true_type{};
    template <> struct is_floating_point_helper<long double> : public true_type{};

    template <typename T>
    struct is_floating_point : public is_floating_point_helper<typename remove_cv<T>::type>{};

    #if KSL_VARIABLE_TEMPLATES_ENABLED
        template <class T>
        KOB_CONSTEXPR bool is_floating_point_v = is_floating_point<T>::value;
    #endif

    //is_arithmetic
    template <typename T>
    struct is_arithmetic
            : public integral_constant<bool, is_integral<T>::value || is_floating_point<T>::value> {};


    //is_fundamental
    template <typename T>
    struct is_fundamental
            : public bool_constant<is_void<T>::value || is_integral<T>::value || is_floating_point<T>::value || is_null_pointer<T>::value> {};


    //is_hat_type
    template <typename T> struct is_hat_type_helper : public false_type {};
    template <typename T>
    struct is_hat_type : public is_hat_type_helper<T> {};
}
#endif //KOB_KSL_DETAIL_TYPE_FUNDAMENTAL_H_
