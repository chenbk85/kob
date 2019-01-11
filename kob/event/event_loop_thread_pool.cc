//
// Created by 李寅斌 on 2018/12/2.
//

#include <kob/event/event_loop_thread_pool.h>

namespace kob {
namespace event {

event_loop_thread_pool::event_loop_thread_pool()
:_thread_num(0),
 _rr(0),
 _threads(),
 _loops(),
 _name()
{

}

event_loop_thread_pool::~event_loop_thread_pool()
{

}

bool event_loop_thread_pool::start()
{
    LOG_TRACE("thread_pool start");
    for (size_t i = 0; i < _thread_num; ++i) {
        event_loop_thread_ptr t(new event_loop_thread());
        _threads.push_back(t);
        if(!_name.empty()) {
            t->thread()->set_name(_name);
        }
        _loops.push_back(t->start());
    }
    LOG_TRACE("thread_pool end");
    return true;
}

void event_loop_thread_pool::stop()
{
    LOG_TRACE("thread_pool stop");
    for (size_t i = 0; i < _thread_num; ++i) {
        _threads[i]->stop();
    }
    _threads.clear();
    _loops.clear();
    LOG_TRACE("thread_pool end");
}

}
}