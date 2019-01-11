//
// Created by 李寅斌 on 2018/12/1.
//

#ifndef KOB_KSL_RATIO_H_
#define KOB_KSL_RATIO_H_
#include <cstdint>
#include <kob/ksl/type_traits.h>

namespace ksl {

template <intmax_t X, intmax_t Y>
struct addition_overflow  {
    static const bool c1 = (X <= 0 && 0 <= Y) || (Y < 0 && 0 < X);   // True if digits do not have the same sign.
    static const bool c2 = abs(Y) <= INTMAX_MAX - abs(X);
    static const bool value = c1 || c2;
};

template <intmax_t X, intmax_t Y>
struct multiply_overflow {
    static const bool value = (abs(X) <= (INTMAX_MAX / abs(Y)));
};

template <intmax_t N = 0, intmax_t D = 1>
class ratio {
public:
    static KOB_CONSTEXPR intmax_t num = N;
    static KOB_CONSTEXPR intmax_t den = D;
    typedef ratio<num, den> type;
};

template <intmax_t N, intmax_t D>
KOB_CONSTEXPR intmax_t ratio<N, D>::num;

template <intmax_t N, intmax_t D>
KOB_CONSTEXPR intmax_t ratio<N, D>::den;

namespace detail {
    template <intmax_t X, intmax_t Y>
    struct gcd {
        static const intmax_t value = gcd<Y, X % Y>::value;
    };
    template <intmax_t X>
    struct gcd<X, 0> {
        static const intmax_t value = X;
    };
    template <>
    struct gcd<0, 0> {
        static const intmax_t value = 1;
    };

    // lcm
    template<intmax_t X, intmax_t Y>
    struct lcm {
        static const intmax_t value = (X * (Y / gcd<X,Y>::value));
    };

    // ct_add
    template <intmax_t X, intmax_t Y>
    struct ct_add {
        static_assert(addition_overflow<X,Y>::value, "compile-time addition overflow");
        static const intmax_t value = X + Y;
    };

    // ct_sub
    template <intmax_t X, intmax_t Y>
    struct ct_sub {
        static_assert(addition_overflow<X,-Y>::value, "compile-time addition overflow");
        static const intmax_t value = X - Y;
    };

    // ct_multi
    template <intmax_t X, intmax_t Y>
    struct ct_multi {
        static_assert(multiply_overflow<X,Y>::value, "compile-time multiply overflow");
        static const intmax_t value = X * Y;
    };

    // ct_simplify
    template <class R1>
    struct ct_simplify {
        static const intmax_t divisor = detail::gcd<R1::num, R1::den>::value;
        static const intmax_t num = R1::num / divisor;
        static const intmax_t den = R1::den / divisor;

        typedef ratio<num, den> ratio_type;
        typedef ct_simplify<R1> this_type;
    };

#if KOB_COMPILER_NO_VARIABLE_TEMPLATES
        template <intmax_t N1, intmax_t N2> struct ct_add_v   : public ct_add<N1, N2>::value {};
        template <intmax_t N1, intmax_t N2> struct ct_multi_v : public ct_multi<N1, N2>::value {};
        template <class R1> struct ct_simplify_t 			  : public ct_simplify<R1>::ratio_type {};
#else
        template <intmax_t N1, intmax_t N2> intmax_t ct_add_v   = ct_add<N1, N2>::value;
        template <intmax_t N1, intmax_t N2> intmax_t ct_multi_v = ct_multi<N1, N2>::value;
        template <class R1, class R2> R2 ct_simplify_t          = ct_simplify<R1>::ratio_type;
#endif

    template <class R1, class R2>
    struct ratio_add {
        typedef typename ct_simplify
                <
                        typename ratio
                                <
                                        ct_add
                                                <
                                                        ct_multi<R1::num, R2::den>::value,
                                                        ct_multi<R2::num, R1::den>::value
                                                >::value,
                                        ct_multi<R1::den, R2::den>::value
                                >::type
                >::ratio_type type;
    };

    template <class R1, class R2>
    struct ratio_subtract {
        typedef typename ct_simplify
                <
                        typename ratio
                                <
                                        ct_sub
                                                <
                                                        ct_multi<R1::num, R2::den>::value,
                                                        ct_multi<R2::num, R1::den>::value
                                                >::value,
                                        ct_multi<R1::den, R2::den>::value
                                >::type
                >::ratio_type type;
    };

    template <class R1, class R2>
    struct ratio_multiply
    {
        typedef typename ct_simplify
                <
                        typename ratio
                                <
                                        ct_multi<R1::num, R2::num>::value,
                                        ct_multi<R1::den, R2::den>::value
                                >::type
                >::ratio_type type;
    };

    template <class R1, class R2>
    struct ratio_divide {
        typedef typename ct_simplify
                <
                        typename ratio
                                <
                                        ct_multi<R1::num, R2::den>::value,
                                        ct_multi<R1::den, R2::num>::value
                                >::type
                >::ratio_type type;
    };

    template <class R1, class R2>
    struct ratio_equal {
        typedef ct_simplify<R1> sr1_t;
        typedef ct_simplify<R2> sr2_t;

        static const bool value = (sr1_t::num == sr2_t::num) && (sr1_t::den == sr2_t::den);
    };

    template <class R1, class R2>
    struct ratio_less {
        static const bool value = (R1::num * R2::den) < (R2::num * R1::den);
    };
}

#if KOB_COMPILER_NO_TEMPLATE_ALIASES 
    template <class R1, class R2> struct ratio_add      : public detail::ratio_add<R1, R2>::type {};
    template <class R1, class R2> struct ratio_subtract : public detail::ratio_subtract<R1, R2>::type {};
    template <class R1, class R2> struct ratio_multiply : public detail::ratio_multiply<R1, R2>::type {};
    template <class R1, class R2> struct ratio_divide   : public detail::ratio_divide<R1, R2>::type {};
#else
    template <class R1, class R2> using ratio_add      = typename detail::ratio_add<R1, R2>::type;
    template <class R1, class R2> using ratio_subtract = typename detail::ratio_subtract<R1, R2>::type;
    template <class R1, class R2> using ratio_multiply = typename detail::ratio_multiply<R1, R2>::type;
    template <class R1, class R2> using ratio_divide   = typename detail::ratio_divide<R1, R2>::type;
#endif


template <class R1, class R2> struct ratio_equal         : public std::integral_constant<bool, detail::ratio_equal<R1, R2>::value> {};
template <class R1, class R2> struct ratio_not_equal     : public std::integral_constant<bool, !ratio_equal<R1, R2>::value> {};
template <class R1, class R2> struct ratio_less          : public std::integral_constant<bool, detail::ratio_less<R1, R2>::value> {};
template <class R1, class R2> struct ratio_less_equal    : public std::integral_constant<bool, !ratio_less<R2, R1>::value> {};
template <class R1, class R2> struct ratio_greater       : public std::integral_constant<bool, ratio_less<R2, R1>::value> {};
template <class R1, class R2> struct ratio_greater_equal : public std::integral_constant<bool, !ratio_less<R1, R2>::value> {};


typedef ratio<1,       1000000000000000000> atto;
typedef ratio<1,          1000000000000000> femto;
typedef ratio<1,             1000000000000> pico;
typedef ratio<1,                1000000000> nano;
typedef ratio<1,                   1000000> micro;
typedef ratio<1,                      1000> milli;
typedef ratio<1,                       100> centi;
typedef ratio<1,                        10> deci;
typedef ratio<10,                        1> deca;
typedef ratio<100,                       1> hecto;
typedef ratio<1000,                      1> kilo;
typedef ratio<1000000,                   1> mega;
typedef ratio<1000000000,                1> giga;
typedef ratio<1000000000000,             1> tera;
typedef ratio<1000000000000000,          1> peta;
typedef ratio<1000000000000000000,       1> exa;


} //namespace ksl
#endif //KOB_KSL_RATIO_H_
