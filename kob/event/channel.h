//
// Created by 李寅斌 on 2018/11/22.
//

#ifndef KOB_EVENT_CHANNEL_H_
#define KOB_EVENT_CHANNEL_H_
#include <kob/ksl/timestamp.h>
#include <kob/profile/prof.h>
#include <kob/event/event.h>
#include <kob/event/event_base.h>
#include <kob/log/logging.h>
#include <functional>
#include <memory>
#include <vector>

namespace kob {
namespace event {

class channel;
typedef std::vector<channel*> channel_list;

class channel {
public:
    static const event_callback kEmptyReadCallBack;

    static const event_callback kEmptyWriteCallBack;
    static const event_callback kEmptyCloseCallBack;
    static const event_callback kEmptyErrorCallBack;

public:
    channel();
    ~channel();

    void set(int fd);

    void handle_event(ksl::timestamp stamp);

    void clear();

    void set_read_callback(const event_callback &cb);

    void set_write_callback(const event_callback &cb);

    void set_close_callback(const event_callback &cb);

    void set_error_callback(const event_callback &cb);

    void enable_read();

    void enable_write();

    void enable_all();

    void disable_read();

    void disable_write();

    void disable_all();

    int get_error() const;

    void ack_opt(int ec);

    void set_active_event(poll_event e);

    bool is_handling_event() const;

    void enable_log_hup();

    void disable_log_hup();

    int get_fd() const;

    bool is_log_hup() const;

    event_opt get_opt() const;

    poll_event get_opt_event() const;

    event_status get_status() const;

    void tie(const std::shared_ptr<void>&);
private:
    void do_handle_event(ksl::timestamp stamp);
private:
    poll_event             _op_event;
    poll_event             _active_event;
    event_status           _status;
    std::weak_ptr<void>    _tie;
    bool                   _tied;
    bool                   _do_event;
    bool                   _log_hup;
    event_opt              _opt;
    int                    _fd;
    int                    _saved_error;
    event_callback         _read_callback;
    event_callback         _write_callback;
    event_callback         _error_callback;
    event_callback         _close_callback;
};



KOB_FORCE_INLINE int channel::get_error() const
{
    return _saved_error;
}

KOB_FORCE_INLINE void channel::enable_read()
{
    KOB_ASSERT_MSG(!is_error_curr(_status), "error do not deal");
    if (is_read_set(_status)) {
        return;
    }
    _op_event = merge_from_status(_status);
    _op_event |= poll_event::read;
    if(!is_managed(_status)) {
        _opt = event_opt::add;
    } else {
        _opt = event_opt::ctl;
    }
}

KOB_FORCE_INLINE void channel::enable_write()
{
    KOB_ASSERT_MSG(!is_error_curr(_status), "error do not deal");
    if (is_write_set(_status)) {
        return;
    }
    _op_event = merge_from_status(_status);
    _op_event |= poll_event::write;
    if(is_managed(_status)) {
        _opt = event_opt::add;
    } else {
        _opt = event_opt::ctl;
    }
}

KOB_FORCE_INLINE void channel::set_read_callback(const event_callback &cb)
{
    _read_callback = cb;
}

KOB_FORCE_INLINE void channel::set_write_callback(const event_callback &cb)
{
    _write_callback = cb;
}

KOB_FORCE_INLINE void channel::set_close_callback(const event_callback &cb)
{
    _close_callback = cb;
}

KOB_FORCE_INLINE void channel::set_error_callback(const event_callback &cb)
{
    _error_callback = cb;
}

KOB_FORCE_INLINE void channel::disable_read()
{
    KOB_ASSERT_MSG(!is_error_curr(_status), "error do not deal");
    KOB_ASSERT_MSG(is_managed(_status), "do not in reactor");
    if (!is_read_set(_status)) {
        return;
    }
    _op_event = merge_from_status(_status);
    _op_event &= ~poll_event::read;
    if(is_event_no_rdwr(_op_event)) {
        _opt = event_opt::del;
        return ;
    }
    _opt = event_opt::ctl;
}

KOB_FORCE_INLINE void channel::disable_write()
{
    KOB_ASSERT_MSG(!is_error_curr(_status), "error do not deal");
    KOB_ASSERT_MSG(is_managed(_status), "do not in reactor");
    if (!is_write_set(_status)) {
        return;
    }

    _op_event = merge_from_status(_status);
    _op_event &= ~poll_event::write;
    if(is_event_no_rdwr(_op_event)) {
        _opt = event_opt::del;
        return ;
    }
    _opt = event_opt::ctl;
}

KOB_FORCE_INLINE void channel::enable_all()
{
    KOB_ASSERT_MSG(!is_error_curr(_status), "error do not deal");
    if (is_write_set(_status) && is_read_set(_status)) {
        return;
    }

    _op_event = merge_from_status(_status);
    _op_event =  _op_event | poll_event::write;
    _op_event = _op_event | poll_event::read;
    if(is_managed(_status)) {
        _opt = event_opt::add;
    } else {
        _opt = event_opt::ctl;
    }
}

KOB_FORCE_INLINE void channel::disable_all()
{
    KOB_ASSERT_MSG(!is_error_curr(_status), "error do not deal");
    KOB_ASSERT_MSG(is_managed(_status), "do not in reactor");
    if (!is_write_set(_status) && !is_read_set(_status)) {
        LOG_TRACE("disable_all nothing todo");
        return;
    }
    _op_event = merge_from_status(_status);
    _op_event &=  ~poll_event::write;
    _op_event &=  ~poll_event::read;
    _opt = event_opt::del;
}

KOB_FORCE_INLINE void channel::set_active_event(poll_event e)
{
    _active_event = e;
}

KOB_FORCE_INLINE bool channel::is_handling_event() const
{
    return _do_event;
}

KOB_FORCE_INLINE event_opt channel::get_opt() const
{
    return _opt;
}

KOB_FORCE_INLINE poll_event channel::get_opt_event() const
{
    return _op_event;
}

KOB_FORCE_INLINE event_status channel::get_status() const
{
    return _status;
}

KOB_FORCE_INLINE void channel::enable_log_hup()
{
    _log_hup = true;
}

KOB_FORCE_INLINE void channel::disable_log_hup()
{
    _log_hup = false;
}

KOB_FORCE_INLINE bool channel::is_log_hup() const
{
    return _log_hup;
}

KOB_FORCE_INLINE int channel::get_fd() const
{
    return _fd;
}

} //namespace event
} //namespace kob
#endif //KOB_EVENT_CHANNEL_H_
