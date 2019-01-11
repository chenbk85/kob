//
// Created by 李寅斌 on 2018/11/22.
//

#ifndef KOB_EVENT_EVENT_H_
#define KOB_EVENT_EVENT_H_

#include <kob/profile/prof.h>
#include <kob/ksl/type_traits.h>
#include <cstdint>

namespace kob {
namespace event {


enum class poll_event : uint32_t{
    none       = 0x00,
    read       = 0x001,
    read_pri   = 0x002,
    write      = 0x004,
    error      = 0x008,
    hup        = 0x010,
    nval       = 0x020,
    read_norm  = 0x040,
    read_band  = 0x080,
    write_norm = 0x100,
    write_band = 0x200,
    msg        = 0x400,
    rd_hup     = 0x2000,
};

KOB_FORCE_INLINE poll_event operator & (poll_event lhs, poll_event rhs)
{
    return static_cast<poll_event>(
                static_cast<uint32_t>(lhs) &
                static_cast<uint32_t>(rhs)
            );
}

KOB_FORCE_INLINE poll_event operator | (poll_event lhs, poll_event rhs)
{
    return static_cast<poll_event>(
                static_cast<uint32_t>(lhs) |
                static_cast<uint32_t>(rhs)
            );
}

KOB_FORCE_INLINE poll_event operator ^ (poll_event lhs, poll_event rhs)
{
    return static_cast<poll_event>(
                static_cast<uint32_t>(lhs) ^
                static_cast<uint32_t>(rhs)
            );
}

KOB_FORCE_INLINE poll_event operator ~ (poll_event lhs)
{
    return static_cast<poll_event>(~static_cast<uint32_t>(lhs));
}

KOB_FORCE_INLINE poll_event& operator &= (poll_event &lhs, poll_event rhs)
{
    lhs = lhs & rhs;
    return lhs;
}

KOB_FORCE_INLINE poll_event& operator |= (poll_event &lhs, poll_event rhs)
{
    lhs = lhs | rhs;
    return lhs;
}

KOB_FORCE_INLINE poll_event& operator ^= (poll_event &lhs, poll_event rhs)
{
    lhs = lhs ^ rhs;
    return lhs;
}

/**
 * 0x00000000 none
 * 0x00000001 0x00000008 read write enable status
 * 0x00000010 0x00000080 managed status
 * 0x00000100 0x00001000 for user
 */

enum class event_status :uint32_t {
    mask       = 0x00F, //0x0000000D?
    none       = 0x000,
    read_set   = 0x001,
    write_set  = 0x002,
    error      = 0x004,
    hup        = 0x008,

    managed    = 0x010,
};


KOB_FORCE_INLINE event_status operator & (event_status lhs, event_status rhs)
{
    return static_cast<event_status>(
            static_cast<uint32_t>(lhs) &
            static_cast<uint32_t>(rhs)
        );
}

KOB_FORCE_INLINE event_status operator | (event_status lhs, event_status rhs)
{
    return static_cast<event_status>(
            static_cast<uint32_t>(lhs) |
            static_cast<uint32_t>(rhs)
        );
}

KOB_FORCE_INLINE event_status operator ^ (event_status lhs, event_status rhs)
{
    return static_cast<event_status>(
            static_cast<uint32_t>(lhs) ^
            static_cast<uint32_t>(rhs)
        );
}

KOB_FORCE_INLINE event_status operator ~ (event_status lhs)
{
    return static_cast<event_status>(
            ~static_cast<uint32_t>(lhs)
        );
}

KOB_FORCE_INLINE event_status& operator ^= (event_status &lhs, event_status rhs)
{
    lhs = lhs ^ rhs;
    return lhs;
}

KOB_FORCE_INLINE event_status& operator &= (event_status &lhs, event_status rhs)
{
    return lhs = lhs & rhs;
}

KOB_FORCE_INLINE event_status& operator |= (event_status &lhs, event_status rhs)
{
    return lhs = lhs | rhs;
}

template <typename IntType>
KOB_FORCE_INLINE bool operator == (event_status es, IntType v)
{
    KOB_STATIC_ASSERT(std::is_integral<IntType>::value);
    return static_cast<uint32_t>(es) == static_cast<uint32_t>(v);
}

template <typename IntType>
KOB_FORCE_INLINE bool operator != (event_status es, IntType v)
{
    KOB_STATIC_ASSERT(std::is_integral<IntType>::value);
    return static_cast<uint32_t>(es) != static_cast<uint32_t>(v);
}

template <typename IntType>
KOB_FORCE_INLINE bool operator == (poll_event es, IntType v)
{
    KOB_STATIC_ASSERT(std::is_integral<IntType>::value);
    return static_cast<uint32_t>(es) == static_cast<uint32_t>(v);
}

template <typename IntType>
KOB_FORCE_INLINE bool operator != (poll_event es, IntType v)
{
    KOB_STATIC_ASSERT(std::is_integral<IntType>::value);
    return static_cast<uint32_t>(es) != static_cast<uint32_t>(v);
}


KOB_FORCE_INLINE bool is_read_set(event_status e)
{
    event_status ec = e & event_status::read_set;
    return ec != 0 ? true : false;
}

KOB_FORCE_INLINE bool is_write_set(event_status e)
{
    event_status ec = e & event_status::write_set;
    return ec != 0 ? true : false;
}

KOB_FORCE_INLINE bool is_error_curr(event_status e)
{
    event_status ec = e & event_status::error;
    return ec != 0 ? true : false;
}

KOB_FORCE_INLINE bool is_managed(event_status e)
{
    event_status ec = e & event_status::managed;
    return ec != 0 ? true : false;
}

KOB_FORCE_INLINE void set_managed(event_status &e)
{
    e |= event_status::managed;
}

KOB_FORCE_INLINE void detach_managed(event_status &e)
{
    e &= ~event_status::managed;
}

KOB_FORCE_INLINE void set_event_status(poll_event pe, event_status &e)
{
    e = static_cast<event_status>(
            (static_cast<uint32_t>(pe) &
            static_cast<uint32_t>(event_status::mask)) |
            static_cast<uint32_t>(e)
            );
}

KOB_FORCE_INLINE poll_event merge_from_status(event_status e)
{
    poll_event ec = poll_event::none;
    ec = static_cast<poll_event>(
            (static_cast<uint32_t>(e) &
             static_cast<uint32_t>(event_status::mask))
    );
    return ec;
}


enum class event_opt {
    none = 0,
    add = 1,
    ctl = 2,
    del = 3
};

/**
 * is_event_*** for poll_event
 */

KOB_FORCE_INLINE bool is_event_read(poll_event e)
{
    return (e & poll_event::read) == poll_event::read ? true : false;
}

KOB_FORCE_INLINE bool is_event_read_pri(poll_event e)
{
    return (e & poll_event::read_pri) != 0 ? true : false;
}

KOB_FORCE_INLINE bool is_event_rd_hup(poll_event e)
{
    return (e & poll_event::rd_hup) != 0 ? true : false;
}

KOB_FORCE_INLINE bool is_event_hup(poll_event e)
{
    return (e & poll_event::hup) != 0 ? true : false;
}

KOB_FORCE_INLINE bool is_event_error(poll_event e)
{
    return (e & poll_event::error) != 0 ? true : false;
}

KOB_FORCE_INLINE bool is_event_write(poll_event e)
{
    return (e & poll_event::write) != 0 ? true : false;
}

KOB_FORCE_INLINE bool is_event_nval(poll_event e)
{
    return (e & poll_event::nval) != 0 ? true : false;
}

KOB_FORCE_INLINE bool is_event_no_rdwr(poll_event e)
{
    return !is_event_read(e) && is_event_write(e);
}

} //namespace event
} //namespace kob
#endif //KOB_EVENT_EVENT_H_
