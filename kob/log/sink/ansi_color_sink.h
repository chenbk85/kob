#ifndef LOG_SINK_ANSICOLOR_SINK_H_
#define LOG_SINK_ANSICOLOR_SINK_H_

#include <kob/log/sink/base_sink.h>
#include <kob/log/common.h>
#include <kob/ksl/color.h>
#include <string>
#include <map>

namespace kob {
namespace log {

class ansi_color_sink : public sink {
public:
    ansi_color_sink(sink_ptr wrapped_sink);
    virtual ~ansi_color_sink();

    ansi_color_sink(const ansi_color_sink& other) = delete;
    ansi_color_sink& operator=(const ansi_color_sink& other) = delete;

    virtual void log(const log_msg& msg) ;
    virtual void flush() ;

    void set_color(log_level level, const std::string& color);



protected:
    sink_ptr                          _sink;
    std::map<log_level, std::string> _colors;
};

inline ansi_color_sink::ansi_color_sink(sink_ptr wrapped_sink) : _sink(wrapped_sink)
{
    _colors[log_level::ll_trace]   = ksl::color::cyan;
    _colors[log_level::ll_debug]   = ksl::color::blue;
    _colors[log_level::ll_info]    = ksl::color::green;
    _colors[log_level::ll_warn]    = ksl::color::yellow;
    _colors[log_level::ll_err]     = ksl::color::red;
    _colors[log_level::ll_critical] = ksl::color::magenta;
    _colors[log_level::ll_off]      = ksl::color::magenta;
}

inline void ansi_color_sink::log(const log_msg& msg)
{
    // Wrap the originally formatted message in color codes
    const std::string& prefix = _colors[msg.level];
    const std::string& s = msg.formatted.str();
    const std::string& suffix = ksl::color::reset;
    log_msg m;
    m.level = msg.level;
    m.logger_name = msg.logger_name;
    m.time = msg.time;
    m.thread_id = msg.thread_id;
    m.formatted << prefix  << s << suffix;
    _sink->log(m);
}

inline void ansi_color_sink::flush()
{
    _sink->flush();
}

inline void ansi_color_sink::set_color(log_level level, const std::string& color)
{
    _colors[level] = color;
}

inline ansi_color_sink::~ansi_color_sink()
{
    flush();
}

} 
} 

#endif