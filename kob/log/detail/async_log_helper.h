#ifndef LOG_DETAIL_ASYNC_LOG_HELPER_H_
#define LOG_DETAIL_ASYNC_LOG_HELPER_H_
#include <kob/log/common.h>
#include <kob/log/sink/sink.h>
#include <kob/log/detail/mpmc_bounded_q.h>
#include <kob/log/detail/log_msg.h>
#include <kob/log/detail/os.h>
#include <kob/log/formatter.h>
#include <kob/ksl/timestamp.h>
#include <kob/ksl/timespan.h>
#include <kob/ksl/thread.h>
#include <kob/ksl/this_thread.h>
#include <exception>
#include <functional>
#include <memory>
#include <string>
#include <utility>
#include <vector>

namespace kob {
namespace log {

class async_log_helper {
    // Async msg to move to/from the queue
    // Movable only. should never be copied
    enum class async_msg_type
    {
        log,
        flush,
        terminate
    };
    struct async_msg
    {
        std::string     logger_name;
        log_level       level;
        log_clock       time;
        size_t          thread_id;
        std::string     txt;
        async_msg_type  msg_type;

        async_msg(async_msg&& other) :
            logger_name(std::move(other.logger_name)),
            level(std::move(other.level)),
            time(std::move(other.time)),
            txt(std::move(other.txt)),
            msg_type(std::move(other.msg_type))
        {}

        async_msg(async_msg_type m_type) :msg_type(m_type)
        {};

        async_msg& operator=(async_msg&& other) 
        {
            logger_name = std::move(other.logger_name);
            level = other.level;
            time = std::move(other.time);
            thread_id = other.thread_id;
            txt = std::move(other.txt);
            msg_type = other.msg_type;
            return *this;
        }


        // construct from log_msg
        async_msg(const log_msg& m) :
            level(m.level),
            time(m.time),
            thread_id(m.thread_id),
            txt(m.raw.data(), m.raw.size()),
            msg_type(async_msg_type::log)
        {
            logger_name = *m.logger_name;
        }


        async_msg()
        {

        }
        ~async_msg()
        {

        }

        // copy into log_msg
        void fill_log_msg(log_msg &msg)
        {
            msg.logger_name = &logger_name;
            msg.level = level;
            msg.time = time;
            msg.thread_id = thread_id;
            msg.raw << txt;
        }
    private:
        // never copy or assign. should only be moved..
        async_msg(const async_msg&);
        async_msg& operator=(async_msg& other);
    };


public:

    typedef  async_msg                      item_type;
    typedef  mpmc_bounded_queue<item_type>  q_type;
    typedef  ksl::timestamp               clock;


    async_log_helper(formatter_ptr formatter,
                     const std::vector<sink_ptr>& sinks,
                     size_t queue_size,
                     const async_overflow_policy overflow_policy = async_overflow_policy::block_retry,
                     const std::function<void()>& worker_warmup_cb = std::function<void()>(),
                     const ksl::timespan& flush_interval = 0,
                     const std::function<void()>& worker_teardown_cb = std::function<void()>());

    void log(const log_msg& msg);

    // stop logging and join the back thread
    ~async_log_helper();

    void set_formatter(formatter_ptr);

    void flush();


private:
    formatter_ptr                                           _formatter;
    std::vector<std::shared_ptr<sink>>                      _sinks;

    // queue of messages to log
    q_type                                                  _q;

    bool                                                    _flush_requested;

    bool                                                    _terminate_requested;


    // last exception thrown from the worker thread
    std::shared_ptr<log_ex>                                 _last_workerthread_ex;

    // overflow policy
    const async_overflow_policy                             _overflow_policy;

    // worker thread warmup callback - one can set thread priority, affinity, etc
    const std::function<void()>                             _worker_warmup_cb;

    // auto periodic sink flush parameter
    const ksl::timespan                                  _flush_interval;

    // worker thread teardown callback
    const std::function<void()>                             _worker_teardown_cb;

    // worker thread
    ksl::thread                                             _worker_thread;
    bool                                                    _started;

    void push_msg(async_msg&& new_msg);

    // throw last worker thread exception or if worker thread is not active
    void throw_if_bad_worker();

    // worker thread main loop
    void worker_loop();

    // pop next message from the queue and process it. will set the last_pop to the pop time
    // return false if termination of the queue is required
    bool process_next_msg(log_clock& last_pop, log_clock& last_flush);

    void handle_flush_interval(log_clock& now, log_clock& last_flush);

    // sleep,yield or return immediatly using the time passed since last message as a hint
    static void sleep_or_yield(const log_clock& now, const log_clock& last_op_time);
    

};
}
}

///////////////////////////////////////////////////////////////////////////////
// async_sink class implementation
///////////////////////////////////////////////////////////////////////////////
inline kob::log::async_log_helper::async_log_helper(
    formatter_ptr formatter,
    const std::vector<sink_ptr>& sinks,
    size_t queue_size,
    const async_overflow_policy overflow_policy,
    const std::function<void()>& worker_warmup_cb,
    const ksl::timespan& flush_interval,
    const std::function<void()>& worker_teardown_cb):
    _formatter(formatter),
    _sinks(sinks),
    _q(queue_size),
    _flush_requested(false),
    _terminate_requested(false),
    _overflow_policy(overflow_policy),
    _worker_warmup_cb(worker_warmup_cb),
    _flush_interval(flush_interval),
    _worker_teardown_cb(worker_teardown_cb),
    _worker_thread()
{
    _worker_thread.set_name("alogger");
    _started = _worker_thread.start(std::bind(&async_log_helper::worker_loop, this));
}

// Send to the worker thread termination message(level=off)
// and wait for it to finish gracefully
inline kob::log::async_log_helper::~async_log_helper()
{
    try
    {
        _started = false;
        push_msg(async_msg(async_msg_type::terminate));
        _worker_thread.join();
    }
    catch (...) // don't crash in destructor
    {}
}


//Try to push and block until succeeded (if the policy is not to discard when the queue is full)
inline void kob::log::async_log_helper::log(const log_msg& msg)
{
    push_msg(async_msg(msg));


}

inline void kob::log::async_log_helper::push_msg(async_log_helper::async_msg&& new_msg)
{
    throw_if_bad_worker();
    if (!_q.enqueue(std::move(new_msg)) && _overflow_policy != async_overflow_policy::discard_log_msg) {

        ksl::timestamp last_op_time;
        ksl::timestamp now = last_op_time;
        do {
            now.update();
            sleep_or_yield(now, last_op_time);
        }
        while (!_q.enqueue(std::move(new_msg)));
    }

}

inline void kob::log::async_log_helper::flush()
{
    push_msg(async_msg(async_msg_type::flush));
}

inline void kob::log::async_log_helper::worker_loop()
{
    try {
        if (_worker_warmup_cb) {
            _worker_warmup_cb();
        }
        ksl::timestamp last_pop;
        ksl::timestamp last_flush = last_pop;
        while(process_next_msg(last_pop, last_flush));
        if (_worker_teardown_cb) {
            _worker_teardown_cb();
        }
    }
    catch (const std::exception& ex)
    {
        _last_workerthread_ex = std::make_shared<log_ex>(std::string("async_logger worker thread exception: ") + ex.what());
    }
    catch (...)
    {
        _last_workerthread_ex = std::make_shared<log_ex>("async_logger worker thread exception");
    }
}

// process next message in the queue
// return true if this thread should still be active (while no terminate msg was received)
inline bool kob::log::async_log_helper::process_next_msg(log_clock& last_pop, log_clock& last_flush)
{

    async_msg incoming_async_msg;


    if (_q.dequeue(incoming_async_msg)) {
        last_pop = now();
        switch (incoming_async_msg.msg_type) {

            case async_msg_type::flush:
                _flush_requested = true;
                break;

            case async_msg_type::terminate:
                _flush_requested = true;
                _terminate_requested = true;
                break;

            default:
                log_msg incoming_log_msg;
                incoming_async_msg.fill_log_msg(incoming_log_msg);
                _formatter->format(incoming_log_msg);
                std::vector<std::shared_ptr<sink>>::iterator it = _sinks.begin();
                for (; it != _sinks.end(); it++) {
                    std::shared_ptr<sink> s = (*it);
                    s->log(incoming_log_msg);
                }
        };
        return true;
    }

    // Handle empty queue..
    // This is the only place where the queue can terminate or flush to avoid losing messages already in the queue
    else
    {
        ksl::timestamp now;
        handle_flush_interval(now, last_flush);
        sleep_or_yield(now, last_pop);
        return !_terminate_requested;

    }
}

// flush all sinks if _flush_interval_ms has expired
inline void kob::log::async_log_helper::handle_flush_interval(log_clock& now, log_clock& last_flush)
{
    bool should_flush = _flush_requested || 
                        (_flush_interval != 0 && ksl::timespan(now - last_flush) >= _flush_interval);
    if (should_flush) {
        std::vector<std::shared_ptr<sink>>::iterator it = _sinks.begin();
        for (; it != _sinks.end(); it++) {
            std::shared_ptr<sink> s = *it;
            s->flush();
        }
        last_flush.update();
        now = last_flush;
        _flush_requested = false;
    }
}

inline void kob::log::async_log_helper::set_formatter(formatter_ptr msg_formatter)
{
    _formatter = msg_formatter;
}


// spin, yield or sleep. use the time passed since last message as a hint
inline void kob::log::async_log_helper::sleep_or_yield(const log_clock& now, const log_clock& last_op_time)
{
    ksl::timespan time_since_op = now - last_op_time;

    // spin upto 50 micros
    if (time_since_op <= 50)
        return;

    // yield upto 150 micros
    if (time_since_op <= 100) {
         ksl::this_thread::yield();
         return ;
    }


    // sleep for 20 ms upto 200 ms
    if (time_since_op <= 200) {
         ksl::this_thread::sleep_for(20);
    }

    // sleep for 200 ms
     ksl::this_thread::sleep_for(200*1000);
}

// throw if the worker thread threw an exception or not active
inline void kob::log::async_log_helper::throw_if_bad_worker()
{
    if (_last_workerthread_ex) {
        std::shared_ptr<log_ex> ex = std::move(_last_workerthread_ex);
        throw *ex;
    }
}

#endif





