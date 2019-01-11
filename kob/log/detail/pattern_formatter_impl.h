#ifndef LOG_DETAIL_PATTERN_FOMATTER_IMPL_H_
#define LOG_DETAIL_PATTERN_FOMATTER_IMPL_H_

#include <kob/log/formatter.h>
#include <kob/log/detail/log_msg.h>
#include <kob/log/detail/os.h>
#include <kob/log/detail/format.h>
#include <kob/ksl/mutex.h>
#include <kob/ksl/timestamp.h>
#include <kob/ksl/timespan.h>
#include <ctime>
#include <memory>
#include <mutex>
#include <string>
#include <thread>
#include <utility>
#include <vector>

namespace kob {
namespace log {

class flag_formatter {
public:
    virtual ~flag_formatter() {}
    virtual void format(log_msg& msg, const std::tm& tm_time) = 0;
};

///////////////////////////////////////////////////////////////////////
// name & level pattern appenders
///////////////////////////////////////////////////////////////////////
class name_formatter :public flag_formatter {
    void format(log_msg& msg, const std::tm&) 
    {
        msg.formatted << *msg.logger_name;
    }
};


// log level appender
class level_formatter :public flag_formatter {
    void format(log_msg& msg, const std::tm&) 
    {
        msg.formatted << to_str(msg.level);
    }
};
// short log level appender
class short_level_formatter :public flag_formatter {
    void format(log_msg& msg, const std::tm&) 
    {
        msg.formatted << to_short_str(msg.level);
    }
};

///////////////////////////////////////////////////////////////////////
// Date time pattern appenders
///////////////////////////////////////////////////////////////////////

static const char* ampm(const tm& t)
{
    return t.tm_hour >= 12 ? "PM" : "AM";
}

static int to12h(const tm& t)
{
    return t.tm_hour > 12 ? t.tm_hour - 12 : t.tm_hour;
}

//Abbreviated weekday name
static const std::string days[] { "Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat" };
class a_formatter :public flag_formatter {
    void format(log_msg& msg, const std::tm& tm_time) 
    {
        msg.formatted << days[tm_time.tm_wday];
    }
};

//Full weekday name
static const std::string full_days[] { "Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday" };
class A_formatter :public flag_formatter {
    void format(log_msg& msg, const std::tm& tm_time) 
    {
        msg.formatted << full_days[tm_time.tm_wday];
    }
};

//Abbreviated month
static const std::string  months[] { "Jan", "Feb", "Mar", "Apr", "May", "June", "July", "Aug", "Sept", "Oct", "Nov", "Dec" };
class b_formatter :public flag_formatter {
    void format(log_msg& msg, const std::tm& tm_time) 
    {
        msg.formatted<< months[tm_time.tm_mon];
    }
};

//Full month name
static const std::string full_months[] { "January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December" };
class B_formatter :public flag_formatter {
    void format(log_msg& msg, const std::tm& tm_time) 
    {
        msg.formatted << full_months[tm_time.tm_mon];
    }
};


//write 2 ints seperated by sep with padding of 2
static fermat::MemoryWriter& pad_n_join(fermat::MemoryWriter& w, int v1, int v2, char sep)
{
    w << fermat::pad(v1, 2, '0') << sep << fermat::pad(v2, 2, '0');
    return w;
}

//write 3 ints seperated by sep with padding of 2
static fermat::MemoryWriter& pad_n_join(fermat::MemoryWriter& w, int v1, int v2, int v3, char sep)
{
    w << fermat::pad(v1, 2, '0') << sep << fermat::pad(v2, 2, '0') << sep << fermat::pad(v3, 2, '0');
    return w;
}


//Date and time representation (Thu Aug 23 15:35:46 2014)
class c_formatter :public flag_formatter
{
    void format(log_msg& msg, const std::tm& tm_time) 
    {
        msg.formatted << days[tm_time.tm_wday] << ' ' << months[tm_time.tm_mon] << ' ' << tm_time.tm_mday << ' ';
        pad_n_join(msg.formatted, tm_time.tm_hour, tm_time.tm_min, tm_time.tm_sec, ':') << ' ' << tm_time.tm_year + 1900;
    }
};


// year - 2 digit
class C_formatter :public flag_formatter
{
    void format(log_msg& msg, const std::tm& tm_time) 
    {
        msg.formatted << fermat::pad(tm_time.tm_year % 100, 2, '0');
    }
};



// Short MM/DD/YY date, equivalent to %m/%d/%y 08/23/01
class D_formatter :public flag_formatter
{
    void format(log_msg& msg, const std::tm& tm_time) 
    {
        pad_n_join(msg.formatted, tm_time.tm_mon + 1, tm_time.tm_mday, tm_time.tm_year % 100, '/');
    }
};


// year - 4 digit
class Y_formatter :public flag_formatter
{
    void format(log_msg& msg, const std::tm& tm_time) 
    {
        msg.formatted << tm_time.tm_year + 1900;
    }
};

// month 1-12
class m_formatter :public flag_formatter
{
    void format(log_msg& msg, const std::tm& tm_time) 
    {
        msg.formatted << fermat::pad(tm_time.tm_mon + 1, 2, '0');
    }
};

// day of month 1-31
class d_formatter :public flag_formatter
{
    void format(log_msg& msg, const std::tm& tm_time) 
    {
        msg.formatted << fermat::pad(tm_time.tm_mday, 2, '0');
    }
};

// hours in 24 format  0-23
class H_formatter :public flag_formatter
{
    void format(log_msg& msg, const std::tm& tm_time) 
    {
        msg.formatted << fermat::pad(tm_time.tm_hour, 2, '0');
    }
};

// hours in 12 format  1-12
class I_formatter :public flag_formatter
{
    void format(log_msg& msg, const std::tm& tm_time) 
    {
        msg.formatted << fermat::pad(to12h(tm_time), 2, '0');
    }
};

// minutes 0-59
class M_formatter :public flag_formatter
{
    void format(log_msg& msg, const std::tm& tm_time) 
    {
        msg.formatted << fermat::pad(tm_time.tm_min, 2, '0');
    }
};

// seconds 0-59
class S_formatter :public flag_formatter
{
    void format(log_msg& msg, const std::tm& tm_time) 
    {
        msg.formatted << fermat::pad(tm_time.tm_sec, 2, '0');
    }
};

// milliseconds
class e_formatter :public flag_formatter
{
    void format(log_msg& msg, const std::tm&) 
    {
        int64_t duration = msg.time.epoch_microseconds();
        int64_t millis = (duration/1000) % 1000;
        msg.formatted << fermat::pad(static_cast<int>(millis), 3, '0');
    }
};

// microseconds
class f_formatter :public flag_formatter
{
    void format(log_msg& msg, const std::tm&) 
    {
        int64_t duration = msg.time.epoch_microseconds();
        int64_t micros = (duration) % 1000000; 
        msg.formatted << fermat::pad(static_cast<int>(micros), 6, '0');
    }
};

// nanoseconds
class F_formatter :public flag_formatter
{
    void format(log_msg& msg, const std::tm&) 
    {
        int64_t duration = msg.time.epoch_microseconds();
        int64_t ns = (duration*1000) % 1000000000;
        msg.formatted << fermat::pad(static_cast<int>(ns), 9, '0');
    }
};

// AM/PM
class p_formatter :public flag_formatter
{
    void format(log_msg& msg, const std::tm& tm_time) 
    {
        msg.formatted << ampm(tm_time);
    }
};


// 12 hour clock 02:55:02 pm
class r_formatter :public flag_formatter
{
    void format(log_msg& msg, const std::tm& tm_time) 
    {
        pad_n_join(msg.formatted, to12h(tm_time), tm_time.tm_min, tm_time.tm_sec, ':') << ' ' << ampm(tm_time);
    }
};

// 24-hour HH:MM time, equivalent to %H:%M
class R_formatter :public flag_formatter
{
    void format(log_msg& msg, const std::tm& tm_time) 
    {
        pad_n_join(msg.formatted, tm_time.tm_hour, tm_time.tm_min, ':');
    }
};

// ISO 8601 time format (HH:MM:SS), equivalent to %H:%M:%S
class T_formatter :public flag_formatter
{
    void format(log_msg& msg, const std::tm& tm_time) 
    {
        pad_n_join(msg.formatted, tm_time.tm_hour, tm_time.tm_min, tm_time.tm_sec, ':');
    }
};


// ISO 8601 offset from UTC in timezone (+-HH:MM)
class z_formatter : public flag_formatter {
public:
    static const int64_t cache_refresh = 5000000;

    z_formatter() :_last_update(0) {}

    void format(log_msg& msg, const std::tm& tm_time) 
    {
        // No need to chache under gcc,
        // it is very fast (already stored in tm.tm_gmtoff)
        int total_minutes = utc_minutes_offset(tm_time);
        int h = total_minutes / 60;
        int m = total_minutes % 60;
        //minus sign will be printed anyway if negative
        if (h >= 0)  {
            msg.formatted << '+';
        }
        pad_n_join(msg.formatted, h, m, ':');
    }
private:
    log_clock      _last_update;
    int            _offset_minutes;
    ksl::mutex     _mutex;

    int get_cached_offset(const log_msg& msg, const std::tm& tm_time)
    {
        ksl::unique_guard<ksl::mutex> lk(_mutex);
        if (msg.time - _last_update >= cache_refresh) {
            _offset_minutes = utc_minutes_offset(tm_time);
            _last_update = msg.time;
        }
        return _offset_minutes;
    }

    z_formatter(const z_formatter&);
    z_formatter& operator=(const z_formatter&);
};


//Thread id
class t_formatter :public flag_formatter {
    void format(log_msg& msg, const std::tm&) 
    {
        msg.formatted << msg.thread_id;
    }
};


class v_formatter :public flag_formatter {
    void format(log_msg& msg, const std::tm&) 
    {
        msg.formatted << msg.raw.c_str();
    }
};

class ch_formatter :public flag_formatter {
public:
    explicit ch_formatter(char ch) : _ch(ch)
    {}
    void format(log_msg& msg, const std::tm&) 
    {
        msg.formatted << _ch;
    }
private:
    char _ch;
};


//aggregate user chars to display as is
class aggregate_formatter :public flag_formatter
{
public:
    aggregate_formatter()
    {}
    void add_ch(char ch)
    {
        _str += ch;
    }
    void format(log_msg& msg, const std::tm&) 
    {
        msg.formatted << _str;
    }
private:
    std::string _str;
};

// Full info formatter
// pattern: [%Y-%m-%d %H:%M:%S.%e] [%n] [%l] %v
class full_formatter :public flag_formatter
{
    void format(log_msg& msg, const std::tm& tm_time) 
    {
        int64_t duration = msg.time.epoch_microseconds();
        int64_t millis = (duration/1000) % 1000;

        /* Slower version(while still very fast - about 3.2 million lines/sec under 10 threads),
        msg.formatted.write("[{:d}-{:02d}-{:02d} {:02d}:{:02d}:{:02d}.{:03d}] [{}] [{}] {} ",
        tm_time.tm_year + 1900,
        tm_time.tm_mon + 1,
        tm_time.tm_mday,
        tm_time.tm_hour,
        tm_time.tm_min,
        tm_time.tm_sec,
        static_cast<int>(millis),
        msg.logger_name,
        to_str(msg.level),
        msg.raw.str());*/


        // Faster (albeit uglier) way to format the line (5.6 million lines/sec under 10 threads)
        msg.formatted << '[' << static_cast<unsigned int>(tm_time.tm_year + 1900) << '-'
                      << fermat::pad(static_cast<unsigned int>(tm_time.tm_mon + 1), 2, '0') << '-'
                      << fermat::pad(static_cast<unsigned int>(tm_time.tm_mday), 2, '0') << ' '
                      << fermat::pad(static_cast<unsigned int>(tm_time.tm_hour), 2, '0') << ':'
                      << fermat::pad(static_cast<unsigned int>(tm_time.tm_min), 2, '0') << ':'
                      << fermat::pad(static_cast<unsigned int>(tm_time.tm_sec), 2, '0') << '.'
                      << fermat::pad(static_cast<unsigned int>(millis), 3, '0') << "] ";



        msg.formatted << '[' << *msg.logger_name << "] ";
        msg.formatted << '[' << to_str(msg.level) << "] ";
        msg.formatted << msg.raw.c_str();
    }
};

///////////////////////////////////////////////////////////////////////////////
// pattern_formatter inline impl
///////////////////////////////////////////////////////////////////////////////
inline pattern_formatter::pattern_formatter(const std::string& pattern)
{
    compile_pattern(pattern);
}

inline void pattern_formatter::compile_pattern(const std::string& pattern)
{
    std::string::const_iterator end = pattern.end();
    std::unique_ptr<aggregate_formatter> user_chars;
    for (std::string::const_iterator it = pattern.begin(); it != end; ++it) {
        if (*it == '%') {
            if (user_chars) {
                //append user chars found so far
                _formatters.push_back(std::move(user_chars));
            }
            
            if (++it != end) {
                handle_flag(*it);
            } else {
                break;
            }
        } else {
            // chars not following the % sign should be displayed as is
            if (!user_chars) {
                user_chars = std::unique_ptr<aggregate_formatter>(new aggregate_formatter());
            }
            user_chars->add_ch(*it);
        }
    }
    //append raw chars found so far
    if (user_chars)  {
        _formatters.push_back(std::move(user_chars));
    }

}

inline void pattern_formatter::handle_flag(char flag)
{
    switch (flag) {
    // logger name
    case 'n':
        _formatters.push_back(std::unique_ptr<flag_formatter>(new name_formatter()));
        break;

    case 'l':
        _formatters.push_back(std::unique_ptr<flag_formatter>(new level_formatter()));
        break;

    case 'L':
        _formatters.push_back(std::unique_ptr<flag_formatter>(new short_level_formatter()));
        break;

    case('t') :
        _formatters.push_back(std::unique_ptr<flag_formatter>(new t_formatter()));
        break;

    case('v') :
        _formatters.push_back(std::unique_ptr<flag_formatter>(new v_formatter()));
        break;

    case('a') :
        _formatters.push_back(std::unique_ptr<flag_formatter>(new a_formatter()));
        break;

    case('A') :
        _formatters.push_back(std::unique_ptr<flag_formatter>(new A_formatter()));
        break;

    case('b') :
    case('h') :
        _formatters.push_back(std::unique_ptr<flag_formatter>(new b_formatter()));
        break;

    case('B') :
        _formatters.push_back(std::unique_ptr<flag_formatter>(new B_formatter()));
        break;
    case('c') :
        _formatters.push_back(std::unique_ptr<flag_formatter>(new c_formatter()));
        break;

    case('C') :
        _formatters.push_back(std::unique_ptr<flag_formatter>(new C_formatter()));
        break;

    case('Y') :
        _formatters.push_back(std::unique_ptr<flag_formatter>(new Y_formatter()));
        break;

    case('D') :
    case('x') :

        _formatters.push_back(std::unique_ptr<flag_formatter>(new D_formatter()));
        break;

    case('m') :
        _formatters.push_back(std::unique_ptr<flag_formatter>(new m_formatter()));
        break;

    case('d') :
        _formatters.push_back(std::unique_ptr<flag_formatter>(new d_formatter()));
        break;

    case('H') :
        _formatters.push_back(std::unique_ptr<flag_formatter>(new H_formatter()));
        break;

    case('I') :
        _formatters.push_back(std::unique_ptr<flag_formatter>(new I_formatter()));
        break;

    case('M') :
        _formatters.push_back(std::unique_ptr<flag_formatter>(new M_formatter()));
        break;

    case('S') :
        _formatters.push_back(std::unique_ptr<flag_formatter>(new S_formatter()));
        break;

    case('e') :
        _formatters.push_back(std::unique_ptr<flag_formatter>(new e_formatter()));
        break;

    case('f') :
        _formatters.push_back(std::unique_ptr<flag_formatter>(new f_formatter()));
        break;
    case('F') :
        _formatters.push_back(std::unique_ptr<flag_formatter>(new F_formatter()));
        break;

    case('p') :
        _formatters.push_back(std::unique_ptr<flag_formatter>(new p_formatter()));
        break;

    case('r') :
        _formatters.push_back(std::unique_ptr<flag_formatter>(new r_formatter()));
        break;

    case('R') :
        _formatters.push_back(std::unique_ptr<flag_formatter>(new R_formatter()));
        break;

    case('T') :
    case('X') :
        _formatters.push_back(std::unique_ptr<flag_formatter>(new T_formatter()));
        break;

    case('z') :
        _formatters.push_back(std::unique_ptr<flag_formatter>(new z_formatter()));
        break;

    case ('+'):
        _formatters.push_back(std::unique_ptr<flag_formatter>(new full_formatter()));
        break;

    default: //Unkown flag appears as is
        _formatters.push_back(std::unique_ptr<flag_formatter>(new ch_formatter('%')));
        _formatters.push_back(std::unique_ptr<flag_formatter>(new ch_formatter(flag)));
        break;
    }
}


inline void pattern_formatter::format(log_msg& msg)
{
    try
    {
        std::tm tm_time = localtime(msg.time.epoch_time());
        for (std::vector<std::unique_ptr<flag_formatter>>::iterator it = _formatters.begin();
            it !=   _formatters.end(); it++) {
            
            (*it)->format(msg, tm_time);
        }
        //write eol
        msg.formatted.write(eol, eol_size);
    }
    catch(const fermat::FormatError& e)
    {
        throw log_ex(fermat::format("formatting error while processing format string: {}", e.what()));
    }
}

}
}
#endif