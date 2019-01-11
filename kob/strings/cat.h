//
// Created by 李寅斌 on 2018/11/12.
//

#ifndef KOB_STRINGS_CAT_H_
#define KOB_STRINGS_CAT_H_

namespace ksl {
namespace strings {

template <class S>
S cat(const S& s1, const S& s2)
{
    S result = s1;
    result.reserve(s1.size() + s2.size());
    result.append(s2);
    return result;
}

    /// Concatenates three strings.
template <class S>
S cat(const S& s1, const S& s2, const S& s3)
{
    S result = s1;
    result.reserve(s1.size() + s2.size() + s3.size());
    result.append(s2);
    result.append(s3);
    return result;
}

    /// Concatenates four strings.
template <class S>
S cat(const S& s1, const S& s2, const S& s3, const S& s4)
{
    S result = s1;
    result.reserve(s1.size() + s2.size() + s3.size() + s4.size());
    result.append(s2);
    result.append(s3);
    result.append(s4);
    return result;
}

template <class S>
S cat(const S& s1, const S& s2, const S& s3, const S& s4, const S& s5)
{
    S result = s1;
    result.reserve(s1.size() + s2.size() + s3.size() + s4.size() + s5.size());
    result.append(s2);
    result.append(s3);
    result.append(s4);
    result.append(s5);
    return result;
}


template <class S>
S cat(const S& s1, const S& s2, const S& s3, const S& s4, const S& s5, const S& s6)
{
    S result = s1;
    result.reserve(s1.size() + s2.size() + s3.size() + s4.size() + s5.size() + s6.size());
    result.append(s2);
    result.append(s3);
    result.append(s4);
    result.append(s5);
    result.append(s6);
    return result;
}


template <class S, class It>
S cat(const S& delim, const It& begin, const It& end)
{
    S result;
    for (It it = begin; it != end; ++it) {
        if (!result.empty()) result.append(delim);
        result += *it;
    }
    return result;
}


} //namespace strings
} //namespace kob
#endif //KOB_STRINGS_CAT_H_

