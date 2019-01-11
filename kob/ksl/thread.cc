#include <kob/ksl/thread.h>
#include <kob/ksl/this_thread.h>
#include <kob/log/logging.h>
#include <kob/ksl/env.h>
#include <atomic>
#include <time.h>
#include <unistd.h>
#include <cstring>
#include <pthread.h>

namespace ksl {

namespace detail {
	
	__thread ksl::thread *current_thread_pointer = NULL;
}



thread::thread(): _tcp(new thread_content())
{
}


thread::thread(const thread_context_ptr &tc):
	_tcp(tc)
{
}


thread::~thread()
{
	LOG_TRACE("ksl::thread dtor");
	if(_tcp->started && !_tcp->joined ) {
		::pthread_detach(_tcp->thread);
	}
	LOG_TRACE("ksl::thread dtor ok");
}

void thread::make_name()
{
    char buf[32];
    std::string tmp(_tcp->name);
    if (tmp.empty()) {
        tmp = "kob";
    }
    _tcp->name.clear();
    ksl::string_view sv(tmp);
    snprintf(buf, sizeof(buf) - 1, "%u",_tcp->id);
    ksl::string_view nsv(buf);
    if(sv.size() + nsv.size() > ksl::this_thread::kMaxThreadNameLen) {
        size_t n = ksl::this_thread::kMaxThreadNameLen - nsv.size() - 1;
        KOB_ASSERT_MSG(n < ksl::this_thread::kMaxThreadNameLen, "over flow");
        sv.remove_suffix(nsv.size() + 1);
    }
    _tcp->name.append(sv.begin(),sv.end());
    _tcp->name.append(1, '#');
    _tcp->name.append(nsv.begin(), nsv.end());
}
void thread::set_name(ksl::string_view sv, bool absolute, char spliter)
{
	if(!_tcp->started) {
		_tcp->name.clear();
		_tcp->name.append(sv.begin(), sv.end());
		return;
	}
	_tcp->name.clear();
	if(!absolute) {
		char buf[32];
		snprintf(buf, sizeof(buf) - 1, "%u",_tcp->id);
		ksl::string_view nsv(buf);
		if(sv.size() + nsv.size() > ksl::this_thread::kMaxThreadNameLen) {
			size_t n = ksl::this_thread::kMaxThreadNameLen - nsv.size() - 1;
			KOB_ASSERT_MSG(n < ksl::this_thread::kMaxThreadNameLen, "over flow");
			sv.remove_suffix(nsv.size() + 1);
		}
		_tcp->name.append(sv.begin(),sv.end());
		_tcp->name.append(1, spliter);
		_tcp->name.append(nsv.begin(), nsv.end());
	} else {
		_tcp->name.append(sv.begin(), sv.end());
	}
	if (_tcp->started && !_tcp->joined) {
		ksl::this_thread::set_thread_name(_tcp->thread, _tcp->name);
	}
}

ksl::string_view thread::get_name()
{
	return _tcp->name;
}

bool thread::start()
{
	KOB_ASSERT_MSG(_tcp, "null thread_context_ptr");
	KOB_ASSERT_MSG(_tcp->runner, "can not runner");
	KOB_ASSERT_MSG(!_tcp->joined, "have already jioned");
	KOB_ASSERT_MSG(!_tcp->started, "have already started");

	if(_tcp->owned) {
		return  false;
	}
	{
		ksl::unique_guard<ksl::mutex> lk(_tcp->mut);
		if (_tcp->owned) {
			return false;
		}
		_tcp->owned = true;
	}

	_tcp->status.set_to(thread_status::ts_check);

	if(_tcp->pre_check && !_tcp->pre_check(_tcp)) {
		return false;
	}

	::pthread_attr_t      attributes;
	::pthread_attr_init(&attributes);
	if (_tcp->stack_size != 0) {
		if (0 != ::pthread_attr_setstacksize(&attributes, _tcp->stack_size)) {
			pthread_attr_destroy(&attributes);
			return false;
		}
	}

	if (pthread_create(&_tcp->thread, &attributes, thread_function, this)) {
		pthread_attr_destroy(&attributes);
		return false;
	}
	_tcp->status.wait(thread_status::ts_pre_run);

	if(!_tcp->pre_check_ok) {
		return false;
	}

	_tcp->started = true;
	::pthread_attr_destroy(&attributes);
	_tcp->status.wait(thread_status::ts_run);
	return true;

}

bool thread::start(const handler_type &fn)
{
	_tcp->runner = fn;
	return start();
}

thread* thread::current()
{
 	return detail::current_thread_pointer;
}
void thread::join()
{
	if(!_tcp){
		return ;
	}

	if(_tcp->started && !_tcp->joined) {
        _tcp->status.wait(thread_status::ts_stop);
        void *result = NULL;
        pthread_join(_tcp->thread, &result);
        _tcp->joined = true;
    }
}

bool thread::is_running() const
{
	if(_tcp->status.get() == thread_status::ts_stop){
		return true;
	} 
	return false;
}

void thread::set_stack_size(size_t size)
{
	_tcp->stack_size = size;
}

size_t thread::get_stack_size() const
{
	return _tcp->stack_size;
}

#ifdef KOB_PLATFORM_LINUX
bool thread::set_affinity(int cpu)
{
	cpu_set_t cpuset;
	CPU_ZERO(&cpuset);
	CPU_SET(cpu, &cpuset);
	if (::pthread_setaffinity_np(_tcp->thread, sizeof(cpuset), &cpuset) != 0) {
		return false;
	}
	ksl::this_thread::yield();
	return true;
}

int thread::get_affinity() const
{
	int cpuSet = -1;
	cpu_set_t cpuset;
	CPU_ZERO(&cpuset);
	if (pthread_getaffinity_np(_tcp->thread, sizeof(cpuset), &cpuset) != 0) {
		return false;
	}
	int cpuCount = env::processor_count();
	for (int i = 0; i < cpuCount; i++) {
		if (CPU_ISSET(i, &cpuset)) {
			cpuSet = i;
			break;
		}
	}
	return cpuSet;
}
#endif
int thread::id() const
{
	return _tcp->id;
}

thread::thread_t thread::tid() const
{
	return _tcp->thread;
}


void* thread::thread_function(void *data)
{
	LOG_TRACE("ksl::thread start");
	thread* pthis = static_cast<thread*>(data);

	pthis->_tcp->tid = ksl::this_thread::tid();
	pthis->_tcp->id = ksl::this_thread::id();
	pthis->_tcp->status.set_to(thread_status::ts_pre_run);
	if(pthis->_tcp->pre_run) {
		pthis->_tcp->pre_check_ok = pthis->_tcp->pre_run(pthis->_tcp);
	}
	if(!pthis->_tcp->pre_check_ok) {
		return NULL;
	}

    pthis->make_name();
	if(pthis->_tcp->name.size() > 0) {
		ksl::this_thread::set_thread_name(pthis->_tcp->thread, pthis->_tcp->name);
	}

	detail::current_thread_pointer = pthis;
	pthis->_tcp->tid = ksl::this_thread::tid();

	pthis->_tcp->status.set_to(thread_status::ts_run);
	pthis->_tcp->runner();

	pthis->_tcp->status.set_to(thread_status::ts_post);

	if(pthis->_tcp->post_run) {
		pthis->_tcp->post_run();
	}

	pthis->_tcp->status.set_to(thread_status::ts_clean);

	if(pthis->_tcp->clean) {
		pthis->_tcp->clean();
	}

	pthis->_tcp->status.set_to(thread_status::ts_stop);
	LOG_TRACE("ksl::thread end");
	return NULL;
}


} 
