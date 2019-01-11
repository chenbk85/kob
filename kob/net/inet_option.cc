//
// Created by 李寅斌 on 2018/12/4.
//
#include <kob/net/inet_option.h>
#include <kob/profile/prof.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/tcp.h>
#include <unistd.h>
#include <net/if_arp.h>
namespace kob {
namespace net {

    bool inet_no_block(int fd)
    {
        KOB_ASSERT_MSG(fd > 0, "invald fd");
        bool rc = false;

        int flags = fcntl(fd, F_GETFL, NULL);
        if (flags >= 0) {
            flags |= O_NONBLOCK;  // set nonblocking
            if (::fcntl(fd, F_SETFL, flags) >= 0) {
                rc = true;
            }
        }

        return rc;
    }

    bool inet_block(int fd)
    {
        KOB_ASSERT_MSG(fd > 0, "invald fd");
        bool rc = false;

        int flags = fcntl(fd, F_GETFL, NULL);
        if (flags >= 0) {
            flags &= ~O_NONBLOCK; // clear nonblocking

            if (::fcntl(fd, F_SETFL, flags) >= 0) {
                rc = true;
            }
        }

        return rc;
    }


    bool inet_shutdown_read(int fd)
    {
        KOB_ASSERT_MSG(fd > 0, "invald fd");
        return ::shutdown(fd, SHUT_RD) == 0;
    }

    bool inet_shutdown_write(int fd)
    {
        KOB_ASSERT_MSG(fd > 0, "invald fd");
        return ::shutdown(fd, SHUT_WR) == 0;
    }

    bool inet_shutdown_all(int fd)
    {
        KOB_ASSERT_MSG(fd > 0, "invald fd");
        return ::shutdown(fd, SHUT_RDWR) == 0;
    }

    bool inet_close(int fd)
    {
        KOB_ASSERT_MSG(fd > 0, "invald fd");
        return ::close(fd) == 0;
    }

    bool inet_get_sockopt(int fd, int level, int optname,
                     void *optval, socklen_t *optlen)
    {
        KOB_ASSERT_MSG(fd > 0, "invald fd");
        int r = ::getsockopt(fd, level, optname, optval, optlen);
        return r == 0;
    }

    bool inet_set_sockopt(int fd, int level, int optname,
                     const void *optval, socklen_t optlen)
    {
        KOB_ASSERT_MSG(fd > 0, "invald fd");
        int r = ::setsockopt(fd, level, optname, optval, optlen);
        return r == 0;
    }


    bool inet_keep_alive(int fd)
    {
        KOB_ASSERT_MSG(fd > 0, "invald fd");
        int sockOptFlag = 1;
        if (::setsockopt(fd, SOL_SOCKET, SO_KEEPALIVE,
                         &sockOptFlag, sizeof(sockOptFlag)) != 0) {
            return false;
        }
        return true;
    }

    bool inet_no_keep_alive(int fd)
    {
        KOB_ASSERT_MSG(fd > 0, "invald fd");
        int sockOptFlag = 0;
        if (::setsockopt(fd, SOL_SOCKET, SO_KEEPALIVE,
                         &sockOptFlag, sizeof(sockOptFlag)) != 0) {
            return false;
        }
        return true;
    }

    bool inet_reuse_addr(int fd)
    {
        int sockoptflag = 1;
        if (::setsockopt(fd, SOL_SOCKET, SO_REUSEADDR,
                         &sockoptflag, sizeof(sockoptflag)) != 0) {
            return false;
        }
        return true;
    }

    bool inet_no_reuse_addr(int fd)
    {
        int sockoptflag = 0;
        if (::setsockopt(fd, SOL_SOCKET, SO_REUSEADDR,
                         &sockoptflag, sizeof(sockoptflag)) != 0) {
            return false;
        }
        return true;
    }


    bool inet_reuse_port(int fd)
    {
        int sockoptflag = 1;
        if (::setsockopt(fd, SOL_SOCKET, SO_REUSEPORT,
                         &sockoptflag, sizeof(sockoptflag)) != 0) {
            return false;
        }
        return true;
    }

    bool inet_no_reuse_port(int fd)
    {
        int sockoptflag = 0;
        if (::setsockopt(fd, SOL_SOCKET, SO_REUSEPORT,
                         &sockoptflag, sizeof(sockoptflag)) != 0) {
            return false;
        }
        return true;
    }

    bool inet_solinger(int fd, int seconds)
    {
        bool rc = false;
        struct linger lingerTime;
        lingerTime.l_onoff = 1;
        lingerTime.l_linger = seconds;
        rc = (setsockopt(fd, SOL_SOCKET, SO_LINGER, (const void *) (&lingerTime), sizeof(lingerTime)) == 0);
        return rc;
    }

    bool inet_no_solinger(int fd, int seconds) {
        bool rc = false;
        struct linger lingerTime;
        lingerTime.l_onoff = 0;
        lingerTime.l_linger = seconds;
        rc = (setsockopt(fd, SOL_SOCKET, SO_LINGER, (const void *) (&lingerTime), sizeof(lingerTime)) == 0);
        return rc;
    }

    bool inet_quick_ack(int fd) {
        bool rc = false;
#ifdef KOB_PLATFORM_LINUX
        int quickAckInt = 1;
    rc = (::setsockopt(fd, IPPROTO_TCP, TCP_QUICKACK,
          (const void *)(&quickAckInt), sizeof(quickAckInt)) == 0);
#endif
        return rc;
    }

    bool inet_no_quick_ack(int fd) {
        bool rc = false;
#ifdef KOB_PLATFORM_LINUX
        int quickAckInt = 0;
    rc = (setsockopt(fd, IPPROTO_TCP, TCP_QUICKACK,
                     (const void *)(&quickAckInt), sizeof(quickAckInt)) == 0);
#endif
        return rc;
    }

    bool inet_time_option(int fd, int option, int milliseconds) {
        bool rc = false;
        struct timeval timeout;
        timeout.tv_sec = (int) (milliseconds / 1000);
        timeout.tv_usec = (milliseconds % 1000) * 1000000;
        rc = (::setsockopt(fd, SOL_SOCKET, option,
                           (const void *) (&timeout), sizeof(timeout)) == 0);
        return rc;
    }


    bool inet_no_delay(int fd) {
        assert(fd > 0);
        int sockoptflag = 1;
        if (::setsockopt(fd, IPPROTO_TCP, TCP_NODELAY,
                         &sockoptflag, sizeof(sockoptflag)) != 0) {
            return false;
        }
        return true;
    }

    bool inet_delay(int fd)
    {
        assert(fd > 0);
        int sockoptflag = 0;
        if (::setsockopt(fd, IPPROTO_TCP, TCP_NODELAY,
                         &sockoptflag, sizeof(sockoptflag)) != 0) {
            return false;
        }
        return true;
    }

//! Set SO_SNDBUF socket option.
    bool inet_sndbuf(int fd, size_t size)
    {
        assert(fd > 0);
        int sockoptflag = static_cast<int>(size);
        if (::setsockopt(fd, SOL_SOCKET, SO_SNDBUF,
                         &sockoptflag, sizeof(sockoptflag)) != 0) {
            return false;
        }
        return true;
    }


    bool inet_rcvbuf(int fd, size_t size)
    {
        int sockoptflag = static_cast<int>(size);
        if (::setsockopt(fd, SOL_SOCKET, SO_RCVBUF,
                         &sockoptflag, sizeof(sockoptflag)) != 0) {
            return false;
        }
        return true;
    }

#ifdef KOB_PLATFORM_LINUX
    bool get_tcpinfo(int fd, struct ::tcp_info *tcpi)
    {
        socklen_t len = sizeof(struct ::tcp_info);
        bzero(tcpi, len);
        return ::getsockopt(fd, SOL_TCP, TCP_INFO, tcpi, &len) == 0;
    }

    bool get_tcpinfo_string(int fd, char* buf, int len)
    {
        struct ::tcp_info tcpi;
        bool ok = get_tcpinfo(fd, &tcpi);
        if (ok) {
            snprintf(buf, len, "unrecovered=%u "
                 "rto=%u ato=%u snd_mss=%u rcv_mss=%u "
                 "lost=%u retrans=%u rtt=%u rttvar=%u "
                 "sshthresh=%u cwnd=%u total_retrans=%u",
                 tcpi.tcpi_retransmits,  // Number of unrecovered [RTO] timeouts
                 tcpi.tcpi_rto,          // Retransmit timeout in usec
                 tcpi.tcpi_ato,          // Predicted tick of soft clock in usec
                 tcpi.tcpi_snd_mss,
                 tcpi.tcpi_rcv_mss,
                 tcpi.tcpi_lost,         // Lost packets
                 tcpi.tcpi_retrans,      // Retransmitted packets out
                 tcpi.tcpi_rtt,          // Smoothed round trip time in usec
                 tcpi.tcpi_rttvar,       // Medium deviation
                 tcpi.tcpi_snd_ssthresh,
                 tcpi.tcpi_snd_cwnd,
                 tcpi.tcpi_total_retrans);
        }
        return ok;
    }
#endif

    bool inet_send_timeout(int fd, ksl::timespan &span)
    {
        struct timeval tv;
        tv.tv_sec = (long) span.total_seconds();
        tv.tv_usec = (long) span.useconds();
        return ::setsockopt(fd, SOL_SOCKET, SO_SNDTIMEO, (const void *) (&tv), sizeof(tv)) == 0;
    }

    bool inet_recv_timeout(int fd, ksl::timespan &span)
    {
        struct timeval tv;
        tv.tv_sec = (long) span.total_seconds();
        tv.tv_usec = (long) span.useconds();
        return ::setsockopt(fd, SOL_SOCKET, SO_RCVTIMEO, (const void *) (&tv), sizeof(tv)) == 0;
    }


} //namespace net
} //namespace kob