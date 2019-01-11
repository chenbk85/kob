#ifndef KOB_LOG_DETAIL_REGISTERY_H_
#define KOB_LOG_DETAIL_REGISTERY_H_

#include <kob/ksl/mutex.h>
#include <kob/ksl/timespan.h>
#include <kob/log/logger.h>
#include <kob/log/async_logger.h>
#include <kob/log/common.h>
#include <functional>
#include <memory>
#include <string>
#include <unordered_map>

namespace kob {
namespace log {

template <class Mutex> 
class registry_t {
public:

    void register_logger(std::shared_ptr<logger> logger)
    {
        ksl::unique_guard<Mutex> lock(_mutex);
        std::string logger_name = logger->name();
        abort_if_exists(logger_name);
        _loggers[logger_name] = logger;
    }


    std::shared_ptr<logger> get(const std::string& logger_name)
    {
        ksl::unique_guard<Mutex> lock(_mutex);
        std::unordered_map <std::string, std::shared_ptr<logger>>::iterator found = _loggers.find(logger_name);
        return found == _loggers.end() ? std::shared_ptr<logger>() : found->second;
    }

    template<class It>
    std::shared_ptr<logger> create(const std::string& logger_name, const It& sinks_begin, const It& sinks_end)
    {
        ksl::unique_guard<Mutex> lock(_mutex);
        abort_if_exists(logger_name);
        std::shared_ptr<logger> new_logger;
        if (_async_mode) {
            new_logger = std::make_shared<async_logger>(logger_name, sinks_begin,
             sinks_end, _async_q_size, _overflow_policy, _worker_warmup_cb, _flush_interval, _worker_teardown_cb);
        } else {
            new_logger = std::make_shared<logger>(logger_name, sinks_begin, sinks_end);
        }

        if (_formatter)
            new_logger->set_formatter(_formatter);

        new_logger->set_level(_level);
        //Add to registry
        _loggers[logger_name] = new_logger;
        return new_logger;
    }

    void drop(const std::string& logger_name)
    {
        ksl::unique_guard<Mutex> lock(_mutex);
        _loggers.erase(logger_name);
    }

    void drop_all()
    {
        ksl::unique_guard<Mutex> lock(_mutex);
        _loggers.clear();
    }
    std::shared_ptr<logger> create(const std::string& logger_name, sinks_init_list sinks)
    {
        return create(logger_name, sinks.begin(), sinks.end());
    }

    std::shared_ptr<logger> create(const std::string& logger_name, sink_ptr sink)
    {
        return create(logger_name, { sink });
    }


    void formatter(formatter_ptr f)
    {
        ksl::unique_guard<Mutex> lock(_mutex);
        _formatter = f;
        std::unordered_map <std::string, std::shared_ptr<logger>>::iterator it = _loggers.begin();
        for (; it != _loggers.end(); it++) {
            it->second->set_formatter(_formatter);
        }
    }

    void set_pattern(const std::string& pattern)
    {
        ksl::unique_guard<Mutex> lock(_mutex);
        _formatter = std::make_shared<pattern_formatter>(pattern);
        std::unordered_map <std::string, std::shared_ptr<logger>>::iterator it = _loggers.begin();
        for (; it != _loggers.end(); it++) {
            it->second->set_formatter(_formatter);
        }
    }

    void set_level(log_level log_level)
    {
        ksl::unique_guard<Mutex> lock(_mutex);
        _level = log_level;
        std::unordered_map <std::string, std::shared_ptr<logger>>::iterator it = _loggers.begin();
        for (; it != _loggers.end(); it++) {
            it->second->set_level(_level);
        }
        
    }

    void set_async_mode(size_t q_size, const async_overflow_policy overflow_policy,
                         const async_log_callback& worker_warmup_cb,
                         const ksl::timespan& flush_interval,
                         const async_log_callback& worker_teardown_cb)
    {
        ksl::unique_guard<Mutex> lock(_mutex);
        _async_mode = true;
        _async_q_size = q_size;
        _overflow_policy = overflow_policy;
        _worker_warmup_cb = worker_warmup_cb;
        _flush_interval = flush_interval;
        _worker_teardown_cb = worker_teardown_cb;
    }

    void set_sync_mode()
    {
        ksl::unique_guard<Mutex> lock(_mutex);
        _async_mode = false;
    }

    static registry_t<Mutex>& instance()
    {
        static registry_t<Mutex> s_instance;
        return s_instance;
    }

private:
    registry_t<Mutex>()
    {
        _overflow_policy = async_overflow_policy::block_retry;
        _level = log_level::ll_info;
        _async_mode = false;
        _async_q_size = 0;

    }
    registry_t<Mutex>(const registry_t<Mutex>&);
    registry_t<Mutex>& operator=(const registry_t<Mutex>&);

    void abort_if_exists(const std::string &logger_name)
    {
        if (_loggers.find(logger_name) != _loggers.end())
            assert(false);
    }
    Mutex _mutex;
    std::unordered_map <std::string, std::shared_ptr<logger>>   _loggers;
    formatter_ptr                                               _formatter;
    log_level                                                  _level;
    bool                                                        _async_mode;
    size_t                                                      _async_q_size;
    async_overflow_policy                                       _overflow_policy;
    async_log_callback                                          _worker_warmup_cb;
    ksl::timespan                                               _flush_interval;
    async_log_callback                                          _worker_teardown_cb;
};

typedef registry_t<ksl::mutex> registry;

} //namespace log
} //namespace kob

#endif //KOB_LOG_DETAIL_REGISTERY_H_