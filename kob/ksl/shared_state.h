//
// Created by 李寅斌 on 2018/11/9.
//

#ifndef KOB_KSL_SHARED_STATE_H_
#define KOB_KSL_SHARED_STATE_H_

#include <kob/ksl/detail/mutex_base.h>

namespace ksl {

template <typename T>
class shared_state {
public:
    typedef T value_type;
public:
    explicit shared_state(const T &v)
    :_v(v)
    {

    }

    void set_to(const T &v)
    {
        unique_guard<mutex> lk(_mu);
        _v = v;
        _cv.notify_one();
    }

    void wait(const T &v)
    {
        unique_guard<mutex> lk(_mu);
        if (_v != v) {
            _cv.wait(lk);
        }
    }

    bool wait_for(const T &v, const timespan &span)
    {
        if(span == 0) {
            return false;
        }

        unique_guard<mutex> lk(_mu);
        while (_v != v) {
           if (_cv.wait_for(lk, span) == cv_status::timeout) {
               return false;
           }
        }
        return true;
    }

    bool wait_until(const T &v, const timestamp &stamp)
    {
        unique_guard<mutex> lk(_mu);
        while (_v != v) {
            if (_cv.wait_until(lk, stamp) == cv_status::timeout) {
                return false;
            }
        }
        return true;
    }

    T get()
    {
        unique_guard<mutex> lk(_mu);
        return _v;
    }
private:
    mutex    _mu;
    cond_var _cv;
    T        _v;

};
}
#endif //KOB_KSL_SHARED_STATE_H_
