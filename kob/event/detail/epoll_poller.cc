//
// Created by 李寅斌 on 2018/11/23.
//

#include <kob/event/detail/epoll_poller.h>
#include <kob/log/logging.h>
#include <kob/event/event.h>
#include <errno.h>
#include <poll.h>
#include <sys/epoll.h>
#include <unistd.h>
#include <cstring>

namespace kob {
namespace event {

epoll_poller::epoll_poller()
:   _events(kInitListSize),
    _epfd(-1)
{

}

epoll_poller::~epoll_poller()
{
    stop();
}

bool epoll_poller::start()
{
    _epfd = ::epoll_create1(EPOLL_CLOEXEC);
    return _epfd > 0 ? true : false;
}

void epoll_poller::stop()
{
    if(_epfd > 0) {
        ::close(_epfd);
        _epfd = -1;
    }
}

void epoll_poller::update_channel(channel *c)
{
    event_opt eo = c->get_opt();
    LOG_TRACE("opt : {}", static_cast<int>(eo));
    if (eo == event_opt::add) {
        do_add(c);
    } else if (eo == event_opt::ctl) {
        do_update(c);
    } else if(eo == event_opt::del) {
        do_remove(c);
    }
    c->ack_opt(errno);
}

void epoll_poller::do_update(channel *c)
{
    epoll_opt(EPOLL_CTL_MOD, c);
}

void epoll_poller::do_add(channel *c)
{
    int fd = c->get_fd();
    _channels[fd] = c;
    epoll_opt(EPOLL_CTL_ADD, c);
}
void epoll_poller::do_remove(channel *c)
{
    int fd = c->get_fd();
    _channels.erase(fd);
    epoll_opt(EPOLL_CTL_DEL, c);
}

void epoll_poller::epoll_opt(int op, channel *c)
{
    struct epoll_event event;
    bzero(&event, sizeof event);
    event.events = static_cast<int>(c->get_opt_event());
    event.data.ptr = c;
    int fd = c->get_fd();
    if (::epoll_ctl(_epfd, op, fd, &event) != 0) {
        LOG_ERROR("epoll ctl {} error efd :{} fd : {} op : {}", errno, _epfd, fd, op);
    }
}

ksl::timestamp epoll_poller::poll(int timeoutMs, channel_list* list)
{
    int numEvents = ::epoll_wait(_epfd,
                                 &*_events.begin(),
                                 static_cast<int>(_events.size()),
                                 timeoutMs);
    int savedErrno = errno;
    ksl::timestamp now;
    if (numEvents > 0) {
        fill_channels(numEvents, list);
        if (numEvents == static_cast<int>(_events.size())) {
            _events.resize(_events.size()*2);
        }
    } else if (numEvents < 0) {
        if (savedErrno != EINTR) {
            errno = savedErrno;
            LOG_ERROR("EPollpoller::poll() errno: {}", savedErrno);
        }
    }
    return now;
}

void epoll_poller::fill_channels(int numEvents,
                                channel_list* activeChannels) const
{
    KOB_ASSERT_MSG(numEvents <= (int)_events.size(), "overflow");
    for (int i = 0; i < numEvents; ++i) {
        channel* c = static_cast<channel*>(_events[i].data.ptr);
        c->set_active_event(static_cast<poll_event>(_events[i].events));
        activeChannels->push_back(c);
    }
}
event_poller* event_poller::get_poller()
{
    return new epoll_poller();
}

} //namespace event
} //namespace kob