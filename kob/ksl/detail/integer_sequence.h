//
// Created by 李寅斌 on 2018/12/6.
//

#ifndef KOB_KSL_DETAIL_INTEGER_SEQUENCE_H_
#define KOB_KSL_DETAIL_INTEGER_SEQUENCE_H_
#include <kob/ksl/detail/config.h>
#include <kob/ksl/type_traits.h>

namespace ksl {

template <typename T, T... Ints>
struct integer_sequence {
    typedef T value_type;
    KOB_STATIC_ASSERT_MSG(std::is_integral<T>::value,"ksl integer_sequence can be instantiated only integer type");
    static KOB_FUNC_CONSTEXPR size_t size() KOB_NOEXCEPT
    {
        return sizeof ...(Ints);
    }
};

template <size_t N, typename IndexSeq>
struct make_index_sequence_impl;

template <size_t N, size_t... Is>
struct make_index_sequence_impl<N, integer_sequence<size_t, Is...>> {
    typedef typename make_index_sequence_impl<N - 1, integer_sequence<size_t, N - 1, Is...>>::type type;
};

template <size_t... Is>
struct make_index_sequence_impl<0, integer_sequence<size_t, Is...>> {
    typedef integer_sequence<size_t, Is...> type;
};

template <size_t... Is>
using index_sequence = integer_sequence<size_t, Is...>;

template <size_t N>
using make_index_sequence = typename make_index_sequence_impl<N, integer_sequence<size_t>>::type;

template <typename Target, typename Seq>
struct integer_sequence_convert_impl;

template <typename Target, size_t... Is>
struct integer_sequence_convert_impl<Target, integer_sequence<size_t, Is...>> {
    typedef integer_sequence<Target, Is...> type;
};

template <typename T, size_t N>
struct make_integer_sequence_impl {
    typedef typename integer_sequence_convert_impl<T, make_index_sequence<N>>::type type;
};

template <typename T, size_t N>
using make_integer_sequence = typename make_integer_sequence_impl<T, N>::type;

template<typename... T>
using index_sequence_for = make_index_sequence<sizeof...(T)>;


} //namespace ksl
#endif //KOB_KSL_DETAIL_INTEGER_SEQUENCE_H_
