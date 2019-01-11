//
// Created by 李寅斌 on 2018/11/12.
//

#ifndef KOB_STRINGS_NUMERIC_FORMATTER_H_
#define KOB_STRINGS_NUMERIC_FORMATTER_H_
#include <kob/strings/double-conversion/numeric_string.h>
#include <kob/profile/prof.h>
#include <kob/ksl/type_traits.h>
#include <cstddef>
#include <string>

namespace kob {

namespace strings {

template <typename IntType>
std::string format(IntType v)
{
    KOB_STATIC_ASSERT_MSG(std::is_integral<IntType>::value, "not integral");
    std::string result;
    int_to_str(v, 10, result);
    return result;
}

template <typename IntType>
std::string format(IntType v, size_t width, char padding = ' ')
{
    KOB_STATIC_ASSERT_MSG(std::is_integral<IntType>::value, "not integral");
    std::string result;
    int_to_str(v, 10, result, false, padding);
    return result;
}

template <typename IntType>
std::string format_hex(IntType v, bool prefix)
{
    KOB_STATIC_ASSERT_MSG(std::is_integral<IntType>::value, "not integral");
    typename std::make_unsigned<IntType>::type vv =
            static_cast<typename std::make_unsigned<IntType>::type>(v);
    std::string result;
    uint_to_str(vv, 0x10, result, prefix);
    return result;
}

template <typename IntType>
std::string format_hex(IntType v, size_t width, bool prefix)
{
    KOB_STATIC_ASSERT_MSG(std::is_integral<IntType>::value, "not integral");
    typename std::make_unsigned<IntType>::type vv =
            static_cast<typename std::make_unsigned<IntType>::type>(v);
    std::string result;
    uint_to_str(vv, 0x10, result, prefix, width, 0);
    return result;
}

//uint

template <typename UIntType>
std::string format_uint(UIntType v)
{
    KOB_STATIC_ASSERT_MSG(std::is_integral<UIntType>::value, "not integral");
    KOB_STATIC_ASSERT_MSG(std::is_unsigned<UIntType>::value, "not unsigned");
    std::string result;
    uint_to_str(v, 10, result);
    return result;
}

template <typename UIntType>
std::string format_uint(UIntType v, size_t width, char padding = ' ')
{
    KOB_STATIC_ASSERT_MSG(std::is_integral<UIntType>::value, "not integral");
    KOB_STATIC_ASSERT_MSG(std::is_unsigned<UIntType>::value, "not unsigned");
    std::string result;
    uint_to_str(v, 10, result, false, padding);
    return result;
}


KOB_FORCE_INLINE std::string format(float v)
{
    std::string result;
    result.resize(constant_value::kMaxFLTStringLen);
    size_t rs = float_to_str(&result[0], constant_value::kMaxFLTStringLen, v);
    result.resize(rs);
    return result;
}

KOB_FORCE_INLINE std::string format(float v, size_t precision)
{
    std::string result;
    result.resize(constant_value::kMaxFLTStringLen);
    size_t rs = float_to_fixed_str(&result[0], constant_value::kMaxFLTStringLen, v, precision);
    result.resize(rs);
    return result;
}

KOB_FORCE_INLINE std::string format(float v, size_t width, size_t precision)
{
    std::string result;
    float_to_fixed_str(result, v, precision, width);
    return result;
}

KOB_FORCE_INLINE std::string format(double v)
{
    std::string result;
    result.resize(constant_value::kMaxFLTStringLen);
    size_t rs = double_to_str(&result[0], constant_value::kMaxFLTStringLen, v);
    result.resize(rs);
    return result;
}

KOB_FORCE_INLINE std::string format(double v, size_t precision)
{
    std::string result;
    result.resize(constant_value::kMaxFLTStringLen);
    size_t rs = double_to_fixed_str(&result[0], constant_value::kMaxFLTStringLen, v, precision);
    result.resize(rs);
    return result;
}

KOB_FORCE_INLINE std::string format(double v, size_t width, size_t precision)
{
    std::string result;
    double_to_fixed_str(result, v, precision, width);
    return result;
}

KOB_FORCE_INLINE std::string format(void *p, bool prefix = true)
{
    std::string result;
    result.resize(32);
    size_t s = ptr_to_str(&result[0],32, p, prefix);
    result.resize(s);
    return  result;
}

} //namespace strings
} //namespace kob
#endif //KOB_STRINGS_NUMERIC_FORMATTER_H_
