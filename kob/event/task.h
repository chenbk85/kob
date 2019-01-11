//
// Created by 李寅斌 on 2018/11/29.
//

#ifndef KOB_EVENT_TASK_H_
#define KOB_EVENT_TASK_H_
#include <kob/ksl/this_thread.h>
#include <functional>
#include <memory>
#include <utility>
#include <vector>

namespace kob {
namespace event {

class task {
public:
    explicit  task(int32_t schedule_id = ksl::this_thread::id())
    :_sid(schedule_id)
    {

    }

    virtual ~task() = default;

    void set_schedule_id(int32_t sid)
    {
        _sid = sid;
    }

    int32_t get_schedule_group() const
    {
        return  _sid;
    }

    virtual void run() = 0;

private:
    int32_t _sid;
};

template <typename Func>
class func_task : public task{
public:
    func_task(int32_t sid, const Func &func)
    :task(sid),
     _func(func)
    {

    }

    func_task(int32_t sid, Func &&func)
            :task(sid),
             _func(std::move(func))
    {

    }

    void run() KOB_OVERRIDE
    {
        _func();
    }


private:
    Func _func;
};

typedef std::shared_ptr<task>              task_ptr;
typedef std::vector<task_ptr>              task_list;

template <typename FUNC>
KOB_FORCE_INLINE task_ptr
make_task(FUNC &&func)
{
    return task_ptr(new func_task<FUNC>(ksl::this_thread::id(), std::forward<FUNC>(func)));
}

template <typename FUNC>
KOB_FORCE_INLINE task_ptr
make_task(int32_t sid, FUNC &&func)
{
    return task_ptr(new func_task<FUNC>(sid, std::forward<FUNC>(func)));
}


} //namespace event
} //namespace kob
#endif //KOB_EVENT_TASK_H_
