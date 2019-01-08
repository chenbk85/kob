//
// Created by 李寅斌 on 2018/12/11.
//

#ifndef KOB_KSL_DETAIL_TYPE_PROPERTIES_H_
#define KOB_KSL_DETAIL_TYPE_PROPERTIES_H_
#include <kob/ksl/detail/config.h>

namespace ksl {
    //underlying_type
    #if (defined(KOB_COMPILER_GNUC) && KOB_COMPILER_VERSION > 4007) || defined(KOB_COMPILER_CLANG)
        template <typename T>
        struct underlying_type{ typedef __underlying_type(T) type; };
    #else
        #error "not support"
    #endif

    #if KSL_TEMPLATE_ALIASES_ENABLED
        template <typename T>
        using underlying_type_t = typename underlying_type<T>::type;
    #endif

    //has_unique_object_representations

    template <typename T>
    struct has_unique_object_representations
            : public integral_constant<bool, is_integral<typename remove_cv<typename remove_all_extents<T>::type>::type>::value>{
    };

    #if KSL_VARIABLE_TEMPLATES_ENABLED
        template <class T>
        KOB_CONSTEXPR auto has_unique_object_representations_v = has_unique_object_representations<T>::value;
    #endif

    //is_signed
    template <typename T> struct is_signed_helper : public false_type{};

    template <> struct is_signed_helper<signed char>      : public true_type{};
    template <> struct is_signed_helper<signed short>     : public true_type{};
    template <> struct is_signed_helper<signed int>       : public true_type{};
    template <> struct is_signed_helper<signed long>      : public true_type{};
    template <> struct is_signed_helper<signed long long> : public true_type{};
    template <> struct is_signed_helper<float>            : public true_type{};
    template <> struct is_signed_helper<double>           : public true_type{};
    template <> struct is_signed_helper<long double>      : public true_type{};

    #if (CHAR_MAX == SCHAR_MAX)
        template <> struct is_signed_helper<char>         : public true_type{};
    #endif

    #if KOB_WCHAR_T_NON_NATIVE == 0
        #if defined(__WCHAR_MAX__) && ((__WCHAR_MAX__ == 2147483647) || (__WCHAR_MAX__ == 32767)) // GCC defines __WCHAR_MAX__ for most platforms.
        template <> struct is_signed_helper<wchar_t>  : public true_type{};
        #endif
    #endif

    template <typename T>
    struct is_signed : public is_signed_helper<typename remove_cv<T>::type>{};

    //is_unsigned
    template <typename T> struct is_unsigned_helper : public false_type{};

    template <> struct is_unsigned_helper<unsigned char>      : public true_type{};
    template <> struct is_unsigned_helper<unsigned short>     : public true_type{};
    template <> struct is_unsigned_helper<unsigned int>       : public true_type{};
    template <> struct is_unsigned_helper<unsigned long>      : public true_type{};
    template <> struct is_unsigned_helper<unsigned long long> : public true_type{};

    #if (CHAR_MAX == UCHAR_MAX)
    template <> struct is_unsigned_helper<char>           : public true_type{};
    #endif
    #ifndef KOB_WCHAR_T_NON_NATIVE // If wchar_t is a native type instead of simply a define to an existing type...
    #if defined(_MSC_VER) || (defined(__WCHAR_MAX__) && ((__WCHAR_MAX__ == 4294967295U) || (__WCHAR_MAX__ == 65535))) // GCC defines __WCHAR_MAX__ for most platforms.
    template <> struct is_unsigned_helper<wchar_t>    : public true_type{};
    #endif
    #endif

    template <typename T>
    struct is_unsigned : public is_unsigned_helper<typename remove_cv<T>::type>{};


    //alignment_of
    template <typename T>
    struct alignment_of_value{ static const size_t value = KOB_ALIGN_OF(T); };

    template <typename T>
    struct alignment_of : public integral_constant<size_t, alignment_of_value<T>::value>{};

    #if KSL_VARIABLE_TEMPLATES_ENABLED
        template <class T>
        KOB_CONSTEXPR size_t alignment_of_v = alignment_of<T>::value;
    #endif

    //is_aligned

    template <typename T>
    struct is_aligned_value{ static const bool value = (KOB_ALIGN_OF(T) > 8); };

    template <typename T>
    struct is_aligned : public integral_constant<bool, is_aligned_value<T>::value>{};

    #if KSL_VARIABLE_TEMPLATES_ENABLED
        template <class T>
        KOB_CONSTEXPR size_t is_aligned_v = is_aligned<T>::value;
    #endif

    //rank
    template<typename T>
    struct rank : public integral_constant<size_t, 0> {};

    template<typename T>
    struct rank<T[]> : public integral_constant<size_t, rank<T>::value + 1> {};

    template<typename T, size_t N>
    struct rank<T[N]> : public integral_constant<size_t, rank<T>::value + 1> {};

    //is_base_of
    template <typename Base, typename Derived>
    struct is_base_of : public integral_constant<bool, __is_base_of(Base, Derived) || is_same<Base, Derived>::value>{};

    #if KOB_VARIABLE_TEMPLATES_ENABLED
        template <typename Base, typename Derived>
        KOB_CPP17_INLINE_VARIABLE KOB_CONSTEXPR bool is_base_of_v = is_base_of<Base, Derived>::value;
    #endif

    //is_lvalue_reference
    template<typename T> struct is_lvalue_reference     : public false_type {};
    template<typename T> struct is_lvalue_reference<T&> : public true_type {};

    //is_rvalue_reference
    template <typename T> struct is_rvalue_reference      : public false_type {};
    template <typename T> struct is_rvalue_reference<T&&> : public true_type {};

    //result_of
    template<typename> struct result_of;

    template<typename F, typename... ArgTypes>
    struct result_of<F(ArgTypes...)> {
        typedef decltype(declval<F>()(declval<ArgTypes>()...)) type;
    };

    #if KSL_TEMPLATE_ALIASES_ENABLED
        template <typename T>
        using result_of_t = typename result_of<T>::type;
    #endif

    //has_equality
    #if KSL_VARIABLE_TEMPLATES_ENABLED
        template <typename, typename = void_t<>>
        struct has_equality : false_type {};

        template <typename T>
        struct has_equality<T, void_t<decltype(declval<T>() == declval<T>())>> : true_type
        {
        };

		template <class T>
		KOB_CONSTEXPR auto has_equality_v = has_equality<T>::value;
    #endif
}
#endif //KOB_KSL_DETAIL_TYPE_PROPERTIES_H_
