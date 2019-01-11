//
// Created by 李寅斌 on 2018/11/22.
//

#include <kob/event/channel.h>
#include <kob/log/logging.h>

namespace kob {
namespace event {

namespace detail {

void  empty_read_callback(ksl::timestamp s)
{
    LOG_WARN("empty_read_callback");
    KOB_UNUSED(s);
}

void  empty_write_callback(ksl::timestamp s)
{
    LOG_WARN("empty_write_callback");
    KOB_UNUSED(s);
}

void  empty_error_callback(ksl::timestamp s)
{
    LOG_WARN("empty_error_callback");
    KOB_UNUSED(s);
}

void  empty_close_callback(ksl::timestamp s)
{
    LOG_WARN("empty_close_callback");
    KOB_UNUSED(s);
}

} //namespace detail

const event_callback channel::kEmptyReadCallBack =
        std::bind(&detail::empty_read_callback, std::placeholders::_1);

const event_callback channel::kEmptyWriteCallBack =
        std::bind(&detail::empty_write_callback, std::placeholders::_1);

const event_callback channel::kEmptyCloseCallBack =
        std::bind(&detail::empty_close_callback, std::placeholders::_1);

const event_callback channel::kEmptyErrorCallBack =
        std::bind(&detail::empty_error_callback, std::placeholders::_1);

channel::channel()
{
    clear();
}

channel::~channel()
{

}
void channel::clear()
{
    _do_event       = false;
    _tie            = std::shared_ptr<void>();
    _tied           = false;
    _op_event       = poll_event::none;
    _active_event   = poll_event::none;
    _status         = event_status::none;
    _fd             = -1;
    _log_hup        = true;
    _saved_error    = 0;
    _read_callback  = kEmptyReadCallBack;
    _write_callback = kEmptyWriteCallBack;
    _error_callback = kEmptyErrorCallBack;
    _close_callback = kEmptyCloseCallBack;
}

void channel::set(int fd)
{
    KOB_ASSERT_MSG(fd > 2, "fd not valid");
    KOB_ASSERT_MSG(!is_managed(_status), "alread in use");
    _fd = fd;
}

void channel::ack_opt(int ec)
{
    if(ec) {
        _saved_error = ec;
        LOG_WARN("error no: {}", ec);
        _error_callback(ksl::timestamp::now());
        return;
    }

    //first time add to reactor
    if (_opt == event_opt::add) {
        set_managed(_status);

    } else if (_opt == event_opt::del) {
        detach_managed(_status);
    }

    set_event_status(_op_event, _status);
    _op_event = poll_event::none;
    _opt = event_opt::none;
}

void channel::tie(const std::shared_ptr<void> &ob)
{
    _tie = ob;
    _tied = true;
}
void channel::handle_event(ksl::timestamp stamp)
{
    std::shared_ptr<void> guard;
    if (_tied) {
        guard = _tie.lock();
        if (guard) {
            do_handle_event(stamp);
        }
    } else {
        do_handle_event(stamp);
    }
}

void channel::do_handle_event(ksl::timestamp stamp)
{
    _do_event = true;
    if (is_event_hup(_active_event) && !is_event_read(_active_event)) {
        if (_log_hup) {
            LOG_WARN("event hup");
        }
        _close_callback(stamp);
    }

    if(is_event_nval(_active_event)) {
        LOG_WARN("nval event, not valid fd");
    }

    if(is_event_error(_active_event) || is_event_nval(_active_event)) {
        _error_callback(stamp);
    }

    if(is_event_read(_active_event) ||
        is_event_read_pri(_active_event) ||
            is_event_rd_hup(_active_event)) {
        _read_callback(stamp);
    }

    if(is_event_write(_active_event)) {
        _write_callback(stamp);
    }

    _do_event = false;
}

} //namespace event
} //namespace kob