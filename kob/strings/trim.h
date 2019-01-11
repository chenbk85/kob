//
// Created by 李寅斌 on 2018/11/12.
//

#ifndef KOB_STRINGS_TRIM_H_
#define KOB_STRINGS_TRIM_H_
#include <kob/ksl/ascii.h>

namespace kob {

namespace strings {
template <class S>
S trim_left(const S& str)
{
    typename S::const_iterator it  = str.begin();
    typename S::const_iterator end = str.end();

    while (it != end && ksl::ascii::is_space(*it)) ++it;
    return S(it, end);
}


template <class S>
S& trim_left_in_place(S& str)
{
    typename S::iterator it  = str.begin();
    typename S::iterator end = str.end();

    while (it != end && ksl::ascii::is_space(*it)) ++it;
    str.erase(str.begin(), it);
    return str;
}


template <class S>
S trim_right(const S& str)
{
    int pos = int(str.size()) - 1;

    while (pos >= 0 && ksl::ascii::is_space(str[pos])) --pos;
    return S(str, 0, pos + 1);
}

template <class S>
S& trim_right_in_place(S& str)
{
    int pos = int(str.size()) - 1;

    while (pos >= 0 && ksl::ascii::is_space(str[pos])) --pos;
    str.resize(pos + 1);

    return str;
}

template <class S>
S trim(const S& str)
{
    int first = 0;
    int last  = int(str.size()) - 1;

    while (first <= last && ksl::ascii::is_space(str[first])) ++first;
    while (last >= first && ksl::ascii::is_space(str[last])) --last;

    return S(str, first, last - first + 1);
}

template <class S>
S& trim_in_place(S& str)
{
    int first = 0;
    int last  = int(str.size()) - 1;

    while (first <= last && ksl::ascii::is_space(str[first])) ++first;
    while (last >= first && ksl::ascii::is_space(str[last])) --last;

    str.resize(last + 1);
    str.erase(0, first);

    return str;
}

} //namespace strings

} //namespace kob
#endif //KOB_STRINGS_TRIM_H_
