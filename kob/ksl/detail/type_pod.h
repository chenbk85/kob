//
// Created by 李寅斌 on 2018/12/11.
//

#ifndef KOB_KSL_DETAIL_TYPE_POD_H_
#define KOB_KSL_DETAIL_TYPE_POD_H_
#include <kob/ksl/detail/config.h>

namespace ksl {
    //is_empty
    template <typename T>
    struct is_empty : public integral_constant<bool, __is_empty(T)>{};

    //is_pod
    template <typename T>
    struct is_pod : public integral_constant<bool, __is_pod(T) || is_void<T>::value || is_scalar<T>::value>{};

    template <typename T, size_t N>
    struct is_pod<T[N]> : public is_pod<T>{};

    #if KSL_VARIABLE_TEMPLATES_ENABLED
        template <class T>
        KOB_CONSTEXPR bool is_pod_v = is_pod<T>::value;
    #endif

    //is_standard_layout
    template <typename T>
    struct is_standard_layout : public integral_constant<bool, __is_standard_layout(T) || is_void<T>::value || is_scalar<T>::value>{};

    //has_trivial_constructor
    template <typename T>
    struct has_trivial_constructor : public integral_constant<bool, (__has_trivial_constructor(T) || is_pod<T>::value) && !is_hat_type<T>::value>{};

    //has_trivial_copy
    template <typename T>
    struct has_trivial_copy : public integral_constant<bool, (__has_trivial_copy(T) || is_pod<T>::value) && (!is_volatile<T>::value && !is_reference<T>::value)>{};

    //has_trivial_assign
    template <typename T>
    struct has_trivial_assign : public integral_constant<bool, (__has_trivial_assign(T) || is_pod<T>::value) && !is_const<T>::value && !is_volatile<T>::value>{};

    //has_trivial_destructor
    template <typename T>
    struct has_trivial_destructor : public integral_constant<bool, __has_trivial_destructor(T) || is_pod<T>::value>{};

    //has_trivial_relocate
    template <typename T>
struct has_trivial_relocate : public bool_constant<is_pod<T>::value && !is_volatile<T>::value> {};

    //has_nothrow_constructor
    template <typename T>
    struct has_nothrow_constructor
            : public integral_constant<bool, __has_nothrow_constructor(T)>{};

    //has_nothrow_copy
    template <typename T>
    struct has_nothrow_copy : public integral_constant<bool, __has_nothrow_copy(T)>{};

    //has_nothrow_assign
    template <typename T>
    struct has_nothrow_assign : public integral_constant<bool, __has_nothrow_assign(T)>{};

    //has_virtual_destructor
    template <typename T>
    struct has_virtual_destructor : public integral_constant<bool, __has_virtual_destructor(T)>{};

    //is_literal_type
    template <typename T>
    struct is_literal_type : public integral_constant<bool, __is_literal_type(T)>{};

    #if KSL_VARIABLE_TEMPLATES_ENABLED
        template <class T>
        KOB_CONSTEXPR bool is_literal_type_v = is_literal_type<T>::value;
    #endif

    //is_abstract
    template <typename T>
    struct is_abstract : public integral_constant<bool, __is_abstract(T)>{};

    //is_trivially_copyable
    template <typename T>
    struct is_trivially_copyable : public integral_constant<bool, (__has_trivial_copy(T) || is_pod<typename remove_all_extents<T>::type>::value) && (!is_void<T>::value && !is_volatile<T>::value && !is_reference<T>::value)>{};
    #if KSL_VARIABLE_TEMPLATES_ENABLED
        template <class T>
        KOB_CONSTEXPR bool is_trivially_copyable_v = is_trivially_copyable<T>::value;
    #endif

    //is_constructible
    template <typename T>
    KOB_FORCE_INLINE typename remove_reference<T>::type&& move_internal(T&& x) KOB_NOEXCEPT
    {
        return ((typename remove_reference<T>::type&&)x);
    }

    template <typename T, class ...Args>
    inline typename first_type_select<true_type, decltype(move_internal(T(declval<Args>()...)))>::type is(T&&, Args&& ...);

    template <typename T>
    struct can_construct_scalar_helper {
        static true_type can(T);
        static false_type can(...);
    };

    template <typename ...Args>
    false_type is(argument_sink, Args&& ...);

    // Except for scalars and references (handled below), check for constructibility via decltype.
    template <bool, typename T, typename... Args>
    struct is_constructible_helper_2    // argument_sink will catch all T that is not constructible from the Args and denote false_type
            : public identity<decltype(is(declval<T>(), declval<Args>()...))>::type {};

    template <typename T>
    struct is_constructible_helper_2<true, T>
            : public is_scalar<T> {};

    template <typename T, typename Arg0> // We handle the case of multiple arguments below (by disallowing them).
    struct is_constructible_helper_2<true, T, Arg0>
            : public identity<decltype(can_construct_scalar_helper<T>::can(declval<Arg0>()))>::type {};

    // Scalars and references can be constructed only with 0 or 1 argument. e.g the following is an invalid expression: int(17, 23)
    template <typename T, typename Arg0, typename ...Args>
    struct is_constructible_helper_2<true, T, Arg0, Args...>
            : public false_type {};

    template <bool, typename T, typename... Args>
    struct is_constructible_helper_1
            : public is_constructible_helper_2<is_scalar<T>::value || is_reference<T>::value, T, Args...> {};

    // Unilaterally dismiss void, abstract, unknown bound arrays, and function types as not constructible.
    template <typename T, typename... Args>
    struct is_constructible_helper_1<true, T, Args...>
            : public false_type {};

    // is_constructible
    template <typename T, typename... Args>
    struct is_constructible
            : public is_constructible_helper_1<(is_abstract<typename remove_all_extents<T>::type>::value ||
                                                is_array_of_unknown_bounds<T>::value                            ||
                                                is_function<typename remove_all_extents<T>::type>::value ||
                                                has_void_arg<T, Args...>::value),
                    T, Args...> {};

    // Array types are constructible if constructed with no arguments and if their element type is default-constructible
    template <typename Array, size_t N>
    struct is_constructible_helper_2<false, Array[N]>
            : public is_constructible<typename remove_all_extents<Array>::type> {};

    // Arrays with arguments are not constructible. e.g. the following is an invalid expression: int[3](37, 34, 12)
    template <typename Array, size_t N, typename ...Args>
    struct is_constructible_helper_2<false, Array[N], Args...>
            : public false_type {};

    #if KSL_VARIABLE_TEMPLATES_ENABLED
        template <class T, class... Args>
        KOB_CONSTEXPR bool is_constructible_v = is_constructible<T, Args...>::value;
    #endif

    //is_trivially_constructible
    template <typename T, typename... Args>
    struct is_trivially_constructible
            : public false_type {};

    template <typename T>
    struct is_trivially_constructible<T>
            : public integral_constant<bool, is_constructible<T>::value && has_trivial_constructor<typename remove_all_extents<T>::type>::value> {};

    // It's questionable whether we can use has_trivial_copy here, as it could theoretically create a false-positive.
    template <typename T>
    struct is_trivially_constructible<T, T>
            : public integral_constant<bool, is_constructible<T>::value && has_trivial_copy<T>::value> {};

    template <typename T>
    struct is_trivially_constructible<T, T&&>
            : public integral_constant<bool, is_constructible<T>::value && has_trivial_copy<T>::value> {};

    template <typename T>
    struct is_trivially_constructible<T, T&>
            : public integral_constant<bool, is_constructible<T>::value && has_trivial_copy<T>::value> {};

    template <typename T>
    struct is_trivially_constructible<T, const T&>
            : public integral_constant<bool, is_constructible<T>::value && has_trivial_copy<T>::value> {};

    template <typename T>
    struct is_trivially_constructible<T, volatile T&>
            : public integral_constant<bool, is_constructible<T>::value && has_trivial_copy<T>::value> {};

    template <typename T>
    struct is_trivially_constructible<T, const volatile T&>
            : public integral_constant<bool, is_constructible<T>::value && has_trivial_copy<T>::value> {};

    #if KSL_VARIABLE_TEMPLATES_ENABLED
        template <class T>
        KOB_CONSTEXPR bool is_trivially_constructible_v = is_trivially_constructible<T>::value;
    #endif

    //is_trivially_default_constructible
    template <typename T>
    struct is_trivially_default_constructible
            : public is_trivially_constructible<T> {};

    #if KSL_VARIABLE_TEMPLATES_ENABLED
        template <class T>
        KOB_CONSTEXPR bool is_trivially_default_constructible_v = is_trivially_default_constructible<T>::value;
    #endif

    //is_trivial
    template <typename T>
    struct is_trivial
            : public integral_constant<bool, is_trivially_copyable<T>::value && is_trivially_default_constructible<T>::value> {};

    #if KSL_VARIABLE_TEMPLATES_ENABLED
        template <class T>
        KOB_CONSTEXPR bool is_trivial_v = is_trivial<T>::value;
    #endif

    //is_nothrow_constructible
    
    template <typename T, typename... Args>
    struct is_nothrow_constructible_helper_noexcept_wrapper
    { static const bool value = noexcept(T(declval<Args>()...)); };
    
    template <bool, typename T, typename... Args>
    struct is_nothrow_constructible_helper;
    
    template <typename T, typename... Args>
    struct is_nothrow_constructible_helper<true, T, Args...>
            : public integral_constant<bool, is_nothrow_constructible_helper_noexcept_wrapper<T, Args...>::value> {};
    
    template<typename T, typename Arg>
    struct is_nothrow_constructible_helper<true, T, Arg>
            : public integral_constant<bool, noexcept(T(declval<Arg>()))> {};
    
    template<typename T>
    struct is_nothrow_constructible_helper<true, T>
            : public integral_constant<bool, noexcept(T())> {};
    
    template <typename T, typename... Args>
    struct is_nothrow_constructible_helper<false, T, Args...>
            : public false_type {};
    
    template <typename T, typename... Args>
    struct is_nothrow_constructible
            : public is_nothrow_constructible_helper<is_constructible<T, Args...>::value, T, Args...> {};
    
    template <typename T, size_t N>
    struct is_nothrow_constructible<T[N]>
            : public is_nothrow_constructible_helper<is_constructible<T>::value, T> {};
    

    #if KSL_VARIABLE_TEMPLATES_ENABLED
        template <class T, typename... Args>
        KOB_CONSTEXPR bool is_nothrow_constructible_v = is_nothrow_constructible<T, Args...>::value;
    #endif
    
    //is_default_constructible
    template <typename T>
    struct is_default_constructible
            : public is_constructible<T> {};

    #if KSL_VARIABLE_TEMPLATES_ENABLED
        template <class T>
        KOB_CONSTEXPR bool is_default_constructible_v = is_default_constructible<T>::value;
    #endif

    //is_copy_constructible
    template <typename T>
    struct is_copy_constructible
            : public is_constructible<T, typename add_lvalue_reference<typename add_const<T>::type>::type> {};

    #if KSL_VARIABLE_TEMPLATES_ENABLED
        template <class T>
        KOB_CONSTEXPR bool is_copy_constructible_v = is_copy_constructible<T>::value;
    #endif

    //is_trivially_copy_constructible
    template <typename T>
    struct is_trivially_copy_constructible
            : public is_trivially_constructible<T, typename add_lvalue_reference<typename add_const<T>::type>::type> {};

    #if KSL_VARIABLE_TEMPLATES_ENABLED
        template <class T>
        KOB_CONSTEXPR bool is_trivially_copy_constructible_v = is_trivially_copy_constructible<T>::value;
    #endif

    //is_nothrow_copy_constructible
    template <typename T>
    struct is_nothrow_copy_constructible
            : public is_nothrow_constructible<T, typename add_lvalue_reference<typename add_const<T>::type>::type> {};

    #if KSL_VARIABLE_TEMPLATES_ENABLED
        template <class T>
        KOB_CONSTEXPR bool is_nothrow_copy_constructible_v = is_nothrow_copy_constructible<T>::value;
    #endif

    //is_move_constructible
    template <typename T>
    struct is_move_constructible
            : public is_constructible<T, typename add_rvalue_reference<T>::type> {};

    #if KSL_VARIABLE_TEMPLATES_ENABLED
        template <class T>
        KOB_CONSTEXPR bool is_move_constructible_v = is_nothrow_copy_constructible<T>::value;
    #endif

    //is_trivially_move_constructible
    template <typename T>
    struct is_trivially_move_constructible
            : public is_trivially_constructible<T, typename add_rvalue_reference<T>::type> {};

    #if KSL_VARIABLE_TEMPLATES_ENABLED
        template <class T>
        KOB_CONSTEXPR bool is_trivially_move_constructible_v = is_trivially_move_constructible<T>::value;
    #endif

    //is_assignable
    template<typename T, typename U>
    struct is_assignable_helper {
        template<typename, typename>
        static no_type is(...);

        template<typename T1, typename U1>
        static decltype(declval<T1>() = declval<U1>(), yes_type()) is(int);

        static const bool value = (sizeof(is<T, U>(0)) == sizeof(yes_type));
    };

    template<typename T, typename U>
    struct is_assignable :
            public integral_constant<bool, is_assignable_helper<T, U>::value> {};

    #if KSL_VARIABLE_TEMPLATES_ENABLED
        template <class T, class U>
        KOB_CONSTEXPR bool is_assignable_v = is_assignable<T, U>::value;
    #endif

    //is_lvalue_assignable
    template <typename T, typename U>
    struct is_lvalue_assignable
            : public is_assignable<typename add_lvalue_reference<T>::type,
                    typename add_lvalue_reference<typename add_const<U>::type>::type> {};

    //is_trivially_assignable
    template <typename T, typename U>
    struct is_trivially_assignable
            : integral_constant<bool, is_assignable<T, U>::value &&
                                             (is_pod<typename remove_reference<T>::type>::value || __has_trivial_assign(typename remove_reference<T>::type))> {};

    #if KSL_VARIABLE_TEMPLATES_ENABLED
        template <class T, class U>
        KOB_CONSTEXPR bool is_trivially_assignable_v = is_trivially_assignable<T, U>::value;
    #endif

    //is_nothrow_assignable
    template <bool, typename T, typename U>
    struct is_nothrow_assignable_helper;

    template <typename T, typename U>
    struct is_nothrow_assignable_helper<false, T, U>
            : public false_type {};
    
    template <typename T, typename U>
    struct is_nothrow_assignable_helper<true, T, U> // Set to true if the assignment (same as is_assignable) cannot generate an exception.
            : public integral_constant<bool, noexcept(declval<T>() = declval<U>()) >
    {
    };
    
    template <typename T, typename U>
    struct is_nothrow_assignable
            : public is_nothrow_assignable_helper<is_assignable<T, U>::value, T, U>
    {
    };

    #if KSL_VARIABLE_TEMPLATES_ENABLED
        template <class T, class U>
        KOB_CONSTEXPR bool is_nothrow_assignable_v = is_nothrow_assignable<T, U>::value;
    #endif

    //is_copy_assignable
    template <typename T>
    struct is_copy_assignable
            : public is_assignable<typename add_lvalue_reference<T>::type,
                    typename add_lvalue_reference<typename add_const<T>::type>::type> {};

    #if KSL_VARIABLE_TEMPLATES_ENABLED
        template <class T>
        KOB_CONSTEXPR bool is_copy_assignable_v = is_copy_assignable<T>::value;
    #endif

    //is_trivially_copy_assignable
    template <typename T>
    struct is_trivially_copy_assignable
            : public is_trivially_assignable<typename add_lvalue_reference<T>::type,
                    typename add_lvalue_reference<typename add_const<T>::type>::type> {};

    #if KSL_VARIABLE_TEMPLATES_ENABLED
        template <class T>
        KOB_CONSTEXPR bool is_trivially_copy_assignable_v = is_trivially_copy_assignable<T>::value;
    #endif
    //is_nothrow_copy_assignable
    template <typename T>
    struct is_nothrow_copy_assignable
            : public is_nothrow_assignable<typename add_lvalue_reference<T>::type,
                    typename add_lvalue_reference<typename add_const<T>::type>::type> {};

    #if KSL_VARIABLE_TEMPLATES_ENABLED
        template <class T>
        KOB_CONSTEXPR bool is_nothrow_copy_assignable_v = is_nothrow_copy_assignable<T>::value;
    #endif

    //is_move_assignable
    template <typename T>
    struct is_move_assignable
            : public is_assignable<typename add_lvalue_reference<T>::type,
                    typename add_rvalue_reference<T>::type> {};
    #if KSL_VARIABLE_TEMPLATES_ENABLED
        template <class T>
        KOB_CONSTEXPR bool is_move_assignable_v = is_move_assignable<T>::value;
    #endif

    //is_trivially_move_assignable
    template <typename T>
    struct is_trivially_move_assignable
            : public is_trivially_assignable<typename add_lvalue_reference<T>::type,
                    typename add_rvalue_reference<T>::type> {};

    //is_nothrow_move_assignable
    template <typename T>
    struct is_nothrow_move_assignable
            : public is_nothrow_assignable<typename add_lvalue_reference<T>::type,
                    typename add_rvalue_reference<T>::type> {};

    //is_destructible
    template <typename U>
    struct destructible_test_helper{ U u; };

    template <typename>
    false_type destructible_test_function(...);
    
    template <typename T, typename U = decltype(declval<destructible_test_helper<T> >().~destructible_test_helper<T>())>
    true_type destructible_test_function(int);
    
    template <typename T, bool = is_array_of_unknown_bounds<T>::value || // Exclude these types from being considered destructible.
                                 is_void<T>::value                    ||
                                 is_function<T>::value                ||
                                 is_abstract<T>::value>
    struct is_destructible_helper
            : public identity<decltype(destructible_test_function<T>(0))>::type {}; // Need to wrap decltype with identity because some compilers otherwise don't like the bare decltype usage.
    
    template <typename T>
    struct is_destructible_helper<T, true>
            : public false_type {};
    
    template <typename T>
    struct is_destructible
            : public is_destructible_helper<T> {};

    //is_trivially_destructible
    template <typename T>
    struct is_trivially_destructible // Can't use just __has_trivial_destructor(T) because some compilers give it slightly different meaning, and are just plain broken, such as VC++'s __has_trivial_destructor, which says false for fundamental types.
            : public integral_constant<bool, is_destructible<T>::value && ((__has_trivial_destructor(T) && !is_hat_type<T>::value)|| is_scalar<typename remove_all_extents<T>::type>::value)> {};

    #if KSL_VARIABLE_TEMPLATES_ENABLED
        template <class T>
        KOB_CONSTEXPR bool is_trivially_destructible_v = is_trivially_destructible<T>::value;
    #endif

    //is_nothrow_destructible
    template <typename T>
    struct is_nothrow_destructible_helper_noexcept_wrapper
    { static const bool value = noexcept(declval<T&>().~T()); };

    template <typename T, bool>
    struct is_nothrow_destructible_helper;
    
    template <typename T>
    struct is_nothrow_destructible_helper<T, false>
            : public false_type {};
    
    template <typename T>
    struct is_nothrow_destructible_helper<T, true>     // If the expression T::~T is a noexcept expression then it's nothrow.
            : public integral_constant<bool, is_nothrow_destructible_helper_noexcept_wrapper<T>::value > {};
    
    template <typename T>
    struct is_nothrow_destructible                      // A type needs to at least be destructible before it could be nothrow destructible.
            : public is_nothrow_destructible_helper<T, is_destructible<T>::value> {};
    
    template <typename T, size_t N>                     // An array is nothrow destructible if its element type is nothrow destructible.
    struct is_nothrow_destructible<T[N]>                // To consider: Replace this with a remove_all_extents pathway.
            : public is_nothrow_destructible<T> {};
    
    template <typename T>
    struct is_nothrow_destructible<T&>                  // A reference type cannot throw while being destructed. It's just a reference.
            : public true_type {};
    
    template <typename T>
    struct is_nothrow_destructible<T&&>                 // An rvalue reference type cannot throw while being destructed.
            : public true_type {};
    
    //is_nothrow_default_constructible
    template <typename T>
    struct is_nothrow_default_constructible
            : public is_nothrow_constructible<T> {};

    //is_nothrow_move_constructible
    template <typename T>
    struct is_nothrow_move_constructible
            : public is_nothrow_constructible<T, typename add_rvalue_reference<T>::type> {};


}
#endif //KOB_KSL_DETAIL_TYPE_POD_H_
