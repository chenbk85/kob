//
// Created by 李寅斌 on 2018/11/23.
//

#ifndef KOB_EVENT_DETAIL_EPOLL_POLLER_H_
#define KOB_EVENT_DETAIL_EPOLL_POLLER_H_
#include <kob/event/event_poller.h>
#include <kob/profile/prof.h>
#include <vector>
#include <poll.h>
#include <sys/epoll.h>

namespace kob {
namespace event {

class epoll_poller : public event_poller {
public:
    epoll_poller();
    ~epoll_poller() KOB_OVERRIDE;

    ksl::timestamp poll(int timeoutMs, channel_list* list) KOB_OVERRIDE;

    void update_channel(channel *c) KOB_OVERRIDE;

    bool start() KOB_OVERRIDE;

    void stop() KOB_OVERRIDE;

private:
    void do_update(channel *c);
    void do_add(channel *c);
    void do_remove(channel *c);
    void epoll_opt(int op, channel *c);

    void fill_channels(int numEvents,
                                     channel_list* activeChannels) const;
    static const size_t kInitListSize = 32;
    typedef std::vector<struct epoll_event> event_list;
    event_list   _events;
    int          _epfd;
};

} //namespace event
} //namespace kob
#endif //KOB_EVENT_DETAIL_EPOLL_POLLER_H_
