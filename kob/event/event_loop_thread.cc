//
// Created by 李寅斌 on 2018/12/2.
//

#include <kob/event/event_loop_thread.h>

namespace kob {
namespace event {
event_loop_thread::event_loop_thread()
: _loop(),
  _thread(),
  _loop_status(0)
{

}

event_loop_thread::~event_loop_thread()
{
    LOG_TRACE("event_loop_thread dtor");
}

event_loop_ptr event_loop_thread::start()
{
    bool r = _thread.start(std::bind(&event_loop_thread::thread_func, this));
    if(!r) {
        LOG_ERROR("event_loop_thread start error");
    }
    _loop_status.wait(1);
    return _loop;
}

void event_loop_thread::thread_func()
{
    LOG_TRACE("thread_func");
    _loop.reset(new event_loop());
    if(!_loop->init()) {
        LOG_ERROR("event loop init error");
        _loop.reset();
        return;
    }
    _loop_status.set_to(1);
    LOG_TRACE("event_loop thread going to loop");
    _loop->loop();
    LOG_TRACE("event_loop thread stoped");
}

void event_loop_thread::stop()
{
    LOG_TRACE(" event_loop_thread::stop");
    _loop->quit();
    _loop->join();
    _thread.join();
    LOG_TRACE(" event_loop_thread::stop end");
}

}
}