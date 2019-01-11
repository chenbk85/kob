//
// Created by 李寅斌 on 2018/12/3.
//

#include <kob/net/socket_addr.h>
#include <kob/strings/numeric_formatter.h>
#include <kob/log/logging.h>
#include <arpa/inet.h>
#include <netdb.h>

#include <algorithm>
#include <cerrno>
#include <string>
#include <vector>

namespace kob {
namespace  net {

socket_addr::socket_addr(struct sockaddr* sa, socklen_t salen)
: _error(0)
{
    memcpy(&_sockaddr, sa, std::min<socklen_t>(salen, sizeof(_sockaddr)));
}

socket_addr::socket_addr(const ksl::cstring_view &hostport)
{
    size_t colonpos = hostport.rfind(':');
    if (colonpos == ksl::cstring_view::npos) {
        resolve(hostport);
    } else {
        std::string hostStr(hostport.data(), colonpos);
        ksl::cstring_view portStr = hostport;
        portStr.remove_prefix(colonpos + 1);
        resolve(ksl::cstring_view(hostStr.c_str()), portStr);
    }
}

socket_addr::socket_addr(const ksl::cstring_view &hostStr,
            const ksl::cstring_view &portStr)
{
    resolve(hostStr, portStr);
}

std::string socket_addr::host() const
{
    char str[64];
    if (sockaddr()->sa_family == AF_INET) {
        if (inet_ntop(AF_INET,
                      &sockaddr_in()->sin_addr, str, sizeof(str)) == nullptr)
        {
            LOG_ERROR("Error in inet_ntop: {}", strerror(errno));
            return "<error>";
        }
        return str;
    } else if (sockaddr()->sa_family == AF_INET6) {
        if (inet_ntop(AF_INET6,
                      &sockaddr_in6()->sin6_addr, str, sizeof(str)) == nullptr) {
            LOG_ERROR("Error in inet_ntop: {}", strerror(errno));
            return "<error>";
        }
        return str;
    } else {
        return "<invalid>";
    }
}


std::string socket_addr::host_port() const
{
    return host() + ":" + kob::strings::format_uint(get_port());
}

bool socket_addr::resolve(const ksl::cstring_view &hostname,
                 const ksl::cstring_view &servicename)
{
    struct addrinfo* result;
    struct addrinfo hints;

    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_INET;    /* Allow IPv4 or IPv6 */
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = 0;
    hints.ai_protocol = 0;

    int ec = getaddrinfo(hostname.c_str(), servicename.c_str(), &hints, &result);
    if (ec != 0) {
        memset(&_sockaddr, 0, sizeof(_sockaddr));
        _error = ec;
        return false;
    } else {
        *this = socket_addr(result->ai_addr, result->ai_addrlen);
        freeaddrinfo(result);
        return is_valid();
    }
}


socket_addr socket_addr::resolve_one(const ksl::cstring_view & hostname,
                                   const ksl::cstring_view & servicename)
{
    struct addrinfo* result;
    struct addrinfo hints;

    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_INET;    /* Allow IPv4 or IPv6 */
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = 0;
    hints.ai_protocol = 0;

    int s = getaddrinfo(hostname.c_str(), servicename.c_str(), &hints, &result);
    if (s != 0) {
        return socket_addr();
    }

    socket_addr sa(result->ai_addr, result->ai_addrlen);

    freeaddrinfo(result);

    return sa;
}




std::vector<socket_addr>
socket_addr::resolve_all(const ksl::cstring_view &hostname,
        const ksl::cstring_view &servicename)
{

    std::vector<socket_addr> salist;
    struct addrinfo* result;
    struct addrinfo hints;

    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_UNSPEC;    /* Allow IPv4 or IPv6 */
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = 0;
    hints.ai_protocol = 0;

    int s = getaddrinfo(hostname.c_str(), servicename.c_str(), &hints, &result);
    if (s != 0) {
        return salist;
    }

    for (struct addrinfo* ap = result; ap != nullptr; ap = ap->ai_next) {
        salist.push_back(socket_addr(ap->ai_addr, ap->ai_addrlen));
    }

    freeaddrinfo(result);

    return salist;
}

ipv4_addr::ipv4_addr(const ksl::cstring_view &ipstring, uint16_t port)
:socket_addr()
{
    struct sockaddr_in* sin = sockaddr_in();
    sin->sin_family = AF_INET;
    if (inet_pton(AF_INET, ipstring.c_str(), &sin->sin_addr) <= 0) {
        sin->sin_family = 0;
        return;
    }
    sin->sin_port = htons(port);
}

ipv6_addr::ipv6_addr(const ksl::cstring_view &ipstring, uint16_t port)
:socket_addr()
{
    struct sockaddr_in6* sin6 = sockaddr_in6();
    sin6->sin6_family = AF_INET6;
    if (inet_pton(AF_INET6, ipstring.c_str(), &sin6->sin6_addr) <= 0) {
        sin6->sin6_family = 0;
        return;
    }
    sin6->sin6_port = htons(port);
}
const char * socket_addr::get_resolve_error() const
{
    return gai_strerror(_error);
}

} //namespace net
} //namespace kob