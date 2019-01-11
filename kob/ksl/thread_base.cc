//
// Created by 李寅斌 on 2018/11/8.
//
#include <kob/ksl/detail/thread_base.h>
#include <kob/ksl/detail/config.h>

namespace ksl {
namespace thread_base {

int recursive_mutex_init(recursive_mutex_t *m)
{
    pthread_mutexattr_t attr;
    int ec = pthread_mutexattr_init(&attr);
    if (ec)
        return ec;
    ec = pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
    if (ec) {
        pthread_mutexattr_destroy(&attr);
        return ec;
    }
    ec = pthread_mutex_init(m, &attr);
    if (ec) {
        pthread_mutexattr_destroy(&attr);
        return ec;
    }
    ec = pthread_mutexattr_destroy(&attr);
    if (ec) {
        pthread_mutex_destroy(m);
        return ec;
    }
    return 0;
}

int recursive_mutex_lock(recursive_mutex_t *m)
{
    KOB_ASSERT_MSG(m != NULL, "mutex is null");
    return pthread_mutex_lock(m);
}

bool recursive_mutex_trylock(recursive_mutex_t *m)
{
    KOB_ASSERT_MSG(m != NULL, "mutex is null");
    return pthread_mutex_trylock(m);
}

int recursive_mutex_unlock(mutex_t *m)
{
    KOB_ASSERT_MSG(m != NULL, "mutex is null");
    return pthread_mutex_unlock(m);
}

int recursive_mutex_destroy(recursive_mutex_t *m)
{
    return pthread_mutex_destroy(m);
}


int mutex_lock(mutex_t *m)
{
    return pthread_mutex_lock(m);
}

bool mutex_trylock(mutex_t *m)
{
    return pthread_mutex_trylock(m);
}

int mutex_unlock(mutex_t *m)
{
    return pthread_mutex_unlock(m);
}

int mutex_destroy(mutex_t *m)
{
    return pthread_mutex_destroy(m);
}

int condvar_signal(condvar_t *cv)
{
    return pthread_cond_signal(cv);
}

int condvar_broadcast(condvar_t *cv)
{
    return pthread_cond_broadcast(cv);
}

int condvar_wait(condvar_t *cv, mutex_t *m)
{
    return pthread_cond_wait(cv, m);
}

int condvar_timedwait(condvar_t *cv, mutex_t *m, timespec *ts)
{
    return pthread_cond_timedwait(cv, m, ts);
}

int condvar_destroy(condvar_t *cv)
{
    return pthread_cond_destroy(cv);
}

int execute_once(once_t *one, once_handler oh)
{
    return pthread_once(one, oh);
}

bool thread_id_equal(thread_id t1, thread_id t2)
{
    return pthread_equal(t1, t2) != 0;
}

bool thread_id_less(thread_id t1, thread_id t2)
{
    return t1 < t2;
}

bool thread_is_null(thread_id *t)
{
    return *t == 0;
}

int thread_create(thread_t *t, void* (*tfun)(void *), void *arg)
{
    return pthread_create(t, 0, tfun, arg);
}


thread_id current_id()
{
    return pthread_self();
}

thread_id get_thread_id(thread_id *t)
{
    return *t;
}

int thread_join(thread_t *t)
{
    return pthread_join(*t, 0);
}

int thread_detach(thread_t *t)
{
    return pthread_detach(*t);
}

void thread_yield()
{
    sched_yield();
}

int tls_create(tls_key *k, void(*at_exit_func)(void*))
{
    return pthread_key_create(k, at_exit_func);
}

void* tls_get(tls_key k)
{
    return pthread_getspecific(k);
}

int tls_set(tls_key k, void *p)
{
    return pthread_setspecific(k, p);
}

}
}
