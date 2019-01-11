#ifndef LOG_ASYNCLOG_H_
#define LOG_ASYNCLOG_H_

#include <kob/log/common.h>
#include <kob/ksl/timespan.h>
#include <kob/log/logger.h>
#include <functional>
#include <string>
#include <memory>

namespace kob {

namespace log {
class async_log_helper;

typedef std::function<void()>  async_log_callback;

class async_logger :public logger {
public:
    template<class It>
    async_logger(const std::string& name,
                 const It& begin,
                 const It& end,
                 size_t queue_size,
                 const async_overflow_policy overflow_policy =  async_overflow_policy::block_retry,
                 const async_log_callback& worker_warmup_cb = async_log_callback(),
                 const ksl::timespan& flush_interval = ksl::timespan(),
                 const async_log_callback& worker_teardown_cb = async_log_callback());

    async_logger(const std::string& logger_name,
                 sinks_init_list sinks,
                 size_t queue_size,
                 const async_overflow_policy overflow_policy = async_overflow_policy::block_retry,
                 const async_log_callback& worker_warmup_cb = async_log_callback(),
                 const ksl::timespan& flush_interval = ksl::timespan(),
                 const async_log_callback& worker_teardown_cb = async_log_callback());

    async_logger(const std::string& logger_name,
                 sink_ptr single_sink,
                 size_t queue_size,
                 const async_overflow_policy overflow_policy =  async_overflow_policy::block_retry,
                 const async_log_callback& worker_warmup_cb = async_log_callback(),
                 const ksl::timespan& flush_interval = ksl::timespan(),
                 const async_log_callback& worker_teardown_cb = async_log_callback());


    void flush() ;
protected:
    void _sink_it(log_msg& msg) ;
    void _set_formatter(formatter_ptr msg_formatter) ;
    void _set_pattern(const std::string& pattern);

private:
    std::unique_ptr<async_log_helper> _async_log_helper;
};

}
}
#endif

#include <kob/log/detail/async_logger_impl.h>
