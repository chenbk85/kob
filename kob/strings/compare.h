//
// Created by 李寅斌 on 2018/11/12.
//

#ifndef KOB_STRINGS_COMPARE_H_
#define KOB_STRINGS_COMPARE_H_
#include <kob/profile/prof.h>
#include <kob/ksl/string_view.h>

namespace kob {
namespace strings {

template <typename Char>
int compare(const ksl::basic_string_view<Char> &s1,
            const ksl::basic_string_view<Char> &s2)
{
    return s1.compare(s2);
}

template <typename S>
int compare(const S &s1, const S &s2)
{
    ksl::basic_string_view<typename S::value_type> sv1(s1);
    ksl::basic_string_view<typename S::value_type> sv2(s2);
    return compare(sv1, sv2);

}

template <typename S>
int compare(const typename S::value_type *s1, const typename S::value_type *s2)
{
    ksl::basic_string_view<typename S::value_type> sv1(s1);
    ksl::basic_string_view<typename S::value_type> sv2(s2);
    return compare(sv1, sv2);
}

template <typename S>
int compare(const typename S::value_type *s1, typename S::size_type n1,
            const typename S::value_type *s2, typename S::size_type n2)
{
    ksl::basic_string_view<typename S::value_type> sv1(s1, n1);
    ksl::basic_string_view<typename S::value_type> sv2(s2, n2);
    return compare(sv1, sv2);
}

template <typename S>
int compare(const typename S::value_type *s1, const S &s2)
{
    ksl::basic_string_view<typename S::value_type> sv1(s1);
    ksl::basic_string_view<typename S::value_type> sv2(s2);
    return compare(sv1, sv2);
}

template <typename S>
int compare(const typename S::value_type *s1, const typename S::size_type n, const S &s2)
{
    ksl::basic_string_view<typename S::value_type> sv1(s1, n);
    ksl::basic_string_view<typename S::value_type> sv2(s2);
    return compare(sv1, sv2);
}

template <typename S>
int compare(const S &s1, const typename S::value_type *s2)
{
    ksl::basic_string_view<typename S::value_type> sv1(s1);
    ksl::basic_string_view<typename S::value_type> sv2(s2);
    return compare(sv1, sv2);
}

template <typename S>
int compare(const S &s1, const typename S::value_type *s2, typename S::size_type n)
{
    ksl::basic_string_view<typename S::value_type> sv1(s1);
    ksl::basic_string_view<typename S::value_type> sv2(s2, n);
    return compare(sv1, sv2);
}

//icase


template <typename Char>
int compare_icase(const ksl::basic_string_view<Char> &s1,
            const ksl::basic_string_view<Char> &s2)
{
    return s1.compare_icase(s2);
}

template <typename S>
int compare_icase(const S &s1, const S &s2)
{
    ksl::basic_string_view<typename S::value_type> sv1(s1);
    ksl::basic_string_view<typename S::value_type> sv2(s2);
    return compare_icase(sv1, sv2);

}

template <typename Char>
int compare_icase(const Char *s1, const Char *s2)
{
    ksl::basic_string_view<Char> sv1(s1);
    ksl::basic_string_view<Char> sv2(s2);
    return compare_icase(sv1, sv2);
}

template <typename S>
int compare_icase(const typename S::value_type *s1, typename S::size_type n1,
            const typename S::value_type *s2, typename S::size_type n2)
{
    ksl::basic_string_view<typename S::value_type> sv1(s1, n1);
    ksl::basic_string_view<typename S::value_type> sv2(s2, n2);
    return compare_icase(sv1, sv2);
}

template <typename S>
int compare_icase(const typename S::value_type *s1, const S &s2)
{
    ksl::basic_string_view<typename S::value_type> sv1(s1);
    ksl::basic_string_view<typename S::value_type> sv2(s2);
    return compare_icase(sv1, sv2);
}

template <typename S>
int compare_icase(const typename S::value_type *s1, const typename S::size_type n, const S &s2)
{
    ksl::basic_string_view<typename S::value_type> sv1(s1, n);
    ksl::basic_string_view<typename S::value_type> sv2(s2);
    return compare_icase(sv1, sv2);
}

template <typename S>
int compare_icase(const S &s1, const typename S::value_type *s2)
{
    ksl::basic_string_view<typename S::value_type> sv1(s1);
    ksl::basic_string_view<typename S::value_type> sv2(s2);
    return compare_icase(sv1, sv2);
}

template <typename S>
int compare_icase(const S &s1, const typename S::value_type *s2, typename S::size_type n)
{
    ksl::basic_string_view<typename S::value_type> sv1(s1);
    ksl::basic_string_view<typename S::value_type> sv2(s2, n);
    return compare_icase(sv1, sv2);
}

//start_with
template <typename Char>
bool start_with(const ksl::basic_string_view<Char> &sv1,
                const ksl::basic_string_view<Char> &sv2)
{
    return sv1.start_with(sv2);
}

template <typename S>
bool start_with( const S &s1, const S &s2)
{
    ksl::basic_string_view<typename S::value_type> sv1(s1);
    ksl::basic_string_view<typename S::value_type> sv2(s2);
    return start_with(sv1, sv2);
}

template <typename S>
bool start_with(const typename S::value_type* s1, typename S::value_type n, const S &s2)
{
    ksl::basic_string_view<typename S::value_type> sv1(s1, n);
    ksl::basic_string_view<typename S::value_type> sv2(s2);
    return start_with(sv1, sv2);
}

template <typename S>
bool start_with(const S &s1, const typename S::value_type *s2)
{
    ksl::basic_string_view<typename S::value_type> sv1(s1);
    ksl::basic_string_view<typename S::value_type> sv2(s2);
    return start_with(sv1, sv2);
}


template <typename S>
bool start_with(const S &s1, const typename S::value_type* s2, typename S::value_type n)
{
    ksl::basic_string_view<typename S::value_type> sv1(s1);
    ksl::basic_string_view<typename S::value_type> sv2(s2, n);
    return start_with(sv1, sv2);
}

template <typename S>
bool start_with(const typename S::value_type* s1, typename S::value_type n1,
                const typename S::value_type* s2, typename S::value_type n2)
{
    ksl::basic_string_view<typename S::value_type> sv1(s1, n1);
    ksl::basic_string_view<typename S::value_type> sv2(s2, n2);
    return start_with(sv1, sv2);
}

//start_with_icase

template <typename Char>
bool start_with_icase(const ksl::basic_string_view<Char> &sv1,
                const ksl::basic_string_view<Char> &sv2)
{
    return sv1.start_with_icase(sv2);
}

template <typename S>
bool start_with_icase( const S &s1, const S &s2)
{
    ksl::basic_string_view<typename S::value_type> sv1(s1);
    ksl::basic_string_view<typename S::value_type> sv2(s2);
    return start_with_icase(sv1, sv2);
}

template <typename S>
bool start_with_icase(const typename S::value_type* s1, typename S::value_type n, const S &s2)
{
    ksl::basic_string_view<typename S::value_type> sv1(s1, n);
    ksl::basic_string_view<typename S::value_type> sv2(s2);
    return start_with_icase(sv1, sv2);
}

template <typename S>
bool start_with_icase(const S &s1, const typename S::value_type *s2)
{
    ksl::basic_string_view<typename S::value_type> sv1(s1);
    ksl::basic_string_view<typename S::value_type> sv2(s2);
    return start_with(sv1, sv2);
}

template <typename S>
bool start_with_icase(const S &s1, const typename S::value_type* s2, typename S::value_type n)
{
    ksl::basic_string_view<typename S::value_type> sv1(s1);
    ksl::basic_string_view<typename S::value_type> sv2(s2, n);
    return start_with_icase(sv1, sv2);
}

template <typename S>
bool start_with_icase(const typename S::value_type* s1, typename S::value_type n1,
                const typename S::value_type* s2, typename S::value_type n2)
{
    ksl::basic_string_view<typename S::value_type> sv1(s1, n1);
    ksl::basic_string_view<typename S::value_type> sv2(s2, n2);
    return start_with_icase(sv1, sv2);
}

//end_with
template <typename Char>
bool end_with(const ksl::basic_string_view<Char> &sv1,
                      const ksl::basic_string_view<Char> &sv2)
{
    return sv1.end_with(sv2);
}

template <typename S>
bool end_with( const S &s1, const S &s2)
{
    ksl::basic_string_view<typename S::value_type> sv1(s1);
    ksl::basic_string_view<typename S::value_type> sv2(s2);
    return end_with(sv1, sv2);
}

template <typename S>
bool end_with(const S &s1, const typename S::value_type *s2)
{
    ksl::basic_string_view<typename S::value_type> sv1(s1);
    ksl::basic_string_view<typename S::value_type> sv2(s2);
    return end_with(sv1, sv2);
}

template <typename S>
bool end_with(const typename S::value_type* s1, typename S::value_type n, const S &s2)
{
    ksl::basic_string_view<typename S::value_type> sv1(s1, n);
    ksl::basic_string_view<typename S::value_type> sv2(s2);
    return end_with(sv1, sv2);
}

template <typename S>
bool end_with(const S &s1, const typename S::value_type* s2, typename S::value_type n)
{
    ksl::basic_string_view<typename S::value_type> sv1(s1);
    ksl::basic_string_view<typename S::value_type> sv2(s2, n);
    return end_with(sv1, sv2);
}

template <typename S>
bool end_with(const typename S::value_type* s1, typename S::value_type n1,
                      const typename S::value_type* s2, typename S::value_type n2)
{
    ksl::basic_string_view<typename S::value_type> sv1(s1, n1);
    ksl::basic_string_view<typename S::value_type> sv2(s2, n2);
    return end_with(sv1, sv2);
}

// end_with_icase
template <typename Char>
bool end_with_icase(const ksl::basic_string_view<Char> &sv1,
              const ksl::basic_string_view<Char> &sv2)
{
    return sv1.end_with_icase(sv2);
}

template <typename S>
bool end_with_icase( const S &s1, const S &s2)
{
    ksl::basic_string_view<typename S::value_type> sv1(s1);
    ksl::basic_string_view<typename S::value_type> sv2(s2);
    return end_with_icase(sv1, sv2);
}

template <typename S>
bool end_with_icase(const typename S::value_type* s1, typename S::value_type n, const S &s2)
{
    ksl::basic_string_view<typename S::value_type> sv1(s1, n);
    ksl::basic_string_view<typename S::value_type> sv2(s2);
    return end_with_icase(sv1, sv2);
}

template <typename S>
bool end_with_icase(const S &s1, const typename S::value_type *s2)
{
    ksl::basic_string_view<typename S::value_type> sv1(s1);
    ksl::basic_string_view<typename S::value_type> sv2(s2);
    return end_with_icase(sv1, sv2);
}

template <typename S>
bool end_with_icase(const S &s1, const typename S::value_type* s2, typename S::value_type n)
{
    ksl::basic_string_view<typename S::value_type> sv1(s1);
    ksl::basic_string_view<typename S::value_type> sv2(s2, n);
    return end_with_icase(sv1, sv2);
}

template <typename S>
bool end_with_icase(const typename S::value_type* s1, typename S::value_type n1,
              const typename S::value_type* s2, typename S::value_type n2)
{
    ksl::basic_string_view<typename S::value_type> sv1(s1, n1);
    ksl::basic_string_view<typename S::value_type> sv2(s2, n2);
    return end_with_icase(sv1, sv2);
}


} //namespace strigns
} //namespace kob
#endif //KOB_STRINGS_COMPARE_H_
