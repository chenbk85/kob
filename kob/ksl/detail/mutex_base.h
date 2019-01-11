//
// Created by 李寅斌 on 2018/11/8.
//

#ifndef KOB_KSL_MUTEX_BASE_H_
#define KOB_KSL_MUTEX_BASE_H_

#include <kob/ksl/detail/config.h>
#include <kob/ksl/detail/thread_base.h>
#include <kob/ksl/timespan.h>
#include <kob/ksl/timestamp.h>
#include <algorithm>
#include <pthread.h>

namespace ksl {

class mutex {
public:
    typedef thread_base::mutex_t   mutex_type;
    typedef thread_base::mutex_t*  pointer_type;
public:
    mutex():_mutex(mutex_init_value){}
    ~mutex(){}

    void lock();

    bool trylock();

    void unlock();

    pointer_type get_pointer()
    {
        return &_mutex;
    }
private:
    mutex(const mutex&);
    mutex& operator=(const mutex&);
private:
    mutex_type   _mutex;
};


struct defer_lock_t {};
struct try_to_lock_t {};
struct adopt_lock_t {};

extern const defer_lock_t  defer_lock;
extern const try_to_lock_t try_to_lock;
extern const adopt_lock_t  adopt_lock;

template <typename MUTEX>
class lock_guard {
public:
    typedef MUTEX mutex_type;
public:
    explicit lock_guard(mutex_type& m):_mutex(m)
    {
        _mutex.lock();
    }

    lock_guard(mutex_type& m, adopt_lock_t):_mutex(m)
    {

    }

    ~lock_guard()
    {
        _mutex.unlock();
    }
private:
    mutex_type  &_mutex;
private:
    lock_guard(const lock_guard&);
    lock_guard& operator=(const lock_guard&);
};

template <typename MUTEX>
class unique_guard {
public:
    typedef MUTEX    mutex_type;
    typedef MUTEX*   pointer_type;
public:
    unique_guard() : _mutex(nullptr), _owned(false){}

    explicit unique_guard(mutex_type &m) : _mutex(&m), _owned(true)
    {
        _mutex->lock();
    }

    unique_guard(mutex_type &m, defer_lock_t) :_mutex(&m), _owned(false) {}

    unique_guard(mutex_type &m, try_to_lock_t) :_mutex(&m), _owned(_mutex->trylock()){}

    unique_guard(mutex_type &m, adopt_lock_t) :_mutex(&m), _owned(true) {}

    unique_guard(mutex_type &m, const timestamp &ts)
    :_mutex(&m),
    _owned(_mutex->try_lock_until(ts))
    {

    }

    unique_guard(mutex_type &m, const timespan &span)
    :_mutex(&m),
     _owned(_mutex->try_lock_for(span))
    {

    }

    ~unique_guard()
    {
        if(_owned) {
            KOB_ASSERT_MSG(_mutex != nullptr, "mutex object null");
            _mutex->unlock();
        }
    }

    unique_guard(unique_guard &&rhs)
    :_mutex(rhs._mutex),
     _owned(rhs._owned)
    {
        KOB_ASSERT_MSG(this != &rhs, "move self");
        rhs._mutex = nullptr;
        rhs._owned = false;
    }

    unique_guard& operator=(const unique_guard&& rhs)
    {
        KOB_ASSERT_MSG(this != &rhs, "move self");
        if (_owned) {
            _mutex->unlock();
        }
        _owned = rhs._owned;
        _mutex = rhs._mutex;
        rhs._mutex = nullptr;
        rhs._owned = false;
        return *this;
    }
public:
    void lock();

    bool try_lock();

    bool try_lock_for(const timespan &span);

    bool try_lock_until(const timestamp & stamp);

    void unlock();

    void swap(unique_guard &rhs)
    {
        std::swap(_mutex, rhs._mutex);
        std::swap(_owned, rhs._owned);
    }

    mutex_type *release()
    {
        mutex_type *p = _mutex;
        _mutex = nullptr;
        _owned = false;
        return p;
    }

    bool is_owned()
    {
        return _owned;
    }

    operator bool()
    {
        return  _owned;
    }

    mutex_type* mutex()
    {
        return _mutex;
    }
private:
    unique_guard(const unique_guard &);
    unique_guard& operator=(const unique_guard&);
private:
    mutex_type *_mutex;
    bool        _owned;
};

template <typename MUTEX>
KOB_FORCE_INLINE void unique_guard<MUTEX>::lock()
{
    KOB_ASSERT_MSG(_mutex != nullptr, "mutex is null");
    KOB_ASSERT_MSG(_owned == false, "alread locked");
    _mutex->lock();
    _owned = true;
}

template <typename MUTEX>
KOB_FORCE_INLINE bool unique_guard<MUTEX>::try_lock()
{
    KOB_ASSERT_MSG(_mutex != nullptr, "mutex is null");
    KOB_ASSERT_MSG(_owned == false, "alread locked");
    _owned = _mutex->trylock();
    return _owned;
}

template <typename MUTEX>
KOB_FORCE_INLINE bool unique_guard<MUTEX>::try_lock_for(const timespan &span)
{
    KOB_ASSERT_MSG(_mutex != nullptr, "mutex is null");
    KOB_ASSERT_MSG(_owned == false, "alread locked");
    _owned = _mutex->try_lock_for(span);
    return _owned;
}

template <typename MUTEX>
KOB_FORCE_INLINE bool unique_guard<MUTEX>::try_lock_until(const timestamp & stamp)
{
    KOB_ASSERT_MSG(_mutex != nullptr, "mutex is null");
    KOB_ASSERT_MSG(_owned == false, "alread locked");
    _owned = _mutex->try_lock_until(stamp);
    return _owned;
}

template <typename MUTEX>
KOB_FORCE_INLINE void unique_guard<MUTEX>::unlock()
{
    KOB_ASSERT_MSG(_mutex != nullptr, "mutex is null");
    KOB_ASSERT_MSG(_owned == true, "alread locked");
    _mutex->unlock();
    _owned = false;
}

enum class cv_status {
    ok,
    timeout
};
class cond_var {
public:
    typedef thread_base::condvar_t  cond_type;
    typedef thread_base::condvar_t* pointer_type;

public:
    cond_var() : _cv(condvar_init_value){}
    ~cond_var();
public:
    void notify_one() KOB_NOEXCEPT;
    void notify_all() KOB_NOEXCEPT;

    void wait(unique_guard<mutex> &ug) KOB_NOEXCEPT;

    template<typename PreFilter>
    void wait(unique_guard<mutex> &lk, PreFilter f);

    cv_status wait_until(unique_guard<mutex> &lk, const timestamp &stamp);

    template<typename PreFilter>
    bool wait_until(unique_guard<mutex> &lk, const timestamp &stamp, PreFilter f);

    cv_status wait_for(unique_guard<mutex> &lk, const timespan &span);

    template<typename PreFilter>
    bool wait_for(unique_guard<mutex> &lk, const timespan &span, PreFilter f);

    pointer_type get_pointer()
    {
        return &_cv;
    }
private:
    cond_var(const cond_var&);
    cond_var& operator=(const cond_var&);

    bool do_timed_wait(unique_guard<mutex> &lk, const timestamp &span);
private:
    cond_type _cv;
};

template<typename PreFilter>
KOB_FORCE_INLINE void cond_var::wait(unique_guard<mutex> &lk, PreFilter f)
{
    while (!f()) {
        wait(lk);
    }
}

KOB_FORCE_INLINE cv_status cond_var::wait_until(unique_guard<mutex> &lk, const timestamp &stamp)
{
    bool ec = do_timed_wait(lk, stamp);
    if(!ec) {
        return  cv_status::timeout;
    }
    return timestamp::now() < stamp ? cv_status::ok : cv_status::timeout;
}

template<typename PreFilter>
KOB_FORCE_INLINE bool cond_var::wait_until(unique_guard<mutex> &lk, const timestamp &stamp, PreFilter f)
{
    while (!f()) {
        if (wait_until(lk, stamp) == cv_status::timeout) {
            return f();
        }
    }
    return true;
}

KOB_FORCE_INLINE cv_status cond_var::wait_for(unique_guard<mutex> &lk, const timespan &span)
{
    if (span <= 0) {
        return cv_status::timeout;
    }

    return wait_until(lk, timestamp::now() + span);
}

template<typename PreFilter>
KOB_FORCE_INLINE bool cond_var::wait_for(unique_guard<mutex> &lk, const timespan &span, PreFilter f)
{
    return wait_until(lk, timestamp::now() + span, std::forward(f));
}


} //namespace ksl
#endif //KOB_KSL_MUTEX_BASE_H_
