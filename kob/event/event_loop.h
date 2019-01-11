//
// Created by 李寅斌 on 2018/11/25.
//

#ifndef KOB_EVENT_EVENT_LOOP_H_
#define KOB_EVENT_EVENT_LOOP_H_

#include <kob/event/event_base.h>
#include <kob/event/channel.h>
#include <kob/ksl/mutex.h>
#include <kob/ksl/this_thread.h>
#include <kob/ksl/shared_state.h>
#include <kob/event/event_notify.h>
#include <kob/event/timer_notify.h>
#include <kob/event/event_poller.h>
#include <kob/event/timer.h>
#include <kob/event/event_option.h>
#include <kob/event/task.h>
#include <memory>



namespace kob {
namespace event {

namespace detail {
    class add_timer_task;
}

enum class loop_status {
    none,
    running,
    stopping,
    stoped
};

class event_loop;
typedef std::shared_ptr<event_loop> event_loop_ptr;

class event_loop {
public:
    event_loop();

    ~event_loop();

    bool init();

    void loop();

    int32_t id() const;

    void quit();

    void join();

    timer_id run_at(ksl::timestamp stamp, const timer_callback &cb);

    timer_id run_after(ksl::timespan span,const timer_callback &cb);

    timer_id run_every(ksl::timespan span, const timer_callback &cb);

    void set_option(event_option_ptr op);

    void run_task(task_ptr &&t);

    bool has_channel(channel *c);

    void update_channel(channel *c);

    void queue_in_loop(task_ptr &&t);

    void wake_up();

    ksl::timestamp last_poll() const
    {
        return _last_poll;
    }

private:
    friend class kob::event::detail::add_timer_task;
    void add_timer_in_loop(timer_task_ptr &ptr);
    void do_shared_task();
    void do_queued_task();
    void on_event_notify(ksl::timestamp stamp);
    void on_timer_notify(ksl::timestamp stamp);
    void clear_loop();
private:
    task_list                           _local_task;
    ksl::mutex                          _shared_mutex;
    task_list                           _shared_task;
    ksl::this_thread::thread_binder     _thread_binder;
    std::shared_ptr<event_poller>       _poller;
    std::shared_ptr<event_notify>       _notify;
    std::shared_ptr<timer_notify>       _timer_notify;
    timer                               _timer;
    ksl::timestamp                      _last_poll;
    channel_list                        _active_channels;
    channel                            *_current_active_channel;
    event_option_ptr                    _option;
    bool                                _do_shared_task;
    bool                                _do_looping;
    bool                                _quit;
    uint64_t                            _iteration;
    ksl::shared_state<loop_status>      _status;
};


} //namepace event
} //namespace kob
#endif //KOB_EVENT_EVENT_LOOP_H_
