//
// Created by 李寅斌 on 2018/12/4.
//

#ifndef KOB_NET_TCP_SOCKET_OPTION_H_
#define KOB_NET_TCP_SOCKET_OPTION_H_
#include <kob/profile/prof.h>
#include <kob/net/socket_option.h>
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

    bool inet_no_block(int fd);

    bool inet_block(int fd);


    bool inet_shutdown_read(int fd);

    bool inet_shutdown_write(int fd);

    bool inet_shutdown_all(int fd);

    bool inet_close(int fd);

    bool inet_get_sockopt(int fd, int level, int optname,
                     void *optval, socklen_t *optlen);

    bool inet_set_sockopt(int fd, int level, int optname,
                     const void *optval, socklen_t optlen);


    bool inet_keep_alive(int fd);

    bool inet_no_keep_alive(int fd);


    bool inet_reuse_addr(int fd);

    bool inet_no_reuse_addr(int fd);

    bool inet_reuse_port(int fd);


    bool inet_no_reuse_port(int fd);

    bool inet_no_solinger(int fd, int seconds);

    bool inet_solinger(int fd, int seconds);

    bool inet_quick_ack(int fd);

    bool inet_no_quick_ack(int fd);

    bool socket_time_option(int fd, int option, int milliseconds);

    bool inet_no_delay(int fd);

    bool inet_delay(int fd);


    bool inet_sndbuf(int fd, size_t size);


    bool inet_rcvbuf(int fd, size_t size);

#ifdef KOB_PLATFORM_LINUX
        bool get_tcpinfo(int fd, struct tcp_info*) ;
        bool get_tcpinfo_string(int fd, char* buf, int len) ;
#endif

    bool socket_send_timeout(int fd, ksl::timespan &span);

    bool socket_recv_timeout(int fd, ksl::timespan &span);


} //namespace net
} //namespace kob
#endif //KOB_NET_TCP_SOCKET_OPTION_H_
