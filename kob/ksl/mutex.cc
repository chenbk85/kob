//
// Created by 李寅斌 on 2018/11/8.
//

#include <kob/ksl/detail/mutex_base.h>
#include <kob/ksl/mutex.h>

namespace ksl {

void mutex::lock()
{
    int ec = ksl::thread_base::mutex_lock(&_mutex);
    KOB_UNUSED(ec);
}

bool mutex::trylock()
{
    return ksl::thread_base::mutex_trylock(&_mutex);
}

void mutex::unlock()
{
    int ec = ksl::thread_base::mutex_unlock(&_mutex);
    KOB_UNUSED(ec);
}

const defer_lock_t  defer_lock;
const try_to_lock_t try_to_lock;
const adopt_lock_t  adopt_lock;

recursive_mutex::recursive_mutex()
{
    int ec = thread_base::recursive_mutex_init(&_mutex);
    KOB_UNUSED(ec);
}

recursive_mutex::~recursive_mutex()
{
    int ec = thread_base::recursive_mutex_destroy(&_mutex);
    KOB_UNUSED(ec);
}

void recursive_mutex::lock()
{
    int ec = thread_base::recursive_mutex_lock(&_mutex);
    KOB_UNUSED(ec);
}

void recursive_mutex::unlock()
{
    int ec = thread_base::recursive_mutex_unlock(&_mutex);
    KOB_UNUSED(ec);
}

void recursive_mutex::try_lock()
{
    int ec = thread_base::recursive_mutex_trylock(&_mutex);
    KOB_UNUSED(ec);
}

timed_mutex::timed_mutex():_locked(false)
{

}

timed_mutex::~timed_mutex()
{
    lock_guard<mutex> lg(_mutex);
}

bool timed_mutex::try_lock()
{
    unique_guard<mutex> lk(_mutex, try_to_lock);
    if (lk.is_owned() && !_locked) {
        _locked = true;
        return true;
    }
    return false;
}

void timed_mutex::lock()
{
    unique_guard<mutex> lk(_mutex);
    while (!_locked) {
        _cond.wait(lk);
    }
    _locked = true;
}

void timed_mutex::unlock()
{
    lock_guard<mutex> lg(_mutex);
    _locked = false;
    _cond.notify_one();
}

bool timed_mutex::try_lock_until(const timestamp &stamp)
{
    unique_guard<mutex> lk(_mutex);
    bool isOut = stamp > timestamp::now();
    while (isOut && _locked) {
        isOut = _cond.wait_until(lk, stamp) == cv_status::timeout;
    }

    if(!_locked) {
        _locked = true;
        return  true;
    }
    return false;

}

}