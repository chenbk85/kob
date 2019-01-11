//
// Created by 李寅斌 on 2018/11/9.
//

#include <kob/ksl/detail/mutex_base.h>
#include <kob/ksl/detail/thread_base.h>

namespace ksl {

cond_var::~cond_var()
{
    int ec = thread_base::condvar_destroy(&_cv);
    KOB_UNUSED(ec);
}

void cond_var::notify_one() KOB_NOEXCEPT
{
    int ec = thread_base::condvar_signal(&_cv);
    KOB_UNUSED(ec);
}
void cond_var::notify_all() KOB_NOEXCEPT
{
    int ec = thread_base::condvar_broadcast(&_cv);
    KOB_UNUSED(ec);
}

void cond_var::wait(unique_guard<mutex> &lk) KOB_NOEXCEPT
{
    KOB_ASSERT_MSG(lk.is_owned(), "now own lock");
    int ec = thread_base::condvar_wait(&_cv, lk.mutex()->get_pointer());
    KOB_UNUSED(ec);
}

bool cond_var::do_timed_wait(unique_guard<mutex> &lk, const timestamp &stamp)
{
    struct ::timespec ts;
    ts.tv_sec = stamp.raw()/1000000;
    ts.tv_nsec = (stamp.raw()%1000000) * 1000;
    int ec = thread_base::condvar_timedwait(&_cv, lk.mutex()->get_pointer(), &ts);
    if (KOB_LIKELY(ec == 0)) {
        return true;
    }
    return false;
}

}