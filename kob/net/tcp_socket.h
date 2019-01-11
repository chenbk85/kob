//
// Created by 李寅斌 on 2018/12/4.
//

#ifndef KOB_NET_TCP_SOCKET_H_
#define KOB_NET_TCP_SOCKET_H_
#include <kob/net/socket_option.h>
#include <kob/log/logging.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <cassert>
#include <cerrno>
#include <cstring>
#include <string>
#include <utility>

namespace kob {
namespace net {

class tcp_socket {
public:
    static const int kInvalidFd = -1;
public:
    tcp_socket();


    explicit tcp_socket(int fd) KOB_NOEXCEPT;


    tcp_socket(tcp_socket &&other) KOB_NOEXCEPT;


    tcp_socket& operator=(tcp_socket &&other) KOB_NOEXCEPT;

    ~tcp_socket();

    bool is_valid() const;

    int  get_fd() const;

    socket_option get_option() const;

    bool set_no_block();

    bool set_block();

    bool set_no_delay();

    bool set_delay();

    bool close();

    bool set_reuse_addr();

    bool set_no_reuse_addr();

    bool set_keep_alive();

    bool set_no_keep_alive();

    bool set_reuse_port();

    bool set_no_reuse_port();

    bool set_snd_buf(size_t n);

    bool set_recv_buf(size_t n);

    bool set_quick_ack();

    bool set_no_quick_ack();

    ssize_t read(void *data, size_t len);

    ssize_t write(const void *data, size_t len);

    static tcp_socket create(int family);

private:
    tcp_socket(const tcp_socket &) = delete;
    tcp_socket& operator=(const tcp_socket &) = delete;
private:
    socket_option  _option;
    int            _fd;
};

///inlines
    KOB_FORCE_INLINE tcp_socket::tcp_socket() :_fd(kInvalidFd)
    {

    }

    KOB_FORCE_INLINE tcp_socket::tcp_socket(int fd) KOB_NOEXCEPT
    :_fd(fd)
    {

    }

    KOB_FORCE_INLINE tcp_socket::tcp_socket(tcp_socket &&other) KOB_NOEXCEPT
        :_fd(other._fd)
    {
        other._fd = kInvalidFd;
    }

    KOB_FORCE_INLINE tcp_socket& tcp_socket::operator=(tcp_socket &&other) KOB_NOEXCEPT
    {
        if (&other == this) {
            return *this;
        }
        if (_fd > 0) {
            close();
        }
        _fd = other._fd;
        other._fd = kInvalidFd;
        return  *this;
    }

    KOB_FORCE_INLINE tcp_socket::~tcp_socket()
    {
        if (_fd) {
            //hop you close it before dctor
            LOG_WARN("do not close fd before fd: {}", _fd);
            close();
        }
    }

    KOB_FORCE_INLINE bool tcp_socket::is_valid() const
    {
        return _fd > 0;
    }

    KOB_FORCE_INLINE int  tcp_socket::get_fd() const
    {
        return  _fd;
    }

    KOB_FORCE_INLINE socket_option tcp_socket::get_option() const
    {
        return _option;
    }

} //namespace net
} //namespace kob
#endif //KOB_NET_TCP_SOCKET_H_
