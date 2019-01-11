//
// Created by 李寅斌 on 2018/11/8.
//

#ifndef KOB_KSL_THREAD_BASE_H_
#define KOB_KSL_THREAD_BASE_H_
#include <pthread.h>

namespace ksl {
namespace thread_base {

    #define mutex_init_value PTHREAD_MUTEX_INITIALIZER
    typedef pthread_mutex_t mutex_t;
    typedef pthread_mutex_t recursive_mutex_t;

    #define condvar_init_value PTHREAD_COND_INITIALIZER
    typedef pthread_cond_t  condvar_t;

    #define once_init_value PTHREAD_ONCE_INIT
    typedef pthread_once_t  once_t;

    typedef pthread_t       thread_id;
    typedef pthread_t       thread_t;

    #define null_thread_value 0U

    typedef pthread_key_t      tls_key;
    typedef void (*once_handler)(void);

    int recursive_mutex_init(recursive_mutex_t *m);

    int recursive_mutex_lock(recursive_mutex_t *m);

    bool recursive_mutex_trylock(recursive_mutex_t *m);

    int recursive_mutex_unlock(mutex_t *m);

    int recursive_mutex_destroy(recursive_mutex_t *m);

    int mutex_lock(mutex_t *m);

    bool mutex_trylock(mutex_t *m);

    int mutex_unlock(mutex_t *m);

    int mutex_destroy(mutex_t *m);

    int condvar_signal(condvar_t *cv);

    int condvar_broadcast(condvar_t *cv);

    int condvar_wait(condvar_t *cv, mutex_t *m);

    int condvar_timedwait(condvar_t *cv, mutex_t *m, timespec *ts);

    int condvar_destroy(condvar_t *cv);

    int execute_once(once_t *one, once_handler oh);

    bool thread_id_equal(thread_id t1, thread_id t2);

    bool thread_id_less(thread_id t1, thread_id t2);

    bool thread_is_null(thread_id *t);

    int thread_create(thread_t *t, void* (*tfun)(void *), void *arg);

    thread_id current_id();

    thread_id get_thread_id(thread_id *t);

    int thread_join(thread_t *t);

    int thread_detach(thread_t *t);

    void thread_yield();

    int tls_create(tls_key *k, void(*at_exit_func)(void*));

    void* tls_get(tls_key k);

    int tls_set(tls_key k, void *p);
}
}
#endif //KOB_KSL_THREAD_BASE_H_
