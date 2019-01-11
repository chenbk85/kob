//
// Created by 李寅斌 on 2018/12/2.
//

#include <kob/event/timer_notify.h>
#include <kob/event/event_poller.h>
#include <kob/log/logging.h>
#include <sys/timerfd.h>

namespace kob {
namespace event {

namespace detail {

int create_timerfd()
{
    int timerfd = ::timerfd_create(CLOCK_MONOTONIC,
                                   TFD_NONBLOCK | TFD_CLOEXEC);
    return timerfd;
}

struct timespec from_now(ksl::timestamp when)
{
    ksl::timespan diff = when -  ksl::timestamp();

    int64_t microseconds = diff.total_microseconds();
    if (microseconds < 100) {
        microseconds = 100;
    }
    struct timespec ts;
    ts.tv_sec = static_cast<time_t>(
            microseconds /  ksl::timestamp::ratio());
    ts.tv_nsec = static_cast<long>(
            (microseconds %  ksl::timestamp::ratio()) * 1000);
    return ts;
}

void reset_timerfd(int timerfd, ksl::timestamp expiration)
{
    struct itimerspec newValue;
    struct itimerspec oldValue;
    bzero(&newValue, sizeof newValue);
    bzero(&oldValue, sizeof oldValue);
    newValue.it_value = from_now(expiration);
    int ret = ::timerfd_settime(timerfd, 0, &newValue, &oldValue);
    if (ret) {
        LOG_ERROR("timerfd_settime()");
    }
}

} //namespace detail

timer_notify::timer_notify()
: _active(false),
  _fd(-1),
  _last_stamp(static_cast<uint64_t>(-1)),
  _channel(),
  _poller(nullptr),
  _callback()
{

}

bool timer_notify::start(event_poller *p, const event_callback &cb)
{
    KOB_ASSERT_MSG(p, "poller nullptr");
    _poller = p;
    _callback = cb;

    _fd = kob::event::detail::create_timerfd();
    if (_fd < 3) {
        LOG_ERROR("create time fd error  errno: ", errno);
        return  false;
    }

    _channel.set(_fd);
    _channel.set_read_callback(std::bind(&timer_notify::handle_read, this, std::placeholders::_1));
    _channel.enable_read();
    _poller->update_channel(&_channel);
    _active = true;
    return true;
}

void timer_notify::handle_read(ksl::timestamp ts)
{
    LOG_TRACE("timer_notify::handle_read");
    uint64_t one = 1;
    ssize_t n = ::read(_fd, &one, sizeof one);
    if (n != sizeof one) {
        LOG_ERROR("timer_notify::wakeup() reads {} bytes instead of 8", n);
    }
    _last_stamp = static_cast<uint64_t>(-1);
    if(_callback) {
        _callback(ts);
    }
}

void timer_notify::reset(ksl::timestamp stamp)
{
    LOG_TRACE("_last_stamp: {}, stamp {}", _last_stamp.raw(), stamp.raw());
    if(stamp < _last_stamp) {
        _last_stamp = stamp;
        kob::event::detail::reset_timerfd(_fd, _last_stamp);
    }

}
void timer_notify::stop()
{
    _channel.disable_all();
    _poller->update_channel(&_channel);
    ::close(_fd);
    _fd = -1;
    _active = false;
}

timer_notify::~timer_notify()
{
    KOB_ASSERT_MSG(!_active, "must be stop");
}

} //namespace event
} //namespace kob