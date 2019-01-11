//
// Created by 李寅斌 on 2018/11/12.
//

#ifndef KOB_STRINGS_TRANSLATE_H_
#define KOB_STRINGS_TRANSLATE_H_

#include <kob/ksl/ascii.h>

namespace kob {
namespace strings {

template <typename S>
S to_upper(const S& str)
{
    typename S::const_iterator it  = str.begin();
    typename S::const_iterator end = str.end();

    S result;
    result.reserve(str.size());
    while (it != end) {
        result +=
            static_cast<typename S::value_type>(ksl::ascii::to_upper(*it++));
    }
    return result;
}

template <class S>
S& to_upper_in_place(S& str)
{
    typename S::iterator it  = str.begin();
    typename S::iterator end = str.end();

    while (it != end) {
        *it = static_cast<typename S::value_type>(ksl::ascii::to_upper(*it));
        ++it;
    }
    return str;
}

template <class S>
S to_lower(const S& str)
{
    typename S::const_iterator it  = str.begin();
    typename S::const_iterator end = str.end();

    S result;
    result.reserve(str.size());
    while (it != end) {
        result +=
            static_cast<typename S::value_type>(ksl::ascii::to_lower(*it++));
    }
    return result;
}

/// Replaces all characters in str with their lower-case counterparts.
template <class S>
S& to_lower_in_place(S& str)
{
    typename S::iterator it  = str.begin();
    typename S::iterator end = str.end();

    while (it != end) {
        *it =
            static_cast<typename S::value_type>(ksl::ascii::to_lower(*it));
        ++it;
    }
    return str;
}

template <class S>
S translate(const S& str, const S& from, const S& to)
{
    S result;
    result.reserve(str.size());
    typename S::const_iterator it  = str.begin();
    typename S::const_iterator end = str.end();
    typename S::size_type toSize = to.size();
    while (it != end) {
        typename S::size_type pos = from.find(*it);
        if (pos == S::npos) {
            result += *it;
        } else {
            if (pos < toSize) result += to[pos];
        }
        ++it;
    }
    return result;
}


template <class S>
S translate(const S& str, const typename S::value_type* from, const typename S::value_type* to)
{
    return translate(str, S(from), S(to));
}

/// Replaces in str all occurences of characters in from
/// with the corresponding (by position) characters in to.
/// If there is no corresponding character, the character
/// is removed.
template <class S>
S& translate_in_place(S& str, const S& from, const S& to)
{
    str = translate(str, from, to);
    return str;
}


template <class S>
S translate_in_place(S& str, const typename S::value_type* from, const typename S::value_type* to)
{
    str = translate(str, S(from), S(to));
    return str;
}

template <class S>
S& replace_in_place(S& str, const S& from, const S& to, typename S::size_type start = 0)
{
    S result;
    typename S::size_type pos = 0;
    result.append(str, 0, start);
    do {
        pos = str.find(from, start);
        if (pos != S::npos) {
            result.append(str, start, pos - start);
            result.append(to);
            start = pos + from.length();
        }
        else result.append(str, start, str.size() - start);
    } while (pos != S::npos);
    str.swap(result);
    return str;
}


template <class S>
S& replace_in_place(S& str, const typename S::value_type* from, const typename S::value_type* to, typename S::size_type start = 0)
{

    S result;
    typename S::size_type pos = 0;
    typename S::size_type fromLen = std::strlen(from);
    result.append(str, 0, start);
    do {
        pos = str.find(from, start);
        if (pos != S::npos) {
            result.append(str, start, pos - start);
            result.append(to);
            start = pos + fromLen;
        } else {
            result.append(str, start, str.size() - start);
        }
    } while (pos != S::npos);
    str.swap(result);
    return str;
}


template <class S>
S& replace_in_place(S& str, const typename S::value_type from, const typename S::value_type to = 0, typename S::size_type start = 0)
{
    if (from == to) return str;

    typename S::size_type pos = 0;
    do {
        pos = str.find(from, start);
        if (pos != S::npos) {
            if (to) {
                str[pos] = to;
            } else {
                str.erase(pos, 1);
            }
        }
    } while (pos != S::npos);

    return str;
}

template <class S>
S& remove_in_place(S& str, const typename S::value_type ch, typename S::size_type start = 0)
{
    return replace_in_place(str, ch, 0, start);
}

template <class S>
S replace(const S& str, const S& from, const S& to, typename S::size_type start = 0)
{
    S result(str);
    replace_in_place(result, from, to, start);
    return result;
}


template <class S>
S replace(const S& str, const typename S::value_type* from, const typename S::value_type* to, typename S::size_type start = 0)
{
    S result(str);
    replace_in_place(result, from, to, start);
    return result;
}


template <class S>
S replace(const S& str, const typename S::value_type from, const typename S::value_type to = 0, typename S::size_type start = 0)
{
    S result(str);
    replace_in_place(result, from, to, start);
    return result;
}


template <class S>
S remove(const S& str, const typename S::value_type ch, typename S::size_type start = 0)
{
    S result(str);
    replace_in_place(result, ch, 0, start);
    return result;
}

} //namespace strings
} //namespace kob
#endif //KOB_STRINGS_TRANSLATE_H_
