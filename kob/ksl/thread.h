#ifndef KOB_KSL_THREAD_H_
#define KOB_KSL_THREAD_H_
#include <kob/ksl/shared_state.h>
#include <kob/ksl/mutex.h>
#include <kob/ksl/detail/thread_base.h>
#include <kob/ksl/string_view.h>
#include <memory>
#include <functional>
#include <limits.h>
#include <assert.h>
#include <pthread.h>
#include <string>
#include "this_thread.h"

namespace ksl {

/*!
* This class implements a platform-independent
* wrapper to an operating system thread.
*
* Every Thread object gets a unique (within
* its process) numeric thread ID.
* Furthermore, a thread can be assigned a name.
* The name of a thread can be changed at any time.
*/

using thread_base::thread_t;
using thread_base::thread_id;


enum class thread_priority {
    tp_lowest  ,     //!< The lowest thread priority.
    tp_low     ,    //!< A lower than normal thread priority.
    tp_normal  ,    //!< The normal thread priority.
    tp_high    ,    //!< A higher than normal thread priority.
    tp_highest     //!< The highest thread priority.
};

enum class thread_status {
    ts_non,
    ts_check,  //pre check start resource
    ts_pre_run,    // run pre_run for env prepare
    ts_run,     // run runner do working
    ts_post,    // run post_run
    ts_clean,   // run clean, clean sources
    ts_stop
};

struct thread_content;
typedef std::shared_ptr<thread_content> thread_context_ptr;

struct thread_content {
    typedef std::function<void()> handler_type;
    typedef std::function<bool(const thread_context_ptr&)> check_type;
    typedef thread_base::thread_t                    thread_t;
	shared_state<thread_status>         status;
    check_type                          pre_check;
	check_type                          pre_run;
    handler_type                        runner;
    handler_type                        post_run;
    handler_type                        clean;
    std::string                         name;
    std::size_t                         stack_size;
    bool                                started;
    bool                                joined;
    bool                                owned;
    bool                                pre_check_ok;
    ksl::mutex                          mut;
	pthread_t                           tid;
	int32_t                             id;
	pthread_t                           thread;

public:
	thread_content()
	: status(thread_status::ts_check),
	  pre_check(check_type()),
	  pre_run(check_type()),
	  runner(handler_type()),
	  post_run(handler_type()),
	  clean(handler_type()),
	  name(""),
	  stack_size(0),
	  started(false),
	  joined(false),
	  owned(false),
	  pre_check_ok(true),
	  mut(),
	  tid(ksl::this_thread::kInvalidThreadTid),
	  id(ksl::this_thread::kInvalidThreadId),
	  thread()
	{}
	~thread_content() = default;

};




class thread {
public:
    typedef thread_content::thread_t      thread_t;
    typedef thread_content::handler_type  handler_type;
    typedef thread                        this_type;
    typedef thread*                       pointer;

public:

	thread();

	explicit thread(const thread_context_ptr &);

	~thread();

	int id() const;

    thread_t tid() const;


    void set_name(ksl::string_view sv, bool absolute = false, char split = '#');

    ksl::string_view get_name();

	thread_context_ptr get_context()
	{
		return _tcp;
	}

	void set_stack_size(size_t size);

    size_t get_stack_size() const;

#ifdef KOB_PLATFORM_LINUX
	bool set_affinity(int cpu);


	int get_affinity() const;
#endif

	bool start();
		

	bool start(const handler_type &fn);

	void join();


	bool is_running() const;

	static thread* current();

protected:
	//thread function

	static void* thread_function(void *data);

	void make_name();

private:
    thread(const thread&);
    thread& operator = (const thread&);

private:
    thread_context_ptr          _tcp;
};

} 


#endif //KOB_KSL_THREAD_H_

