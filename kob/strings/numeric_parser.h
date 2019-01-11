//
// Created by 李寅斌 on 2018/11/12.
//

#ifndef KOB_STRINGS_NUMERIC_PARSER_H_
#define KOB_STRINGS_NUMERIC_PARSER_H_
#include <kob/ksl/string_view.h>
#include <kob/ksl/type_traits.h>
#include <kob/strings/double-conversion/numeric_string.h>
#include <kob/strings/compare.h>
#include <cstdint>

namespace kob {
namespace strings {

template <typename IntType>
bool parse_int(ksl::string_view s, IntType& value, char thousandSeparator = ',')
{
    KOB_STATIC_ASSERT_MSG(std::is_integral<IntType>::value, "not int type");
    return kob::strings::str_to_int(s, value,
            kob::strings::constant_value::kNumBaseDes,
            thousandSeparator);
}

template <typename IntType>
bool parse_hex(ksl::string_view sv, IntType& value)
{
    KOB_STATIC_ASSERT_MSG(std::is_integral<IntType>::value, "not int type");
    if(sv.size() > 2 && sv[0] == '0' && (sv[1] == 'x' || sv[1] == 'X')) {
        sv.remove_prefix(2);
    }
    return kob::strings::str_to_int(sv, value,
            kob::strings::constant_value::kNumBaseHex);
}

template <typename IntType>
bool parse_oct(ksl::string_view sv, IntType &value)
{
    KOB_STATIC_ASSERT_MSG(std::is_integral<IntType>::value, "not int type");
    return kob::strings::str_to_int(sv, value,
                                    kob::strings::constant_value::kNumBaseOct);
}

bool parse_bool(ksl::string_view sv, bool &value)
{
        int n;
        if (parse_int(sv, n)) {
            value = (n != 0);
            return true;
        }

        if (compare_icase(sv, "true") == 0) {
            value = true;
            return true;
        } else if (compare_icase(sv, "yes") == 0) {
            value = true;
            return true;
        } else if (compare_icase(sv, "on") == 0) {
            value = true;
            return true;
        }

        if (compare_icase(sv, "false") == 0) {
            value = false;
            return true;
        } else if (compare_icase(sv, "no") == 0) {
            value = false;
            return true;
        } else if (compare_icase(sv, "off") == 0) {
            value = false;
            return true;
        }

        return false;

}


bool parse_float(const std::string& s, float& value, char decSep = '.', char thSep = ',')
{
    return str_to_float(s, value, decSep, thSep);
}

bool parse_double(const std::string& s, double& value, char decSep = '.', char thSep = ',')
{
    return str_to_double(s, value, decSep, thSep);
}


} //namespace strings
} //namespace kob
#endif //KOB_STRINGS_NUMERIC_PARSER_H_
