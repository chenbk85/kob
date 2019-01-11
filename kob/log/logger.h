#ifndef KOB_LOG_LOGGER_H_
#define KOB_LOG_LOGGER_H_

#include <kob/log/sink/base_sink.h>
#include <kob/log/common.h>
#include <vector>
#include <memory>
#include <string>


namespace kob {
namespace log {

class logger {
public:
    logger(const std::string& logger_name, sink_ptr single_sink);
    logger(const std::string& name, sinks_init_list);
    template<class It>
    logger(const std::string& name, const It& begin, const It& end);

    virtual ~logger();

    template <typename... Args> void log(log_level lvl, const char* fmt, const Args&... args);
    template <typename... Args> void log(log_level lvl, const char* msg);
    template <typename... Args> void trace(const char* fmt, const Args&... args);
    template <typename... Args> void debug(const char* fmt, const Args&... args);
    template <typename... Args> void info(const char* fmt, const Args&... args);
    template <typename... Args> void warn(const char* fmt, const Args&... args);
    template <typename... Args> void error(const char* fmt, const Args&... args);
    template <typename... Args> void critical(const char* fmt, const Args&... args);

    template <typename T> void log(log_level lvl, const T&);
    template <typename T> void trace(const T&);
    template <typename T> void debug(const T&);
    template <typename T> void info(const T&);
    template <typename T> void warn(const T&);
    template <typename T> void error(const T&);
    template <typename T> void critical(const T&);


    bool should_log(log_level) const;
    void set_level(log_level);
    log_level level() const;
    const std::string& name() const;
    void set_pattern(const std::string&);
    void set_formatter(formatter_ptr);

    // automatically call flush() if message level >= log_level
    void flush_on(log_level log_level);
    virtual void flush();

protected:
    virtual void _sink_it(log_msg&);
    virtual void _set_pattern(const std::string&);
    virtual void _set_formatter(formatter_ptr);

    const std::string        _name;
    std::vector<sink_ptr>    _sinks;
    formatter_ptr            _formatter;
    mutable level_t          _level;
    mutable level_t          _flush_level;
private:
    logger(const logger&);
    logger& operator=(const logger&);
};

} //namespace log
} //namespace kob
#include <kob/log/detail/logger_impl.h>
#endif //KOB_LOG_LOGGER_H_


