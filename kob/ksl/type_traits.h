#ifndef KOB_KSL_TYPE_TRAITS_H_
#define KOB_KSL_TYPE_TRAITS_H_



#include <kob/ksl/detail/config.h>
#include <stddef.h>
#include <type_traits>

namespace ksl {


	template <bool bCondition, class ConditionIsTrueType, class ConditionIsFalseType>
	struct type_select { typedef ConditionIsTrueType type; };

	template <typename ConditionIsTrueType, class ConditionIsFalseType>
	struct type_select<false, ConditionIsTrueType, ConditionIsFalseType> { typedef ConditionIsFalseType type; };

	#if KOB_COMPILER_NO_VARIABLE_TEMPLATES == 0
		template <bool bCondition, class ConditionIsTrueType, class ConditionIsFalseType>
		using type_select_t = typename type_select<bCondition, ConditionIsTrueType, ConditionIsFalseType>::type;
	#endif


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
	struct static_min<I0>
		{ static const size_t value = I0; };

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

	template <typename T>
	typename std::add_rvalue_reference<T>::type declval() KOB_NOEXCEPT;

    template<typename T>
    struct decay {
        typedef typename std::remove_reference<T>::type U;

        typedef typename std::conditional<
                std::is_array<U>::value,
                typename std::remove_extent<U>::type*,
                typename std::conditional<
                        std::is_function<U>::value,
                        typename std::add_pointer<U>::type,
                        typename std::remove_cv<U>::type
                >::type
        >::type type;
    };

    template<typename T>
    using decay_t = typename decay<T>::type;


} // namespace ksl


#endif //KOB_KSL_TYPE_TRAITS_H_
