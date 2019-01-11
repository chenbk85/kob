//
// Created by 李寅斌 on 2018/11/23.
//

#ifndef KOB_EVENT_EVENT_POLLER_H_
#define KOB_EVENT_EVENT_POLLER_H_
#include <kob/ksl/timestamp.h>
#include <vector>
#include <unordered_map>

namespace kob {
namespace event {


class channel;

class event_poller {
public:
    typedef std::vector<channel*> channel_list;
public:
    event_poller();
    virtual ~event_poller();

    virtual bool start() = 0;

    virtual void stop() = 0;

    virtual ksl::timestamp poll(int timeoutMs, channel_list* list) = 0;

    virtual void update_channel(channel *c) = 0;

    virtual bool has_channel(channel *c);

    static event_poller* get_poller();

protected:
    typedef std::unordered_map<int, channel*> channel_map;

    channel_map    _channels;
};


}
}
#endif //KOB_EVENT_EVENT_POLLER_H_
