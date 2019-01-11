//
// Created by 李寅斌 on 2018/12/2.
//

#ifndef KOB_EVENT_EVENT_LOOP_THREAD_H_
#define KOB_EVENT_EVENT_LOOP_THREAD_H_
#include <kob/event/event_loop.h>
#include <kob/ksl/shared_state.h>
#include <kob/ksl/thread.h>
#include <memory>

namespace kob {
namespace event {

class event_loop_thread {
public:
    event_loop_thread();
    ~event_loop_thread();

    event_loop_ptr start();

    void stop();

    ksl::thread* thread()
    {
        return &_thread;
    }

    event_loop_ptr get_loop() const
    {
        return  _loop;
    }

private:
    void thread_func();
private:
    event_loop_ptr          _loop;
    ksl::thread             _thread;
    ksl::shared_state<int>  _loop_status;
};

typedef std::shared_ptr<event_loop_thread> event_loop_thread_ptr;
} //namespace event
} //namespace kob
#endif //KOB_EVENT_EVENT_LOOP_THREAD_H_
