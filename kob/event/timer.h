//
// Created by 李寅斌 on 2018/11/29.
//

#ifndef KOB_EVENT_TIMER_H_
#define KOB_EVENT_TIMER_H_
#include <kob/profile/prof.h>
#include <kob/event/timer_base.h>
#include <cstdint>
#include <set>

namespace kob {
namespace event {

class timer {
public:
    timer(int32_t id);

    ~timer() = default;

    timer_id add_timer(const timer_callback& cb,
                     ksl::timestamp when,
                     ksl::timespan interval);

    void add_timer(timer_task_ptr &ptr);

    void cancel(timer_id timerId);

    void do_timers(ksl::timestamp now);

    ksl::timestamp earliest() const;

private:
    typedef std::pair<ksl::timestamp, timer_task*> entry;
    typedef std::set<entry> task_list;
    typedef std::unordered_map<uint64_t, timer_task_ptr> active_task_set;

private:
    int32_t           _id;
    task_list         _tasks;
    active_task_set   _active_tasks;
    bool              _do_timers;
    active_task_set   _canceled_tasks;
};
} //namespace event
} //namespace kob

#endif //KOB_EVENT_TIMER_H_
