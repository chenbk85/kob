//
// Created by 李寅斌 on 2018/11/23.
//

#include <kob/event/event_poller.h>
#include <kob/event/channel.h>

namespace kob {
namespace event {

event_poller::event_poller()
{

}
event_poller::~event_poller()
{

}

bool event_poller::has_channel(channel *c)
{
    channel_map::const_iterator it = _channels.find(c->get_fd());
    return it != _channels.end() && it->second == c;
}

} //namespace event
} //namespace kob

#ifdef KOB_PLATFORM_LINUX
    #include <kob/event/detail/epoll_poller.cc>
#endif