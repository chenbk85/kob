#ifndef KOB_KSL_TYPE_TRAITS_H_
#define KOB_KSL_TYPE_TRAITS_H_

/**
 * integral_constant
 * true_type
 * false_type
 * yes_type
 * no_type
 * is_const
 * is_volatile
 */
#include <kob/ksl/detail/config.h>
#include <stddef.h>

namespace ksl {

	//helper class
	template <typename T, T v>
	struct integral_constant {
	    typedef T              value_type;
		static KOB_CONSTEXPR T value = v;
		KOB_FUNC_CONSTEXPR operator value_type() const
        {
            return v;
        }

        KOB_FUNC_CONSTEXPR value_type operator ()() const
        {
            return v;
        }

	};

	template <typename T, T v>
	const T integral_constant<T, v>::value;

    //true_type / false_type
    typedef integral_constant<bool, true>  true_type;
    typedef integral_constant<bool, false> false_type;

	#if KSL_TEMPLATE_ALIASES_ENABLED
		template <bool B>
		using bool_constant = integral_constant<bool, B>;
	#else
		template <bool B>
		struct bool_constant : public integral_constant<bool, B>{};
    #endif

	typedef char yes_type;
	struct no_type{ char padding[8]; };

	struct unused { };

    struct argument_sink {
        template<typename... Args>
        argument_sink(Args&&...) {}
    };

	//is_const
	template <typename T>
	struct is_const : public false_type{};
	template <typename T>
	struct is_const<T const> : public true_type{};

	#if KSL_VARIABLE_TEMPLATES_ENABLED
		template <typename T>
		KOB_FORCE_INLINE KOB_CONSTEXPR is_const_v =
				is_const<T>::value;
    #endif

	//is_volatile
	template <typename T>
	struct is_volatile : public false_type{};
	template <typename T>
	struct is_volatile<T volatile> : public true_type{};

	#if KSL_VARIABLE_TEMPLATES_ENABLED
		template <typename T>
		KOB_FORCE_INLINE KOB_CONSTEXPR bool is_volatile_v =
				is_volatile<T>::value;
	#endif

    //remove_const
    template <typename T>
    struct remove_const {
        typedef T type;
    };

    template <typename T>
    struct remove_const<const T> {
        typedef T type;
    };

    #if KSL_TEMPLATES_ALIASES_ENABLED
        template <typename T>
        using remove_const_t = typename remove_const<T>::type;
    #endif

    //remove_volatile

    template <typename T>
    struct remove_volatile {
        typedef T type;
    };

    template <typename T>
    struct remove_volatile<volatile T> {
        typedef T type;
    };

    #if KSL_TEMPLATES_ALIASES_ENABLED
        template <typename T>
        using remove_volatile_t = typename remove_volatile<T>::type;
    #endif

    //remove_cv
    template <typename T>
    struct remove_cv {
        typedef typename remove_volatile<typename remove_const<T>::type>::type type;
    };
    #if KSL_TEMPLATES_ALIASES_ENABLED
        template <typename T>
        using remove_cv_t = typename remove_cv<T>::type;
    #endif

    //is_reference
    template <typename T> struct is_reference     : public false_type{};
    template <typename T> struct is_reference<T&> : public true_type{};
    template <typename T> struct is_reference<T&&> : public true_type{};

    //is_function
    template <typename>
    struct is_function : public false_type {};

    template <typename ReturnValue, typename... ArgPack>
    struct is_function<ReturnValue (ArgPack...)>
            : public true_type {};

    template <typename ReturnValue, typename... ArgPack>
    struct is_function<ReturnValue (ArgPack..., ...)>
            : public true_type {};

    //add_reference
    template <typename T> struct add_reference_impl      { typedef T&   type; };
    template <typename T> struct add_reference_impl<T&>  { typedef T&   type; };
    template <>           struct add_reference_impl<void>{ typedef void type; };

    template <typename T> struct add_reference {
        typedef typename add_reference_impl<T>::type type;
    };
    #if KSL_TEMPLATES_ALIASES_ENABLED
        template <typename T>
        using add_reference_t = typename add_reference<T>::type;
    #endif

    //remove_reference
    template <typename T> struct remove_reference    { typedef T type; };
    template <typename T> struct remove_reference<T&>{ typedef T type; };

    template <typename T> struct remove_reference<T&&>{ typedef T type; };
    #if KSL_TEMPLATES_ALIASES_ENABLED
        template<typename T>
        using remove_reference_t = typename remove_reference<T>::type;
    #endif

    #if KSL_TEMPLATES_ALIASES_ENABLED
        template <typename T>
        struct remove_cvref { typedef typename remove_volatile<typename remove_const<typename remove_reference<T>::type>::type>::type type; };

        template<typename T>
        using remove_cvref_t = typename remove_cvref<T>::type;
    #endif

    //add_lvalue_reference
    template <typename T> struct add_lvalue_reference                      { typedef T& type;                  };
    template <typename T> struct add_lvalue_reference<T&>                  { typedef T& type;                  };
    template <>           struct add_lvalue_reference<void>                { typedef void type;                };
    template <>           struct add_lvalue_reference<const void>          { typedef const void type;          };
    template <>           struct add_lvalue_reference<volatile void>       { typedef volatile void type;       };
    template <>           struct add_lvalue_reference<const volatile void> { typedef const volatile void type; };

    #if KSL_TEMPLATES_ALIASES_ENABLED
        template <typename T>
        using add_lvalue_reference_t = typename add_lvalue_reference<T>::type;
    #endif

    //add_rvalue_reference
    template <typename T> struct add_rvalue_reference                      { typedef T&& type;                 };
    template <typename T> struct add_rvalue_reference<T&>                  { typedef T& type;                  };
    template <>           struct add_rvalue_reference<void>                { typedef void type;                };
    template <>           struct add_rvalue_reference<const void>          { typedef const void type;          };
    template <>           struct add_rvalue_reference<volatile void>       { typedef volatile void type;       };
    template <>           struct add_rvalue_reference<const volatile void> { typedef const volatile void type; };

    #if KSL_TEMPLATES_ALIASES_ENABLED
        template <typename T>
        using add_rvalue_reference_t = typename add_rvalue_reference<T>::type;
    #endif


	template <bool bCondition, class ConditionIsTrueType, class ConditionIsFalseType>
	struct type_select { typedef ConditionIsTrueType type; };

	template <typename ConditionIsTrueType, class ConditionIsFalseType>
	struct type_select<false, ConditionIsTrueType, ConditionIsFalseType> { typedef ConditionIsFalseType type; };

	#if KSL_VARIABLE_TEMPLATES_ENABLED
		template <bool bCondition, class ConditionIsTrueType, class ConditionIsFalseType>
		using type_select_t = typename type_select<bCondition, ConditionIsTrueType, ConditionIsFalseType>::type;
	#endif

    template <typename T, typename = unused, typename = unused>
    struct first_type_select { typedef T type; };

	template <bool b1, bool b2, bool b3 = false, bool b4 = false, bool b5 = false>
	struct type_or;

	template <bool b1, bool b2, bool b3, bool b4, bool b5>
	struct type_or { static const bool value = true; };

	template <>
	struct type_or<false, false, false, false, false> { static const bool value = false; };


	template <bool b1, bool b2, bool b3 = true, bool b4 = true, bool b5 = true>
	struct type_and;

	template <bool b1, bool b2, bool b3, bool b4, bool b5>
	struct type_and{ static const bool value = false; };

	template <>
	struct type_and<true, true, true, true, true>{ static const bool value = true; };


	template <int b1, int b2>
	struct type_equal{ static const bool value = (b1 == b2); };


	template <int b1, int b2>
	struct type_not_equal{ static const bool value = (b1 != b2); };


	template <bool b>
	struct type_not{ static const bool value = true; };

	template <>
	struct type_not<true>{ static const bool value = false; };


	template <size_t I0, size_t ...in>
	struct static_min;

	template <size_t I0>
	struct static_min<I0> {
	    static const size_t value = I0;
	};

	template <size_t I0, size_t I1, size_t ...in>
	struct static_min<I0, I1, in...>
		{ static const size_t value = ((I0 <= I1) ? static_min<I0, in...>::value : static_min<I1, in...>::value); };

	template <size_t I0, size_t ...in>
	struct static_max;

	template <size_t I0>
	struct static_max<I0>
		{ static const size_t value = I0; };

	template <size_t I0, size_t I1, size_t ...in>
	struct static_max<I0, I1, in...>
		{ static const size_t value = ((I0 >= I1) ? static_max<I0, in...>::value : static_max<I1, in...>::value); };

	enum class endian {
		#ifdef KOB_SYSTEM_LITTLE_ENDIAN
			little = 1,
			big = 0,
			native = little
		#else
			little = 0,
			big = 1,
			native = big
		#endif
	};

    ///conditional

    template<bool B, typename T, typename F>
    struct conditional { typedef T type; };

    template <typename T, typename F>
    struct conditional<false, T, F> { typedef F type; };
    #if KSL_TEMPLATES_ALIASES_ENABLED
        template <bool B, class T, class F>
        using conditional_t = typename conditional<B, T, F>::type;
    #endif

    //conjunction
    template <class...>
    struct conjunction : true_type {};

    template <class B>
    struct conjunction<B> : B {};

    template <class B, class... Bn>
    struct conjunction<B, Bn...> : conditional<bool(B::value), conjunction<Bn...>, B>::type {};

    #if KSL_VARIABLE_TEMPLATES_ENABLED
        template<class... Bn>
            static const KOB_CONSTEXPR bool conjunction_v = conjunction<Bn...>::value;
    #endif

    ///disjunction
    template <class...>
    struct disjunction : false_type {};

    template <class B>
    struct disjunction<B> : B {};

    template <class B, class... Bn>
    struct disjunction<B, Bn...> : conditional<bool(B::value), B, disjunction<Bn...>>::type {};

    #if KSL_VARIABLE_TEMPLATES_ENABLED
        template<class... B>
        static const KOB_CONSTEXPR bool disjunction_v = disjunction<B...>::value;
    #endif

    template <class B>
    struct negation : bool_constant<!bool(B::value)> {};

    #if KSL_VARIABLE_TEMPLATES_ENABLED
        template<class B>
        static const KOB_CONSTEXPR bool negation_v = negation<B>::value;
    #endif

    template <typename T>
    struct identity { using type = T; };


    template <typename T, typename U>
    struct is_same : public false_type { };

    template <typename T>
    struct is_same<T, T> : public true_type { };

    #if KSL_VARIABLE_TEMPLATES_ENABLED
        template <class T, class U>
            KOB_CONSTEXPR bool is_same_v = is_same<T, U>::value;
    #endif

    template <typename T>
	typename std::add_rvalue_reference<T>::type declval() KOB_NOEXCEPT;


    //enable_if/disable_if
    template<bool B, typename T = void>
    struct enable_if {};

    template <typename T>
    struct enable_if<true, T> {
        typedef T type;
    };

    #if KSL_TEMPLATES_ALIASES_ENABLED
        template <bool B, class T = void>
        using enable_if_t = typename enable_if<B, T>::type;
    #endif

    template<bool B, typename T = void>
    struct disable_if {};

    template <typename T>
    struct disable_if<false, T> {
        typedef T type;
    };
    #if KSL_TEMPLATES_ALIASES_ENABLED
        template <bool B, class T = void>
        using disable_if_t = typename disable_if<B, T>::type;
    #endif



} // namespace ksl

#include <kob/ksl/detail/type_fundamental.h>
#include <kob/ksl/detail/type_transformations.h>
#include <kob/ksl/detail/type_properties.h>
#include <kob/ksl/detail/type_compound.h>
#include <kob/ksl/detail/type_pod.h>
#endif //KOB_KSL_TYPE_TRAITS_H_
