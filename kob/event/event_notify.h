//
// Created by 李寅斌 on 2018/11/23.
//

#ifndef KOB_EVENT_EVENT_NOTIFY_H_
#define KOB_EVENT_EVENT_NOTIFY_H_
#include <kob/event/channel.h>
#include <kob/ksl/timestamp.h>

namespace kob {
namespace event {

class event_poller;

class event_notify {
public:
    event_notify();
    ~event_notify();

    bool start(event_poller *p, const event_callback &cb);

    void stop();

    void wake_up();

private:
    void handle_read(ksl::timestamp);
private:
    bool           _active;
    int            _fd;
    channel        _channel;
    event_poller  *_poller;
    event_callback _callback;
};
}
}
#endif //KOB_EVENT_EVENT_NOTIFY_H_
