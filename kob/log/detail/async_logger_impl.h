#ifndef LOG_DETAIL_ASYNC_LOG_IMPL_H_
#define LOG_DETAIL_ASYNC_LOG_IMPL_H_

#include <kob/log/detail/async_log_helper.h>
#include <kob/log/async_logger.h>

#include <string>
#include <functional>
#include <memory>

template<class It>
inline kob::log::async_logger::async_logger(const std::string& logger_name,
        const It& begin,
        const It& end,
        size_t queue_size,
        const  async_overflow_policy overflow_policy,
        const std::function<void()>& worker_warmup_cb,
        const ksl::timespan& flush_interval,
        const std::function<void()>& worker_teardown_cb) :
    logger(logger_name, begin, end),
    _async_log_helper(new async_log_helper(_formatter, _sinks, queue_size, overflow_policy, worker_warmup_cb, flush_interval, worker_teardown_cb))
{
}

inline kob::log::async_logger::async_logger(const std::string& logger_name,
        sinks_init_list sinks,
        size_t queue_size,
        const  async_overflow_policy overflow_policy,
        const std::function<void()>& worker_warmup_cb,
        const ksl::timespan& flush_interval,
        const std::function<void()>& worker_teardown_cb) :
        logger(logger_name, sinks.begin(), sinks.end()),
        _async_log_helper(new async_log_helper(_formatter, _sinks, queue_size, overflow_policy, worker_warmup_cb, flush_interval, worker_teardown_cb))
     {}

inline kob::log::async_logger::async_logger(const std::string& logger_name,
        sink_ptr single_sink,
        size_t queue_size,
        const  async_overflow_policy overflow_policy,
        const std::function<void()>& worker_warmup_cb,
        const ksl::timespan& flush_interval,
        const std::function<void()>& worker_teardown_cb) :
          logger(logger_name, { single_sink}),
         _async_log_helper(new async_log_helper(_formatter, _sinks, queue_size, overflow_policy, worker_warmup_cb, flush_interval, worker_teardown_cb))
{}


inline void kob::log::async_logger::flush()
{

    _async_log_helper->flush();
}

inline void kob::log::async_logger::_set_formatter(kob::log::formatter_ptr msg_formatter)
{
    _formatter = msg_formatter;
    _async_log_helper->set_formatter(_formatter);
}

inline void kob::log::async_logger::_set_pattern(const std::string& pattern)
{
    _formatter = std::make_shared<pattern_formatter>(pattern);
    _async_log_helper->set_formatter(_formatter);
}


inline void kob::log::async_logger::_sink_it(log_msg& msg)
{
    _async_log_helper->log(msg);
}

#endif