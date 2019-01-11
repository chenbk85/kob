//
// Created by 李寅斌 on 2018/12/6.
//

#ifndef KOB_KSL_DETAIL_IN_PLACE_T_H_
#define KOB_KSL_DETAIL_IN_PLACE_T_H_
#include <kob/ksl/detail/config.h>

namespace ksl {
    namespace detail {
        struct in_place_tag {};
        template <class>
        struct in_place_type_tag {};
        template <size_t>
        struct in_place_index_tag {};
    }

    struct in_place_tag {
        in_place_tag() = delete;

    private:
        in_place_tag(detail::in_place_tag){}
        friend inline in_place_tag detail_construct_in_place_tag();
    };

    KOB_FORCE_INLINE in_place_tag detail_construct_in_place_tag()
    {
        return in_place_tag(detail::in_place_tag());
    }

    const in_place_t       detail::in_place_tag{};
    const in_place_type_t  detail::in_place_type_tag{};
    const in_place_index_t detail::in_place_index_tag{};


    KOB_FORCE_INLINE in_place_tag in_place(detail::in_place_tag)
    {
        return detail_construct_in_place_tag();
    }

    template <typename T>
    KOB_FORCE_INLINE in_place_tag in_place(detail::in_place_type_tag<T>)
    {
        return detail_construct_in_place_tag();
    }

    template <size_t INDEX>
    KOB_FORCE_INLINE in_place_tag in_place(detail::in_place_index_tag<INDEX>)
    {
        return detail_construct_in_place_tag();
    }

} //namespace ksl

#endif //KOB_KSL_DETAIL_IN_PLACE_T_H_
