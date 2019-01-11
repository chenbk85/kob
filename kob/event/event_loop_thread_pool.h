//
// Created by 李寅斌 on 2018/12/2.
//

#ifndef KOB_EVENT_EVENT_LOOP_THREAD_POOL_H_
#define KOB_EVENT_EVENT_LOOP_THREAD_POOL_H_
#include <kob/event/event_loop_thread.h>
#include <kob/event/event_loop.h>
#include <vector>
namespace kob {
namespace event {

class event_loop_thread_pool {
public:
    event_loop_thread_pool();

    ~event_loop_thread_pool();

    void set_thread_num(uint32_t n)
    {
        _thread_num = n;
    }

    void set_name(const std::string &name)
    {
        _name = name;
    }

    bool start();

    void stop();

    event_loop_ptr get_rr()
    {
        if(_thread_num > 1) {
            event_loop_ptr ptr =  _loops[_rr % _thread_num];
            _rr++;
            return ptr;
        } else if(_thread_num == 1) {
            return _loops[0];
        }

        return event_loop_ptr();
    }

    event_loop_ptr get_hash(uint64_t hcode)
    {
        if(_thread_num > 1) {
            return _loops[hcode % _thread_num];
        } else if(_thread_num == 1) {
            return _loops[0];
        }
        return event_loop_ptr();
    }

    std::vector<event_loop_ptr> get_all()
    {
        return _loops;
    }
private:
    uint32_t                            _thread_num;
    uint32_t                            _rr;
    std::vector<event_loop_thread_ptr>  _threads;
    std::vector<event_loop_ptr>         _loops;
    std::string                         _name;

};
}
}
#endif //KOB_EVENT_EVENT_LOOP_THREAD_POOL_H_
