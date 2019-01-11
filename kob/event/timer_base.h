//
// Created by 李寅斌 on 2018/11/23.
//

#ifndef KOB_EVENT_TIMER_TASK_H_
#define KOB_EVENT_TIMER_TASK_H_

#include <kob/ksl/timestamp.h>
#include <kob/ksl/timespan.h>
#include <kob/profile/prof.h>
#include <kob/log/logging.h>
#include <functional>
#include <atomic>
#include <cstdint>
#include <memory>

namespace kob {
namespace event {


typedef std::function<void(ksl::timestamp)> timer_callback;
class timer_task {
public:
    timer_task(const timer_callback &cb,
               const ksl::timestamp &when,
               const ksl::timespan &interval)
    : _callback(cb),
      _expiration(when),
      _interval(interval),
      _repeat(interval > 0),
      _sequence(g_timer_seq.fetch_add(1))
    {
        LOG_TRACE("_interval : {} repeat : {}", _interval.total_microseconds(), _repeat);
    }

    void run(ksl::timestamp stamp);

    ksl::timestamp& expiration();

    bool repeat() const;

    uint64_t sequence() const;

    void restart(const ksl::timestamp &now);

    ksl::timespan step() const;

    uint64_t total_task_create() const
    {
        return g_timer_seq.load();
    }

private:

    const timer_callback  _callback;
    ksl::timestamp        _expiration;
    const ksl::timespan   _interval;
    const bool            _repeat;
    const uint64_t        _sequence;
    static std::atomic<uint64_t> g_timer_seq;

};

KOB_FORCE_INLINE ksl::timestamp& timer_task::expiration()
{
    return _expiration;
}


KOB_FORCE_INLINE bool timer_task::repeat() const
{
    return _repeat;
}


KOB_FORCE_INLINE uint64_t timer_task::sequence() const
{
    return _sequence;
}

KOB_FORCE_INLINE ksl::timespan timer_task::step() const
{
    return _interval;
}

KOB_FORCE_INLINE void timer_task::run(ksl::timestamp stamp)
{
    _callback(stamp);
}

KOB_FORCE_INLINE void timer_task::restart(const ksl::timestamp &now)
{
    if (_repeat) {
        _expiration = now + _interval;
        LOG_TRACE("now {}, _expiration {}, interval {}", now.raw(),
                _expiration.raw(), _interval.total_microseconds());
    } else {
        _expiration = ksl::timestamp();
    }
}

typedef std::shared_ptr<timer_task> timer_task_ptr;


class timer_id {
public:
    timer_id(timer_task_ptr t, uint64_t seq)
            :_task(t),
             _sequeue(seq)
    {

    }

    ~timer_id() = default;

    timer_id()
    :_task(nullptr),
     _sequeue(0)
    {

    }

    uint64_t sequeue() const
    {
        return  _sequeue;
    }

    timer_task_ptr task() const
    {
        return  _task;
    }

private:
    timer_task_ptr  _task;
    uint64_t        _sequeue;

};

} //namespace event
} //namespace kob
#endif //KOB_EVENT_TIMER_TASK_H_
