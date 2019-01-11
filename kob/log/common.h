#ifndef KOB_LOG_COMMON_H_
#define KOB_LOG_COMMON_H_

#include <atomic>
#include <kob/ksl/timestamp.h>
#include <kob/ksl/filesystem.h>
#include <memory>
#include <string>
#include <exception>
#include <initializer_list>
#include <string.h>

namespace kob {
namespace log{

class formatter;
class sink;

typedef ksl::timestamp                      log_clock;
typedef std::shared_ptr<sink>               sink_ptr; 
typedef std::initializer_list < sink_ptr >  sinks_init_list;
typedef std::shared_ptr<formatter>          formatter_ptr;
typedef std::atomic<int>                    level_t;
typedef ksl::filesystem::path_view          filename_t;

enum class log_level : int {
    ll_trace = 0,
    ll_debug = 1,
    ll_info = 2,
    ll_warn = 3,
    ll_err = 4,
    ll_critical = 5,
    ll_off = 6
} ;

static const char* level_names[] { "trace", "debug", "info",  "warning", "error", "critical", "off"};

static const char* short_level_names[] { "T", "D", "I", "W", "E", "C", "O"};

inline const char* to_str(log_level l)
{
    return level_names[static_cast<int>(l)];
}

inline const char* to_short_str(log_level l)
{
    return short_level_names[static_cast<int>(l)];
}

enum  class async_overflow_policy {
    block_retry,
    discard_log_msg
};

inline std::string errno_str(int err_num)
{
    char buf[256];
    size_t buf_size = sizeof(buf);
    return std::string(strerror_r(err_num, buf, buf_size));
}

class log_ex : public std::exception {
public:
    log_ex(const std::string& msg) :_msg(msg) {}
    log_ex(const std::string& msg, int last_errno)
    {
        _msg = msg + ": " + errno_str(last_errno);
    }
    ~log_ex() throw()
    {}
    const char* what() const throw()
    {
        return _msg.c_str();
    }
private:
    std::string _msg;

};

} //namespace log
} //namespace kob
#endif //KOB_LOG_COMMON_H_
