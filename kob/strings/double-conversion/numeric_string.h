//
// Created by 李寅斌 on 2018/11/12.
//

#ifndef KOB_STRING_DETAIL_NUMERIC_STRING_H_
#define KOB_STRING_DETAIL_NUMERIC_STRING_H_
#include <kob/ksl/string_view.h>
#include <kob/ksl/ascii.h>
#include <kob/profile/prof.h>
#include <cstddef>

namespace kob {
namespace strings {

class constant_value {
public:
    static const size_t         kMaxIntStringLen;
    static const size_t         kMaxFLTStringLen;
    static const char*          kFltInf;
    static const char*          kFltNan;
    static const char           kFltExp;
    static const char           kDecimalSeparator;
    static const char           kThousandSeparator;
    static const unsigned short kNumBaseOct;
    static const unsigned short kNumBaseDes;
    static const unsigned short kNumBaseHex;

};

KOB_FORCE_INLINE ksl::string_view get_inf_str()
{
    return constant_value::kFltInf;
}

KOB_FORCE_INLINE ksl::string_view get_nan_str()
{
    return constant_value::kFltNan;
}

KOB_FORCE_INLINE char decimal_separator()
{
    return constant_value::kDecimalSeparator;
}

KOB_FORCE_INLINE char thousand_separator()
{
    return constant_value::kThousandSeparator;
}

template <typename I>
bool str_to_int(ksl::string_view sv, I& result, short base = 10, char thSep = ',')
{
    sv.trim();
    ksl::string_view::const_iterator citr = sv.begin();

    if (sv.empty() || *citr == '\0') {
        return false;
    }

    short sign = 1;
    if ((base == 10) && (*citr == '-')) {
        if (std::numeric_limits<I>::min() >= 0) {
            return false;
        }
        sign = -1;
        ++citr;
    } else if (*citr == '+') {
        ++citr;
    }

    // parser states:
    const char STATE_SIGNIFICANT_DIGITS = 1;
    char state = 0;

    result = 0;
    I limitCheck = std::numeric_limits<I>::max() / base;
    for (; *citr != '\0'&&citr != sv.end(); ++citr) {
        switch (*citr) {
            case '0':
                if (state < STATE_SIGNIFICANT_DIGITS) break;

            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
                if (state < STATE_SIGNIFICANT_DIGITS)
                    state = STATE_SIGNIFICANT_DIGITS;
                if (result > limitCheck)
                    return false;
                result = result * base + (*citr - '0');

                break;

            case '8':
            case '9':
                if ((base == 10) || (base == 0x10)){
                    if (state < STATE_SIGNIFICANT_DIGITS)
                        state = STATE_SIGNIFICANT_DIGITS;
                    if (result > limitCheck)
                        return false;
                    result = result * base + (*citr - '0');
                }else {
                    return false;
                }

                break;

            case 'a': case 'b': case 'c': case 'd': case 'e': case 'f':
                if (base != 0x10)
                    return false;
                if (state < STATE_SIGNIFICANT_DIGITS)
                    state = STATE_SIGNIFICANT_DIGITS;
                if (result > limitCheck)
                    return false;
                result = result * base + (10 + *citr - 'a');

                break;

            case 'A': case 'B': case 'C': case 'D': case 'E': case 'F':
                if (base != 0x10)
                    return false;
                if (state < STATE_SIGNIFICANT_DIGITS)
                    state = STATE_SIGNIFICANT_DIGITS;
                if (result > limitCheck)
                    return false;
                result = result * base + (10 + *citr - 'A');

                break;

            case '.':
                if ((base == 10) && (thSep == '.'))
                    break;
                else
                    return false;

            case ',':
                if ((base == 10) && (thSep == ','))
                    break;
                else
                    return false;

            case ' ':
                if ((base == 10) && (thSep == ' '))
                    break;
                // fallthrough

            default:
                return false;
        }
    }

    if ((sign < 0) && (base == 10)) {
        result *= sign;
    }

    return true;
}

template <typename I>
bool str_to_int(const std::string& str, I& result, short base = 10, char thSep = ',')
{
    return str_to_int(str, result, base, thSep);
}

namespace Impl {

/// Utility char pointer wrapper class.
/// Class ensures increment/decrement remain within boundaries.
class Ptr {
public:
    Ptr(char* ptr, size_t offset): _beg(ptr), _cur(ptr), _end(ptr + offset)
    {
    }

    char*& operator ++ () // prefix
    {
        check(_cur + 1);
        return ++_cur;
    }

    char* operator ++ (int) // postfix
    {
        check(_cur + 1);
        char* tmp = _cur++;
        return tmp;
    }

    char*& operator -- () // prefix
    {
        check(_cur - 1);
        return --_cur;
    }

    char* operator -- (int) // postfix
    {
        check(_cur - 1);
        char* tmp = _cur--;
        return tmp;
    }

    char*& operator += (int incr)
    {
        check(_cur + incr);
        return _cur += incr;
    }

    char*& operator -= (int decr)
    {
        check(_cur - decr);
        return _cur -= decr;
    }

    operator char* () const
    {
        return _cur;
    }

    size_t span() const
    {
        return _end - _beg;
    }

private:
    void check(char* ptr)
    {
        if (ptr > _end) assert(false);
    }

    const char* _beg;
    char*       _cur;
    const char* _end;
};

} // namespace Impl

/// Converts integer to string. Numeric bases from binary to hexadecimal are supported.
/// If width is non-zero, it pads the return value with fill character to the specified width.
/// When padding is zero character ('0'), it is prepended to the number itself; all other
/// paddings are prepended to the formatted result with minus sign or base prefix included
/// If prefix is true and base is octal or hexadecimal, respective prefix ('0' for octal,
/// "0x" for hexadecimal) is prepended. For all other bases, prefix argument is ignored.
/// Formatted string has at least [width] total length.

template <typename T>
size_t int_to_str(T value,
              unsigned short base,
              char* result,
              const size_t& size,
              bool prefix = false,
              int width = -1,
              char fill = ' ',
              char thSep = 0)
{
    if (base < 2 || base > 0x10) {
        *result = '\0';
        return 0;
    }

    Impl::Ptr ptr(result, size);
    int thCount = 0;
    T tmpVal;
    do {
        tmpVal = value;
        value /= base;
        *ptr++ = "FEDCBA9876543210123456789ABCDEF"[15 + (tmpVal - value * base)];
        if (thSep && (base == 10) && (++thCount == 3)) {
            *ptr++ = thSep;
            thCount = 0;
        }
    } while (value);

    if ('0' == fill) {
        if (tmpVal < 0) --width;
        if (prefix && base == 010) --width;
        if (prefix && base == 0x10) width -= 2;
        while ((ptr - result) < width) {
            *ptr++ = fill;
        }
    }

    if (prefix && base == 010) {
        *ptr++ = '0';
    } else if (prefix && base == 0x10) {
        *ptr++ = 'x';
        *ptr++ = '0';
    }

    if (tmpVal < 0) {
        *ptr++ = '-';
    }

    if ('0' != fill) {
        while ((ptr - result) < width) {
            *ptr++ = fill;
        }
    }

    size_t length = ptr - result;
    KOB_ASSERT_MSG(length <= ptr.span(), "overflow");
    KOB_ASSERT_MSG((-1 == width) || (length >= size_t(width)), "width not enough");

    *ptr-- = '\0';

    char* ptrr = result;
    char tmp;
    while(ptrr < ptr) {
        tmp    = *ptr;
        *ptr--  = *ptrr;
        *ptrr++ = tmp;
    }

    return length;
}

/// Converts unsigned integer to string. Numeric bases from binary to hexadecimal are supported.
/// If width is non-zero, it pads the return value with fill character to the specified width.
/// When padding is zero character ('0'), it is prepended to the number itself; all other
/// paddings are prepended to the formatted result with minus sign or base prefix included
/// If prefix is true and base is octal or hexadecimal, respective prefix ('0' for octal,
/// "0x" for hexadecimal) is prepended. For all other bases, prefix argument is ignored.
/// Formatted string has at least [width] total length.
template <typename T>
size_t uint_to_str(T value,
               unsigned short base,
               char* result,
               const size_t& size,
               bool prefix = false,
               int width = -1,
               char fill = ' ',
               char thSep = 0)
{
    KOB_STATIC_ASSERT_MSG(std::is_integral<T>::value, "not integral");
    KOB_STATIC_ASSERT_MSG(std::is_unsigned<T>::value, "not unsigned");
    if (base < 2 || base > 0x10) {
        *result = '\0';
        return 0;
    }

    Impl::Ptr ptr(result, size);
    int thCount = 0;
    T tmpVal;
    do {
        tmpVal = value;
        value /= base;
        *ptr++ = "FEDCBA9876543210123456789ABCDEF"[15 + (tmpVal - value * base)];
        if (thSep && (base == 10) && (++thCount == 3)) {
            *ptr++ = thSep;
            thCount = 0;
        }
    } while (value);

    if ('0' == fill) {
        if (prefix && base == 010) --width;
        if (prefix && base == 0x10) width -= 2;
        while ((ptr - result) < width) *ptr++ = fill;
    }

    if (prefix && base == 010) {
        *ptr++ = '0';
    } else if (prefix && base == 0x10) {
        *ptr++ = 'x';
        *ptr++ = '0';
    }

    if ('0' != fill) {
        while ((ptr - result) < width) {
            *ptr++ = fill;
        }
    }

    size_t length = ptr - result;
    KOB_ASSERT_MSG(size <= ptr.span(), "over flow");
    KOB_ASSERT_MSG((-1 == width) || (length >= size_t(width)), "width not enough");
    *ptr-- = '\0';

    char* ptrr = result;
    char tmp;
    while(ptrr < ptr) {
        tmp    = *ptr;
        *ptr--  = *ptrr;
        *ptrr++ = tmp;
    }

    return length;
}

KOB_FORCE_INLINE size_t ptr_to_str(char *buffer, size_t length, void *ptr, bool prefix = true)
{
    static const char digitsHex[] = "0123456789ABCDEF";
    uintptr_t value = reinterpret_cast<uintptr_t>(ptr);
    uintptr_t i = value;
    char *e = buffer + (length - 1);
    char *p = buffer;
    if((e - p) < 2U) {
        return 0;
    }

    if (prefix) {
        p[0] = '0';
        p[1] = 'x';
        p = p + 2;
    }

    do {
        size_t lsd = i % 16;
        i /= 16;
        *p++ = digitsHex[lsd];
    } while (i != 0 && p != e);

    *p = '\0';
    return p - buffer;
}

template <typename T>
bool int_to_str(T number, unsigned short base, std::string& result, bool prefix = false, int width = -1, char fill = ' ', char thSep = 0)
{
    std::string::size_type os = result.size();
    result.resize(os + constant_value::kMaxIntStringLen);
    size_t ret = int_to_str(number, base, &result[os], constant_value::kMaxIntStringLen, prefix, width, fill, thSep);
    result.resize(os + ret);
    return ret != 0;
}

template <typename T>
bool uint_to_str (T number, unsigned short base, std::string& result, bool prefix = false, int width = -1, char fill = ' ', char thSep = 0)
{
    std::string::size_type os = result.size();
    std::string::size_type ns = os + constant_value::kMaxIntStringLen;
    result.resize(ns);
    bool ret = uint_to_str(number, base, &result[os], constant_value::kMaxIntStringLen, prefix, width, fill, thSep);
    result.resize(os + ret);
    return ret != 0;
}

size_t float_to_str(char* buffer,
                int bufferSize,
                float value,
                int lowDec = -std::numeric_limits<float>::digits10,
                int highDec = std::numeric_limits<float>::digits10);


size_t float_to_fixed_str(char* buffer,
                     int bufferSize,
                     float value,
                     int precision);


std::string& float_to_str(std::string& str,
                        float value,
                        int precision = -1,
                        int width = 0,
                        char thSep = 0,
                        char decSep = 0);

std::string& float_to_fixed_str(std::string& str,
                             float value,
                             int precision,
                             int width = 0,
                             char thSep = 0,
                             char decSep = 0);

size_t double_to_str(char* buffer,
                 int bufferSize,
                 double value,
                 int lowDec = -std::numeric_limits<double>::digits10,
                 int highDec = std::numeric_limits<double>::digits10);

size_t double_to_fixed_str(char* buffer,
                      int bufferSize,
                      double value,
                      int precision);


std::string& double_to_str(std::string& str,
                         double value,
                         int precision = -1,
                         int width = 0,
                         char thSep = 0,
                         char decSep = 0);

std::string& double_to_fixed_str(std::string& str,
                              double value,
                              int precision = -1,
                              int width = 0,
                              char thSep = 0,
                              char decSep = 0);

float str_to_float(ksl::string_view sv);

bool str_to_float(ksl::string_view sv, float& result, char decSep = '.', char thSep = ',');

double str_to_double(ksl::string_view str);

bool str_to_double(ksl::string_view, double& result, char decSep = '.', char thSep = ',');

} //namespace strings
} //namespace kob

#endif //KOB_STRING_DETAIL_NUMERIC_STRING_H_s
