//
// Created by 李寅斌 on 2018/11/24.
//

#include <kob/event/event_notify.h>
#include <kob/event/event_poller.h>
#include <kob/profile/prof.h>
#include <kob/log/logging.h>
#include <sys/eventfd.h>
#include <errno.h>

namespace kob {
namespace event {

namespace detail {

int create_eventfd()
{
    int evtfd = ::eventfd(0, EFD_NONBLOCK | EFD_CLOEXEC);
    return evtfd;
}

}
event_notify::event_notify()
:_active(false),
 _fd(-1),
 _channel(),
 _poller(nullptr),
 _callback()
{

}
event_notify::~event_notify()
{
    KOB_ASSERT_MSG(!_active, "must be stop");
}

bool event_notify::start(event_poller *p, const event_callback &cb)
{
    KOB_ASSERT_MSG(p, "poller nullptr");
    _poller = p;
    _callback = cb;

    _fd = detail::create_eventfd();
    if(_fd < 0) {
        LOG_ERROR("error to create eventfd errno: {}", errno);
        return false;
    }
    _channel.set(_fd);
    _channel.set_read_callback(std::bind(&event_notify::handle_read, this, std::placeholders::_1));
    _channel.enable_read();
    _poller->update_channel(&_channel);
    _active = true;
    return true;

}

void event_notify::handle_read(ksl::timestamp ts)
{
    LOG_TRACE("event_notify::handle_read");
    uint64_t one = 1;
    ssize_t n = ::read(_fd, &one, sizeof one);
    if (n != sizeof one) {
        LOG_ERROR("event_notify::wakeup() reads {} bytes instead of 8", n);
    }
    if(_callback) {
        _callback(ts);
    }
}

void event_notify::wake_up()
{
    uint64_t one = 1;
    ssize_t n = ::write(_fd, &one, sizeof one);
    if (n != sizeof one) {
        LOG_ERROR("event_notify::wake_up() writes {} bytes instead of 8", n);
    }
}

void event_notify::stop()
{
    _channel.disable_all();
    _poller->update_channel(&_channel);
    ::close(_fd);
    _fd = -1;
    _active = false;
}

} //namespace event
} //namespace kob