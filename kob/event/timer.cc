//
// Created by 李寅斌 on 2018/12/2.
//

#include <kob/event/timer.h>
#include <vector>
#include <memory>

namespace kob {
namespace event {


namespace detail {
    struct null_delete {
        void operator ()(timer_task *ptr)
        {

        }
    };
}
timer::timer(int32_t id)
 : _id(id),
   _tasks(),
   _active_tasks(),
   _do_timers(false),
   _canceled_tasks()
{

}

timer_id timer::add_timer(const timer_callback& cb,
                   ksl::timestamp when,
                   ksl::timespan interval)
{
    LOG_TRACE("{}", interval.total_microseconds());
    timer_task_ptr task= std::make_shared<timer_task>(cb, when, interval);
    _tasks.insert(entry(when, task.get()));
    _active_tasks[task->sequence()] = task;
    return timer_id(task, task->sequence());
}

void timer::add_timer(timer_task_ptr &ptr)
{
    _tasks.insert(entry(ptr->expiration(), ptr.get()));
    _active_tasks[ptr->sequence()] = ptr;
}

void timer::cancel(timer_id id)
{
    active_task_set::iterator it = _active_tasks.find(id.sequeue());
    if(it != _active_tasks.end()) {
        _tasks.erase(entry(it->second->expiration(), it->second.get()));
        _active_tasks.erase(it);
    } else {
        _canceled_tasks[id.sequeue()] = id.task();
    }
}

ksl::timestamp timer::earliest() const
{
    task_list::const_iterator it = _tasks.begin();
    if (it != _tasks.end()) {
        return it->first;
    }
    return ksl::timestamp(0);
}

void timer::do_timers(ksl::timestamp now)
{
    LOG_TRACE("timer::do_timers");
    _do_timers = true;
    _canceled_tasks.clear();
    std::vector<entry> expired;
    entry sentry(now, reinterpret_cast<timer_task*>(0xffffffffffffffff));
    LOG_TRACE("{}", _tasks.size());
    task_list::iterator itTask = _tasks.begin();
    for(; itTask != _tasks.end();itTask++ ) {
        LOG_TRACE("_task : {}, addr {}", itTask->first.raw(), (void*)itTask->second);
    }
    task_list::iterator end = _tasks.lower_bound(sentry);
    std::copy(_tasks.begin(), end, back_inserter(expired));
    _tasks.erase(_tasks.begin(), end);


    LOG_TRACE("timer::do_timers timers :{}", expired.size());
    if(expired.size() == 0) {
        exit(-1);
    }
    for (std::vector<entry>::iterator it = expired.begin();
         it != expired.end(); ++it) {
        it->second->run(now);
        if (it->second->repeat()) {
            it->second->restart(now);
            _tasks.insert(entry(it->second->expiration(), it->second));
        } else {
            _active_tasks.erase(it->second->sequence());
        }
    }
    _do_timers = false;

}

} //namespace event
} //namespace kob
