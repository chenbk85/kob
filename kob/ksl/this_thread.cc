#include <kob/ksl/this_thread.h>
#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/syscall.h>
#include <atomic>
#include <iostream>

namespace ksl {
namespace this_thread{
	  __thread thread_id   t_tid = kInvalidThreadTid;
	  __thread int32_t     t_id = kInvalidThreadId;

	static std::atomic<int32_t>   g_t_id(0);

	void cache_id()
	{
		t_id =  g_t_id.fetch_add(1);
	}

	int32_t thread_number()
	{
		return g_t_id.load();
	}

	void cache_tid()
	{
	  if (t_tid == 0) {
	    	t_tid = thread_base::current_id();
	  }
	}

	void exit()
	{
		::pthread_exit(NULL);
	}
		
	void yield()
	{
		::sched_yield();
	}

	bool sleep_for(timespan span)
	{

		struct timespec ts = { 0, 0 };
		ts.tv_sec = static_cast<time_t>(span.total_seconds());
		ts.tv_nsec = static_cast<long>(span.useconds() * 1000);
		::nanosleep(&ts, NULL);

	return true;

	}

	bool sleep_until(timestamp stamp)
	{
		timestamp now;
		if(now >= stamp) {
			return false;
		} 
		timespan span = now -stamp;
		return  sleep_for(span);
	}
#ifdef  KOB_PLATFORM_LINUX
	void set_thread_name(thread_base::thread_t thread, ksl::string_view sv)
	{
		std::string truck(sv.data(), sv.size());

		if(truck.size() >  kMaxThreadNameLen) {
			truck.resize(kMaxThreadNameLen);
		}
		pthread_setname_np(thread, truck.c_str());

	}
#endif

#ifdef KOB_PLATFORM_OSX
		void set_thread_name(ksl::string_view sv)
		{
			std::string truck(sv.data(), sv.size());
			if(truck.size() >  kMaxThreadNameLen) {
				truck.resize(kMaxThreadNameLen);
			}
			pthread_setname_np(truck.c_str());
		}
#endif

static const thread_binder main;

bool is_main_thread()
{
	return main.id() == 0;
}

} //namespace this_thread


} //namespace ksl
