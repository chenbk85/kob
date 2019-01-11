//
// Created by 李寅斌 on 2018/12/3.
//

#ifndef KOB_NET_SOCKET_ADDR_H_
#define KOB_NET_SOCKET_ADDR_H_
#include <kob/ksl/string_view.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <cstdint>
#include <cstring>
#include <iosfwd>
#include <string>
#include <vector>

namespace kob {
namespace net {

class socket_addr {
public:
    socket_addr() :_error(0)
    {
        memset(&_sockaddr, 0, sizeof(_sockaddr));
    }

    socket_addr(struct sockaddr* sa, socklen_t salen);

    socket_addr(const ksl::cstring_view &hostport);

    socket_addr(const ksl::cstring_view &host, const ksl::cstring_view &port);

    struct sockaddr * sockaddr()
    {
        return &_sockaddr.generic;
    }

    const struct sockaddr * sockaddr() const
    {
        return &_sockaddr.generic;
    }

    socklen_t socklen() const
    {
        return socklen_t(
                sockaddr()->sa_family == AF_INET ? sizeof(_sockaddr.in) :
                sockaddr()->sa_family == AF_INET6 ? sizeof(_sockaddr.in6) :
                0);
    }

    bool is_valid() const
    {
        return (sockaddr()->sa_family == AF_INET) ||
               (sockaddr()->sa_family == AF_INET6);
    }

    bool is_ipv4() const
    {
        return (sockaddr()->sa_family == AF_INET);
    }

    bool is_ipv6() const
    {
        return (sockaddr()->sa_family == AF_INET6);
    }

    struct sockaddr_in * sockaddr_in()
    {
        return &_sockaddr.in;
    }

    const struct sockaddr_in * sockaddr_in() const
    {
        return &_sockaddr.in;
    }

    struct sockaddr_in6 * sockaddr_in6()
    {
        return &_sockaddr.in6;
    }

    const struct sockaddr_in6 * sockaddr_in6() const
    {
        return &_sockaddr.in6;
    }

    std::string host() const;

    std::string host_port() const;

    uint16_t get_port() const
    {
        if (sockaddr()->sa_family == AF_INET) {
            return ntohs(sockaddr_in()->sin_port);
        } else if (sockaddr()->sa_family == AF_INET6) {
            return ntohs(sockaddr_in6()->sin6_port);
        }
        return 0;
    }

    void set_port(uint16_t port)
    {
        if (sockaddr()->sa_family == AF_INET) {
            sockaddr_in()->sin_port = htons(port);
        } else if (sockaddr()->sa_family == AF_INET6) {
            sockaddr_in6()->sin6_port = htons(port);
        }
    }

    bool resolve(const ksl::cstring_view &hostname,
                 const ksl::cstring_view &servicename = ksl::cstring_view(nullptr));

    static socket_addr resolve_one(const ksl::cstring_view & hostname,
                                   const ksl::cstring_view & servicename = ksl::cstring_view(nullptr));

    static socket_addr resolve_service(const ksl::cstring_view &hostname,
                                       const ksl::cstring_view &defaultservice);

    static std::vector<socket_addr>
    resolve_all(const ksl::cstring_view &hostname, const ksl::cstring_view &servicename = ksl::cstring_view(nullptr));

    const char * get_resolve_error() const;
protected:
    union sock_addr_union {
        struct sockaddr     generic;
        struct sockaddr_in  in;
        struct sockaddr_in6 in6;
    }                     _sockaddr;

private:
    int                   _error;
};

class ipv4_addr : public  socket_addr {
public:
    ipv4_addr() : socket_addr()
    {
        _sockaddr.in.sin_family = AF_INET;
    }

    explicit ipv4_addr(uint16_t port) : socket_addr()
    {
        _sockaddr.in.sin_family = AF_INET;
        _sockaddr.in.sin_port = htons(port);
    }

    ipv4_addr(uint32_t ip, uint16_t port) : socket_addr()
    {
        _sockaddr.in.sin_family = AF_INET;
        _sockaddr.in.sin_addr.s_addr = ip;
        _sockaddr.in.sin_port = htons(port);
    }


    ipv4_addr(const struct in_addr& addr, uint16_t port) : socket_addr()
    {
        _sockaddr.in.sin_family = AF_INET;
        _sockaddr.in.sin_addr = addr;
        _sockaddr.in.sin_port = htons(port);
    }

    ipv4_addr(const struct sockaddr_in& sa) : socket_addr()
    {
        _sockaddr.in = sa;
    }

    ipv4_addr(const ksl::cstring_view &ipstring, uint16_t port = 0);
};

class ipv6_addr : public socket_addr {
public:
    ipv6_addr() : socket_addr()
    {
        _sockaddr.in6.sin6_family = AF_INET6;
    }

    ipv6_addr(uint16_t port) : socket_addr()
    {
        _sockaddr.in6.sin6_family = AF_INET6;
        _sockaddr.in6.sin6_port = htons(port);
    }

    ipv6_addr(uint8_t addr[16], uint16_t port) : socket_addr()
    {
        _sockaddr.in6.sin6_family = AF_INET6;
        memcpy(&_sockaddr.in6.sin6_addr, addr, 16 * sizeof(uint8_t));
        _sockaddr.in6.sin6_port = htons(port);
    }

    ipv6_addr(struct in6_addr& addr, uint16_t port) : socket_addr()
    {
        _sockaddr.in6.sin6_family = AF_INET6;
        _sockaddr.in6.sin6_addr = addr;
        _sockaddr.in6.sin6_port = htons(port);
    }

    ipv6_addr(struct sockaddr_in6& sa) : socket_addr()
    {
        _sockaddr.in6 = sa;
    }

    ipv6_addr(const ksl::cstring_view &ipstring, uint16_t port = 0);
};
}
}
#endif //KOB_NET_SOCKET_ADDR_H_
