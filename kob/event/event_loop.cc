//
// Created by 李寅斌 on 2018/11/25.
//
#include <kob/event/event_loop.h>
#include <kob/log/logging.h>

namespace kob {
namespace event {


namespace detail {
    __thread event_loop *_current_loop = nullptr;

    class add_timer_task : public task {
    public:
        add_timer_task(const timer_task_ptr &ptr, event_loop *loop)
                : task(loop->id()),
                  _ptr(ptr),
                  _loop(loop)
        {

        }

        void run() KOB_OVERRIDE
        {
            _loop->add_timer_in_loop(_ptr);
        }

    private:
        timer_task_ptr _ptr;
        event_loop *_loop;
    };


    class update_channel_task : public task {
    public:
        update_channel_task(event_poller *p, channel *c, int32_t id)
                : task(id),
                  _poller(p),
                  _c(c) {

        }

        virtual void run() KOB_OVERRIDE {
            _poller->update_channel(_c);
        }

    private:
        event_poller *_poller;
        channel *_c;
    };
} //namespace detail

event_loop::event_loop()
: _local_task(),
  _shared_mutex(),
  _shared_task(),
  _thread_binder(),
  _poller(),
  _notify(),
  _timer_notify(),
  _timer(_thread_binder.id()),
  _last_poll(),
  _active_channels(),
  _current_active_channel(nullptr),
  _option(defualt_option()),
  _do_shared_task(false),
  _do_looping(false),
  _quit(false),
  _iteration(0UL),
  _status(loop_status::none)
{

}


bool event_loop::init()
{
    _thread_binder.abort_not_in_thread();
    _poller.reset(event_poller::get_poller());
    if (!_poller) {
        return false;
    }

    if(!_poller->start()) {
        return false;
    }
    _notify = std::make_shared<event_notify>();
    if(!_notify) {
        return false;
    }

    if(!_notify->start(_poller.get(),
            std::bind(&event_loop::on_event_notify, this, std::placeholders::_1))) {
        return false;
    }

    _timer_notify = std::make_shared<timer_notify>();
    if(!_timer_notify->start(_poller.get(),
                       std::bind(&event_loop::on_timer_notify, this, std::placeholders::_1))) {
        return false;
    }

    detail::_current_loop = this;
    return true;
}

void event_loop::on_event_notify(ksl::timestamp stamp)
{
    KOB_UNUSED(stamp);
}

void event_loop::on_timer_notify(ksl::timestamp stamp)
{
    LOG_TRACE("event_loop::on_timer_notify");
    _timer.do_timers(stamp);
    ksl::timestamp next = _timer.earliest();
    if ( next >= ksl::timestamp()) {
        LOG_TRACE("do reset timer");
        _timer_notify->reset(next);
    }

}

void event_loop::set_option(event_option_ptr op)
{
    _option = op;
}

event_loop::~event_loop()
{
    LOG_TRACE("event_loop::~event_loop");
}

void event_loop::quit()
{
    LOG_TRACE("event_loop::quit");
    _quit = true;

}

void event_loop::join()
{
    LOG_TRACE("event_loop::join");
    _status.wait(loop_status::stoped);
}
int32_t event_loop::id() const
{
    return _thread_binder.id();
}

void event_loop::queue_in_loop(task_ptr &&t)
{
    if (_thread_binder.is_in_thread()) {
        _local_task.push_back(t);
    } else {
        ksl::unique_guard<ksl::mutex> lk(_shared_mutex);
        _shared_task.push_back(std::move(t));
    }
    wake_up();
}

bool event_loop::has_channel(channel *c)
{
    return _poller->has_channel(c);
}

void event_loop::run_task(task_ptr &&t)
{
    if (_thread_binder.is_in_thread()) {
        t->run();
    } else {
        queue_in_loop(std::move(t));
    }
}


void event_loop::do_shared_task()
{
    task_list functors;
    _do_shared_task = true;

    {
        ksl::unique_guard<ksl::mutex> lock(_shared_mutex);
        functors.swap(_shared_task);
    }

    for (size_t i = 0; i < functors.size(); ++i) {
        functors[i]->run();

    }
    _do_shared_task = false;
}


void event_loop::update_channel(channel *c)
{
    if (_thread_binder.is_in_thread()) {
        _poller->update_channel(c);
    } else {
        task_ptr t(new detail::update_channel_task(_poller.get(), c, id()));
        queue_in_loop(std::move(t));
    }
}

void event_loop::do_queued_task()
{
    task_list functors;
    functors.swap(_local_task);

    for (size_t i = 0; i < functors.size(); ++i) {
        functors[i]->run();
    }
    _do_shared_task = false;
}

void event_loop::wake_up()
{
    LOG_TRACE("event_loop::wake_up");
    _notify->wake_up();
}

timer_id event_loop::run_at(ksl::timestamp stamp, const timer_callback &cb)
{

    if(_thread_binder.is_in_thread()) {
        timer_id rid = _timer.add_timer(cb, stamp, 0);
        ksl::timestamp min = _timer.earliest();
        _timer_notify->reset(min);
        return rid;
    } else {
        timer_task_ptr task= std::make_shared<timer_task>(cb, stamp, 0);
        task_ptr t(new detail::add_timer_task(task, this));
        run_task(std::move(t));
        return timer_id(task, task->sequence());
    }

}

timer_id event_loop::run_after(ksl::timespan span, const timer_callback &cb)
{
    ksl::timestamp runTime = ksl::timestamp::now() + span;
    if(_thread_binder.is_in_thread()) {
        timer_id rid = _timer.add_timer(cb, runTime, 0);
        ksl::timestamp min = _timer.earliest();
        _timer_notify->reset(min);
        return rid;
    } else {
        timer_task_ptr task= std::make_shared<timer_task>(cb, runTime, 0);
        task_ptr t(new detail::add_timer_task(task, this));
        run_task(std::move(t));
        return timer_id(task, task->sequence());
    }
}

timer_id event_loop::run_every(ksl::timespan span, const timer_callback &cb)
{
    ksl::timestamp runTime = ksl::timestamp::now() + span;
    if(_thread_binder.is_in_thread()) {
        LOG_TRACE("loop span {}", span.total_microseconds());
        timer_id rid = _timer.add_timer(cb, runTime, span);
        ksl::timestamp min = _timer.earliest();
        _timer_notify->reset(min);
        return rid;
    } else {
        timer_task_ptr task= std::make_shared<timer_task>(cb, runTime, span);
        task_ptr t(new detail::add_timer_task(task, this));
        run_task(std::move(t));
        return timer_id(task, task->sequence());
    }
}

void event_loop::add_timer_in_loop(timer_task_ptr &ptr)
{
    _timer.add_timer(ptr);
    ksl::timestamp min = _timer.earliest();
    _timer_notify->reset(min);
}
void event_loop::loop()
{
    _thread_binder.abort_not_in_thread();
    _status.set_to(loop_status::running);

    while (!_quit) {
        _active_channels.clear();
        event_option_ptr curOption = _option;
        _last_poll = _poller->poll(curOption->poll_time_ms, &_active_channels);
        ++_iteration;
        LOG_TRACE("id: {} iteration: {} ", _thread_binder.id(), _iteration);
        _do_looping = true;

        for (channel_list::iterator it = _active_channels.begin();
            it != _active_channels.end(); ++it) {
            _current_active_channel = *it;
            _current_active_channel->handle_event(_last_poll);
        }
        _current_active_channel = nullptr;
        _do_looping = false;;
        if(curOption->pri_local_task) {
            do_queued_task();
            do_shared_task();
        } else {
            do_shared_task();
            do_queued_task();
        }
    }
    _status.set_to(loop_status::stopping);
    clear_loop();
    _status.set_to(loop_status::stoped);
    LOG_TRACE("event_loop::loop end");
}

void event_loop::clear_loop()
{
    LOG_TRACE("clear_loop:");
    _notify->stop();
    _timer_notify->stop();
    _poller->stop();
    _notify.reset();
    _timer_notify.reset();
    _poller.reset();
    LOG_TRACE("clear_loop:");
}

} //namespace event
} //namespace kob