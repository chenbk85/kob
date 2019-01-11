#ifndef KOB_LOG_DETAIL_FMTLOG_IMPL_H_
#define KOB_LOG_DETAIL_FMTLOG_IMPL_H_
//
// Global registry functions
//
#include <kob/log/fmtlog.h>
#include <kob/log/detail/registry.h>
#include <kob/log/sink/file_sinks.h>
#include <kob/log/sink/stdout_sinks.h>
#include <kob/log/sink/ansi_color_sink.h>

#include <chrono>
#include <functional>
#include <memory>
#include <string>

namespace kob {

namespace log {

inline void register_logger(std::shared_ptr<logger> logger)
{
    return registry::instance().register_logger(logger);
}

inline std::shared_ptr<logger> get(const std::string& name)
{
    return registry::instance().get(name);
}

inline void drop(const std::string &name)
{
    registry::instance().drop(name);
}

// Create multi/single threaded simple file logger
inline std::shared_ptr<logger> basic_logger_mt(const std::string& logger_name, 
                                                const std::string& filename,
                                                bool force_flush)
{
    return create<simple_file_sink_mt>(logger_name, filename, force_flush);
}

inline std::shared_ptr<logger> basic_logger_st(const std::string& logger_name, 
                                            const std::string& filename, 
                                            bool force_flush)
{
    return create<simple_file_sink_st>(logger_name, filename, force_flush);
}

// Create multi/single threaded rotating file logger
inline std::shared_ptr<logger> rotating_logger_mt(const std::string& logger_name, 
                                                    const std::string& filename,
                                                     size_t max_file_size, 
                                                     size_t max_files, 
                                                     bool force_flush)
{
    return create<rotating_file_sink_mt>(logger_name, filename, 
                                                "txt", 
                                                max_file_size, 
                                                max_files, 
                                                force_flush);
}

inline std::shared_ptr<logger> rotating_logger_st(const std::string& logger_name, 
                                                const std::string& filename, 
                                                size_t max_file_size, 
                                                size_t max_files, 
                                                bool force_flush)
{
    return create<rotating_file_sink_st>(logger_name, filename, 
                                            "txt", 
                                            max_file_size, 
                                            max_files, 
                                            force_flush);
}

// Create file logger which creates new file at midnight):
inline std::shared_ptr<logger> daily_logger_mt(const std::string& logger_name,
                                                 const std::string& filename, 
                                                 int hour, 
                                                 int minute, 
                                                 bool force_flush)
{
    return create<daily_file_sink_mt>(logger_name, filename, 
                                        "txt", 
                                        hour, 
                                        minute, 
                                        force_flush);
}

inline std::shared_ptr<logger> daily_logger_st(const std::string& logger_name, 
                                                const std::string& filename, 
                                                int hour, 
                                                int minute, 
                                                bool force_flush)
{
    return create<daily_file_sink_st>(logger_name, 
                                    filename, 
                                    "txt", 
                                    hour, 
                                    minute, 
                                    force_flush);
}

// Create stdout/stderr loggers (with optinal color support)
inline std::shared_ptr<logger> create_console_logger(
                            const std::string& logger_name, 
                            sink_ptr sink, 
                            bool color)
{
    if (color) //use color wrapper sink
        sink = std::make_shared<ansi_color_sink>(sink);
    return registry::instance().create(logger_name, sink);
}

inline std::shared_ptr<logger> stdout_logger_mt(const std::string& logger_name, 
                                                bool color)
{
    return create_console_logger(logger_name, stdout_sink_mt::instance(), 
                                color);
}

inline std::shared_ptr<logger> stdout_logger_st(const std::string& logger_name, 
                                                bool color)
{
    return create_console_logger(logger_name, 
            stdout_sink_st::instance(), color);
}

inline std::shared_ptr<logger> stderr_logger_mt(const std::string& logger_name,
                                            bool color)
{
    return create_console_logger(logger_name, stderr_sink_mt::instance(),
                                    color);
}

inline std::shared_ptr<logger> stderr_logger_st(const std::string& logger_name, 
                                                bool color)
{
    return create_console_logger(logger_name, stderr_sink_st::instance(),
                                color);
}


// Create and register a logger a single sink
inline std::shared_ptr<logger> create(const std::string& logger_name, 
                                        const sink_ptr& sink)
{
    return registry::instance().create(logger_name, sink);
}

//Create logger with multiple sinks

inline std::shared_ptr<logger> create(const std::string& logger_name, 
                                        sinks_init_list sinks)
{
    return registry::instance().create(logger_name, sinks);
}


template <typename Sink, typename... Args>
inline std::shared_ptr<logger> create(const std::string& logger_name, Args... args)
{
    sink_ptr sink = std::make_shared<Sink>(args...);
    return registry::instance().create(logger_name, sink);
}


template<class It>
inline std::shared_ptr<logger> create(const std::string& logger_name, 
                                        const It& sinks_begin, 
                                        const It& sinks_end)
{
    return registry::instance().create(logger_name, sinks_begin, sinks_end);
}

inline void set_formatter(formatter_ptr f)
{
    registry::instance().formatter(f);
}

inline void set_pattern(const std::string& format_string)
{
    return registry::instance().set_pattern(format_string);
}

inline void set_level(log_level log_level)
{
    return registry::instance().set_level(log_level);
}


inline void set_async_mode(size_t queue_size, 
                            const async_overflow_policy overflow_policy, 
                            const std::function<void()>& worker_warmup_cb, 
                            const ksl::timespan& flush_interval,
                            const std::function<void()>& worker_teardown_cb)
{
    registry::instance().set_async_mode(queue_size, overflow_policy,
                                         worker_warmup_cb,
                                        flush_interval, 
                                        worker_teardown_cb);
}

inline void set_sync_mode()
{
    registry::instance().set_sync_mode();
}

inline void drop_all()
{
    registry::instance().drop_all();
}

}
}
#endif