//
// Created by 李寅斌 on 2018/11/9.
//

#ifndef KOB_KSL_MUTEX_H_
#define KOB_KSL_MUTEX_H_
#include <kob/ksl/detail/mutex_base.h>
#include <kob/ksl/detail/thread_base.h>
#include <kob/ksl/detail/config.h>
#include <cstdint>

namespace ksl {
class recursive_mutex {
public:
    typedef thread_base::recursive_mutex_t  mutex_type;
    typedef thread_base::recursive_mutex_t* pointer_type;
public:
    recursive_mutex();
    ~recursive_mutex();

public:
    void lock();
    void unlock();
    void try_lock();

    pointer_type get_pointer() { return &_mutex; }
private:
    mutex_type  _mutex;
};

class timed_mutex{
public:
    timed_mutex();
    ~timed_mutex();

public:

    void lock();

    bool try_lock();

    bool try_lock_for(const timespan &span)
    {
        return try_lock_until(timestamp::now() + span);
    }

    bool try_lock_until(const timestamp &stamp);
    void unlock();

private:
    mutex     _mutex;
    cond_var  _cond;
    bool      _locked;
};

class null_mutex{
public:
    null_mutex() {}
    ~null_mutex(){}

    void lock() {}

    bool try_lock() { return  true; }

    void unlock() { }
};
struct once_flag;

template <typename Call, typename...args>
void call_once(once_flag&, Call&&, args&&...);

struct once_flag {
public:
    once_flag() : _state(0) {}

private:
    once_flag(const once_flag&);
    once_flag& operator=(const once_flag&);

    template <typename Call, typename...args>
    friend void call_once(once_flag&, Call&&, args&&...);
private:
    uint64_t  _state;
};
}
#endif //KOB_KSL_MUTEX_H_
