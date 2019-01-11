//
// Created by 李寅斌 on 2018/12/2.
//

#ifndef KOB_EVENT_TIMER_NOTIFY_H_
#define KOB_EVENT_TIMER_NOTIFY_H_
#include <kob/event/channel.h>
#include <kob/ksl/timestamp.h>

namespace kob {
namespace event {

class event_poller;

class timer_notify {
public:
    timer_notify();
    ~timer_notify();

    bool start(event_poller *p, const event_callback &cb);

    void stop();

    void reset(ksl::timestamp);

private:
    void handle_read(ksl::timestamp);
private:
    bool           _active;
    int            _fd;
    ksl::timestamp _last_stamp;
    channel        _channel;
    event_poller  *_poller;
    event_callback _callback;
};

} //namespace event
} //namespace kob
#endif //KOB_EVENT_TIMER_NOTIFY_H_
