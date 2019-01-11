#ifndef KOB_KSL_THISTHREAD_H_
#define KOB_KSL_THISTHREAD_H_
#include <pthread.h>
#include <kob/ksl/timestamp.h>
#include <kob/ksl/timespan.h>
#include <kob/ksl/detail/config.h>
#include <kob/ksl/detail/thread_base.h>
#include <kob/ksl/string_view.h>
#include <kob/profile/prof.h>
#include <string>
#include <unistd.h>
#include <cstdint>

namespace ksl {
namespace this_thread {

    using thread_base::thread_id;
    extern __thread thread_id t_tid;
    extern __thread int32_t   t_id;

    static const thread_id kInvalidThreadTid = 0;
    static const int32_t   kInvalidThreadId  = -1;
    void cache_id();

    void cache_tid();

KOB_FORCE_INLINE int32_t   id()
{
    if (KOB_UNLIKELY(t_id == kInvalidThreadId)) {
        cache_id();
    }
    return t_id;
}

int32_t thread_number();

KOB_FORCE_INLINE thread_id tid()
{
    if (KOB_UNLIKELY(t_tid == kInvalidThreadTid)) {
      cache_tid();
    }
    return t_tid;
}

/*!
* Suspends the current thread for the specified
* amount of time.
*/
bool sleep_for(timespan span);

/*!
* Suspends the current thread for the specified
* amount of time, a another way to sleep for sleep.
*/
bool sleep_until(timestamp stamp);

  /*!
  * Yields cpu to other threads.
  */
void yield();
  /*!
  * exit current thread.
  */
void exit();

    static const size_t kMaxThreadNameLen = 15;
#ifdef  KOB_PLATFORM_LINUX
    void set_thread_name(thread_base::thread_t t, ksl::string_view sv);
#endif

#ifdef KOB_PLATFORM_OSX
    void set_thread_name(ksl::string_view sv);
#endif

class thread_binder {
public:
    thread_binder() KOB_NOEXCEPT
    : _tid(this_thread::tid()),
      _id(this_thread::id())
    {

    }

    ~thread_binder() = default;


    bool is_in_thread() KOB_NOEXCEPT
    {
        return _tid == ksl::this_thread::tid();
    }

    void abort_not_in_thread() KOB_NOEXCEPT
    {
        if (!is_in_thread()) {
            KOB_ASSERT_MSG(false, "not run in own thread");
        }
    }

    int32_t id() const
    {
        return _id;
    }

    thread_id tid() const
    {
        return _tid;
    }

private:
    thread_id              _tid;
    int32_t                _id;
};

bool is_main_thread();


} //namespace this_thread

} //namespace ksl
#endif
