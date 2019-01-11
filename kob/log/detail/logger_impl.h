#ifndef kob_LOG_DETAIL_LOGFER_IMPL_H_
#define kob_LOG_DETAIL_LOGFER_IMPL_H_

#include <kob/log/logger.h>
#include <kob/log/common.h>
#include <memory>
#include <string>
#include <kob/log/sink/base_sink.h>
// create logger with given name, sinks and the default pattern formatter
// all other ctors will call this one
template<class It>
inline kob::log::logger::logger(const std::string& logger_name, const It& begin, const It& end) :
    _name(logger_name),
    _sinks(begin, end),
    _formatter(std::make_shared<pattern_formatter>("%+"))
{

    // no support under vs2013 for member initialization for std::atomic
    _level = static_cast<int>(log_level::ll_info);
    _flush_level = static_cast<int>(log_level::ll_off);
}

// ctor with sinks as init list
inline kob::log::logger::logger(const std::string& logger_name, sinks_init_list sinks_list) :
    _name(logger_name), 
    _sinks(sinks_list.begin(), sinks_list.end()),
    _formatter(std::make_shared<pattern_formatter>("%+"))
    {}


// ctor with single sink
inline kob::log::logger::logger(const std::string& logger_name, kob::log::sink_ptr single_sink) :
    _name(logger_name),
    _sinks( {single_sink}),
    _formatter(std::make_shared<pattern_formatter>("%+"))
    {

    }


inline kob::log::logger::~logger() = default;


inline void kob::log::logger::set_formatter(kob::log::formatter_ptr msg_formatter)
{
    _set_formatter(msg_formatter);
}

inline void kob::log::logger::set_pattern(const std::string& pattern)
{
    _set_pattern(pattern);
}


template <typename... Args>
inline void kob::log::logger::log(log_level lvl, const char* fmt, const Args&... args)
{
    if (!should_log(lvl)) return;

    log_msg log_msg(&_name, lvl);
    try {
        log_msg.raw.write(fmt, args...);
    } catch (fermat::FormatError &ex) {
        throw kob::log::log_ex(std::string("format error in \"") + fmt + "\": " + ex.what());
    }

    _sink_it(log_msg);

}

template <typename... Args>
inline void kob::log::logger::log(log_level lvl, const char* msg)
{
    if (!should_log(lvl)) return;

    log_msg log_msg(&_name, lvl);
    log_msg.raw << msg;
    _sink_it(log_msg);

}

template<typename T>
inline void kob::log::logger::log(log_level lvl, const T& msg)
{
    if (!should_log(lvl)) return;

    log_msg log_msg(&_name, lvl);
    log_msg.raw << msg;
    _sink_it(log_msg);

}


template <typename... Args>
inline void kob::log::logger::trace(const char* fmt, const Args&... args)
{
    log(log_level::ll_trace, fmt, args...);
}

template <typename... Args>
inline void kob::log::logger::debug(const char* fmt, const Args&... args)
{
    log(log_level::ll_debug, fmt, args...);
}

template <typename... Args>
inline void kob::log::logger::info(const char* fmt, const Args&... args)
{
    log(log_level::ll_info, fmt, args...);
}


template <typename... Args>
inline void kob::log::logger::warn(const char* fmt, const Args&... args)
{
    log(log_level::ll_warn, fmt, args...);
}

template <typename... Args>
inline void kob::log::logger::error(const char* fmt, const Args&... args)
{
    log(log_level::ll_err, fmt, args...);
}

template <typename... Args>
inline void kob::log::logger::critical(const char* fmt, const Args&... args)
{
    log(log_level::ll_critical, fmt, args...);
}


template<typename T>
inline void kob::log::logger::trace(const T& msg)
{
    log(log_level::ll_trace, msg);
}

template<typename T>
inline void kob::log::logger::debug(const T& msg)
{
    log(log_level::ll_debug, msg);
}


template<typename T>
inline void kob::log::logger::info(const T& msg)
{
    log(log_level::ll_info, msg);
}


template<typename T>
inline void kob::log::logger::warn(const T& msg)
{
    log(log_level::ll_warn, msg);
}

template<typename T>
inline void kob::log::logger::error(const T& msg)
{
    log(log_level::ll_err, msg);
}

template<typename T>
inline void kob::log::logger::critical(const T& msg)
{
    log(log_level::ll_critical, msg);
}




//
// name and level
//
inline const std::string& kob::log::logger::name() const
{
    return _name;
}

inline void kob::log::logger::set_level(kob::log::log_level ll)
{
    _level.store(static_cast<int>(ll));
}

inline void kob::log::logger::flush_on(log_level ll)
{
    _flush_level.store(static_cast<int>(ll));
}

inline kob::log::log_level kob::log::logger::level() const
{
    return static_cast<kob::log::log_level>(_level.load());
}

inline bool kob::log::logger::should_log(kob::log::log_level msg_level) const
{
    return static_cast<int>(msg_level) >= _level.load();
}

//
// protected virtual called at end of each user log call (if enabled) by the line_logger
//
inline void kob::log::logger::_sink_it(log_msg& msg)
{
    _formatter->format(msg);
    std::vector<sink_ptr>::iterator it = _sinks.begin();
    for ( ; it != _sinks.end(); it++) {
        sink_ptr s = *(it);
        s->log(msg);
    }
       
    level_t flush_level;
    flush_level.store(_flush_level.load());
    if (static_cast<int>(msg.level) >= flush_level)
        flush();
}

inline void kob::log::logger::_set_pattern(const std::string& pattern)
{
    _formatter = std::make_shared<pattern_formatter>(pattern);
}
inline void kob::log::logger::_set_formatter(formatter_ptr msg_formatter)
{
    _formatter = msg_formatter;
}

inline void kob::log::logger::flush()
{
    std::vector<sink_ptr>::iterator it = _sinks.begin();
    for ( ; it != _sinks.end(); it++) {
         sink_ptr s = *(it);
         s->flush();
    }
}
#endif