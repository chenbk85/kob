//
// Created by 李寅斌 on 2018/12/4.
//

#ifndef KOB_EVENT_SOCKET_OPTION_H_
#define KOB_EVENT_SOCKET_OPTION_H_
#include <kob/profile/prof.h>
#include <kob/ksl/timespan.h>
#include <kob/ksl/timespan.h>
#include <cassert>
#include <cerrno>
#include <cstring>
#include <string>
#include <utility>
#include <fcntl.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>


namespace kob {
namespace net {

enum class socket_status : uint32_t{
    none            = 0x000,
    no_block        = 0x001,
    no_delay        = 0x002,
    cloexec         = 0x004,
    reuse_addr      = 0x008,
    reuse_port      = 0x010,
    quick_ack       = 0x020,
    keep_alive      = 0x040,
    so_linger       = 0x080,
    shutdown_read   = 0x100,
    shutdown_write  = 0x200,
    shutdown_all    = shutdown_read | shutdown_write,
    closed          = 0x800,
    read_timeout    = 0x1000,
    write_timeout   = 0x2000,
    snd_buf         = 0x4000,
    recv_buf        = 0x8000,
    error           = 0x10000000
};

KOB_FORCE_INLINE socket_status operator & (socket_status lhs, socket_status rhs)
{
    return static_cast<socket_status>(
                static_cast<uint32_t>(lhs) &
                static_cast<uint32_t>(rhs)
            );
}

KOB_FORCE_INLINE socket_status operator | (socket_status lhs, socket_status rhs)
{
    return static_cast<socket_status>(
            static_cast<uint32_t>(lhs) |
            static_cast<uint32_t>(rhs)
            );
}

KOB_FORCE_INLINE socket_status operator ^ (socket_status lhs, socket_status rhs)
{
    return static_cast<socket_status>(
            static_cast<uint32_t>(lhs) ^
            static_cast<uint32_t>(rhs)
            );
}

KOB_FORCE_INLINE socket_status operator ~ (socket_status lhs)
{
    return static_cast<socket_status>(
            ~static_cast<uint32_t>(lhs)
            );
}

KOB_FORCE_INLINE socket_status& operator &= (socket_status &lhs, socket_status rhs)
{
    lhs = lhs & rhs;
    return lhs;
}

KOB_FORCE_INLINE socket_status& operator |= (socket_status &lhs, socket_status rhs)
{
    lhs = lhs | rhs;
    return lhs;
}

KOB_FORCE_INLINE socket_status& operator ^= (socket_status &lhs, socket_status rhs)
{
    lhs = lhs | rhs;
    return lhs;
}

KOB_FORCE_INLINE bool is_status_no_block(socket_status ss)
{
    if (static_cast<uint32_t>(ss & socket_status::no_block) != 0) {
        return true;
    }
    return  false;
}

KOB_FORCE_INLINE void set_status_no_block(socket_status &ss)
{
    ss |= socket_status::no_block;
}

KOB_FORCE_INLINE void unset_status_no_block(socket_status &ss)
{
    ss &= ~socket_status::no_block;
}

KOB_FORCE_INLINE bool is_status_no_delay(socket_status ss)
{
    if (static_cast<uint32_t>(ss & socket_status::no_delay) != 0) {
        return true;
    }
    return  false;
}

KOB_FORCE_INLINE void set_status_no_delay(socket_status &ss)
{
    ss |= socket_status::no_delay;
}

KOB_FORCE_INLINE void unset_status_no_delay(socket_status &ss)
{
    ss &= ~socket_status::no_delay;
}

KOB_FORCE_INLINE bool is_status_cloexec(socket_status ss)
{
    if (static_cast<uint32_t>(ss & socket_status::cloexec) != 0) {
        return true;
    }
    return  false;
}

KOB_FORCE_INLINE void set_status_cloexec(socket_status &ss)
{
    ss |= socket_status::cloexec;
}

KOB_FORCE_INLINE void unset_status_cloexec(socket_status &ss)
{
    ss &= ~socket_status::cloexec;
}

KOB_FORCE_INLINE bool is_status_reuse_addr(socket_status ss)
{
    if (static_cast<uint32_t>(ss & socket_status::reuse_addr) != 0) {
        return true;
    }
    return  false;
}

KOB_FORCE_INLINE void set_status_reuse_addr(socket_status &ss)
{
    ss |= socket_status::reuse_addr;
}

KOB_FORCE_INLINE void unset_status_reuse_addr(socket_status &ss)
{
    ss &= ~socket_status::reuse_addr;
}


KOB_FORCE_INLINE bool is_status_reuse_port(socket_status ss)
{
    if (static_cast<uint32_t>(ss & socket_status::reuse_port) != 0) {
        return true;
    }
    return  false;
}

KOB_FORCE_INLINE void set_status_reuse_port(socket_status &ss)
{
    ss |= socket_status::reuse_port;
}

KOB_FORCE_INLINE void unset_status_reuse_port(socket_status &ss)
{
    ss &= ~socket_status::reuse_port;
}

KOB_FORCE_INLINE bool is_status_quick_ack(socket_status ss)
{
    if (static_cast<uint32_t>(ss & socket_status::quick_ack) != 0) {
        return true;
    }
    return  false;
}

KOB_FORCE_INLINE void set_status_quick_ack(socket_status &ss)
{
    ss |= socket_status::quick_ack;
}

KOB_FORCE_INLINE void unset_status_quick_ack(socket_status &ss)
{
    ss &= ~socket_status::quick_ack;
}

KOB_FORCE_INLINE bool is_status_shutdown_read(socket_status ss)
{
    if (static_cast<uint32_t>(ss & socket_status::shutdown_read) != 0) {
        return true;
    }
    return  false;
}

KOB_FORCE_INLINE void set_status_shutdown_read(socket_status &ss)
{
    ss |= socket_status::shutdown_read;
}

KOB_FORCE_INLINE void unset_status_shutdown_read(socket_status &ss)
{
    ss &= ~socket_status::shutdown_read;
}

KOB_FORCE_INLINE bool is_status_shutdown_write(socket_status ss)
{
    if (static_cast<uint32_t>(ss & socket_status::shutdown_write) != 0) {
        return true;
    }
    return  false;
}

KOB_FORCE_INLINE void set_status_shutdown_write(socket_status &ss)
{
    ss |= socket_status::shutdown_write;
}

KOB_FORCE_INLINE void unset_status_shutdown_write(socket_status &ss)
{
    ss &= ~socket_status::shutdown_write;
}

KOB_FORCE_INLINE bool is_status_shutdown_all(socket_status ss)
{
    if (static_cast<uint32_t>(ss & socket_status::shutdown_all) != 0) {
        return true;
    }
    return  false;
}

KOB_FORCE_INLINE void set_status_shutdown_all(socket_status &ss)
{
    ss |= socket_status::shutdown_all;
}

KOB_FORCE_INLINE void unset_status_shutdown_all(socket_status &ss)
{
    ss &= ~socket_status::shutdown_all;
}

KOB_FORCE_INLINE bool is_status_closed(socket_status ss)
{
    if (static_cast<uint32_t>(ss & socket_status::closed) != 0) {
        return true;
    }
    return  false;
}

KOB_FORCE_INLINE void set_status_closed(socket_status &ss)
{
    ss |= socket_status::closed;
}

KOB_FORCE_INLINE void unset_status_closed(socket_status &ss)
{
    ss &= ~socket_status::closed;
}

KOB_FORCE_INLINE bool is_status_keep_alive(socket_status ss)
{
    if (static_cast<uint32_t>(ss & socket_status::keep_alive) != 0) {
        return true;
    }
    return  false;
}

KOB_FORCE_INLINE void set_status_keep_alive(socket_status &ss)
{
    ss |= socket_status::keep_alive;
}

KOB_FORCE_INLINE void unset_status_keep_alive(socket_status &ss)
{
    ss &= ~socket_status::keep_alive;
}

KOB_FORCE_INLINE bool is_status_so_linger(socket_status ss)
{
    if (static_cast<uint32_t>(ss & socket_status::so_linger) != 0) {
        return true;
    }
    return  false;
}

KOB_FORCE_INLINE void set_status_so_linger(socket_status &ss)
{
    ss |= socket_status::so_linger;
}

KOB_FORCE_INLINE void unset_status_so_linger(socket_status &ss)
{
    ss &= ~socket_status::so_linger;
}

KOB_FORCE_INLINE bool is_status_read_timeout(socket_status ss)
{
    if (static_cast<uint32_t>(ss & socket_status::read_timeout) != 0) {
        return true;
    }
    return  false;
}

KOB_FORCE_INLINE void set_status_read_timeout(socket_status &ss)
{
    ss |= socket_status::read_timeout;
}

KOB_FORCE_INLINE void unset_status_read_timeout(socket_status &ss)
{
    ss &= ~socket_status::read_timeout;
}

KOB_FORCE_INLINE bool is_status_write_timeout(socket_status ss)
{
    if (static_cast<uint32_t>(ss & socket_status::write_timeout) != 0) {
        return true;
    }
    return  false;
}

KOB_FORCE_INLINE void set_status_write_timeout(socket_status &ss)
{
    ss |= socket_status::write_timeout;
}

KOB_FORCE_INLINE void unset_status_write_timeout(socket_status &ss)
{
    ss &= ~socket_status::write_timeout;
}

KOB_FORCE_INLINE bool is_status_snd_buf(socket_status ss)
{
    if (static_cast<uint32_t>(ss & socket_status::snd_buf) != 0) {
        return true;
    }
    return  false;
}

KOB_FORCE_INLINE void set_status_snd_buf(socket_status &ss)
{
    ss |= socket_status::snd_buf;
}

KOB_FORCE_INLINE void unset_status_snd_buf(socket_status &ss)
{
    ss &= ~socket_status::snd_buf;
}

KOB_FORCE_INLINE bool is_status_recv_buf(socket_status ss)
{
    if (static_cast<uint32_t>(ss & socket_status::recv_buf) != 0) {
        return true;
    }
    return  false;
}

KOB_FORCE_INLINE void set_status_recv_buf(socket_status &ss)
{
    ss |= socket_status::recv_buf;
}

KOB_FORCE_INLINE void unset_status_recv_buf(socket_status &ss)
{
    ss &= ~socket_status::recv_buf;
}


KOB_FORCE_INLINE bool is_status_error(socket_status ss)
{
    if (static_cast<uint32_t>(ss & socket_status::write_timeout) != 0) {
        return true;
    }
    return  false;
}

KOB_FORCE_INLINE void set_status_error(socket_status &ss)
{
    ss |= socket_status::error;
}

KOB_FORCE_INLINE void unset_status_error(socket_status &ss)
{
    ss &= ~socket_status::error;
}

struct socket_option {
    socket_status   status;
    uint32_t        sbuf;
    uint32_t        rbuf;
};

} //namespace net
} //namespace kob
#endif //KOB_EVENT_SOCKET_OPTION_H_
