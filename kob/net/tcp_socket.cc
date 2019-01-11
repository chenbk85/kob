//
// Created by 李寅斌 on 2018/12/4.
//

#include <kob/net/tcp_socket.h>
#include <kob/net/inet_option.h>

namespace kob {
namespace net {

bool tcp_socket::set_no_block()
{
    if (inet_no_block(_fd)) {
        set_status_no_block(_option.status);
        return true;
    }
    set_status_error(_option.status);
    return false;
}


bool tcp_socket::set_block()
{
    if (inet_block(_fd)) {
        unset_status_no_block(_option.status);
        return true;
    }
    set_status_error(_option.status);
    return false;
}

bool tcp_socket::set_no_delay()
{
    if(inet_no_delay(_fd)) {
        set_status_no_delay(_option.status);
        return true;
    }
    set_status_error(_option.status);
    return false;
}

bool tcp_socket::set_delay()
{
    if(inet_delay(_fd)) {
        unset_status_no_delay(_option.status);
        return true;
    }
    set_status_error(_option.status);
    return false;
}

bool tcp_socket::close()
{
    if(inet_close(_fd)) {
        set_status_closed(_option.status);
        return true;
    }
    set_status_error(_option.status);
    return false;

}

bool tcp_socket::set_reuse_addr()
{
    if(inet_reuse_addr(_fd)) {
        set_status_reuse_addr(_option.status);
        return true;
    }
    set_status_error(_option.status);
    return false;
}

bool tcp_socket::set_no_reuse_addr()
{
    if(inet_no_reuse_addr(_fd)) {
        unset_status_reuse_addr(_option.status);
        return true;
    }
    set_status_error(_option.status);
    return false;
}

bool tcp_socket::set_keep_alive()
{
    if(inet_keep_alive(_fd)) {
        set_status_keep_alive(_option.status);
        return true;
    }
    set_status_error(_option.status);
    return false;
}


bool tcp_socket::set_no_keep_alive()
{
    if(inet_no_keep_alive(_fd)) {
        unset_status_keep_alive(_option.status);
        return true;
    }
    set_status_error(_option.status);
    return false;
}

bool tcp_socket::set_reuse_port()
{
    if(inet_reuse_port(_fd)) {
        set_status_reuse_port(_option.status);
        return true;
    }
    set_status_error(_option.status);
    return false;
}


bool tcp_socket::set_no_reuse_port()
{
    if(inet_no_reuse_port(_fd)) {
        unset_status_reuse_port(_option.status);
        return true;
    }
    set_status_error(_option.status);
    return false;
}


bool tcp_socket::set_snd_buf(size_t n)
{
    if(inet_sndbuf(_fd, n)) {
        set_status_snd_buf(_option.status);
        _option.sbuf = n;
        return true;
    }
    set_status_error(_option.status);
    return false;

}

bool tcp_socket::set_recv_buf(size_t n)
{
    if(inet_sndbuf(_fd, n)) {
        set_status_recv_buf(_option.status);
        _option.rbuf = n;
        return true;
    }
    set_status_error(_option.status);
    return false;
}

bool tcp_socket::set_quick_ack()
{
    if(inet_quick_ack(_fd)) {
        set_status_quick_ack(_option.status);
        return true;
    }
    set_status_error(_option.status);
    return false;
}


bool tcp_socket::set_no_quick_ack()
{
    if(inet_quick_ack(_fd)) {
        set_status_quick_ack(_option.status);
        return true;
    }
    set_status_error(_option.status);
    return false;
}

ssize_t tcp_socket::read(void *data, size_t len)
{
    if(is_status_error(_option.status) || _fd == kInvalidFd) {
        return -1;
    }
    ssize_t res;
    do {
        res = ::read(_fd, data, len);
    } while (res < 0 && errno == EINTR);
    return res;
}


ssize_t tcp_socket::write(const void *data, size_t len)
{
    if(is_status_error(_option.status) || _fd == kInvalidFd) {
        return -1;
    }

    ssize_t res;
    do {
        res = ::write(_fd, data, len);
    } while (res < 0 && errno == EINTR);
    return res;
}

tcp_socket tcp_socket::create(int family)
{
    int fd = ::socket(family, SOCK_STREAM, 0);
    if (fd == kInvalidFd) {
        return tcp_socket();
    }
    return tcp_socket(fd);
}

} //namespace net
} //namespace kob