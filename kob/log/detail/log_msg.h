#ifndef LOG_DETAIL_LOG_MSG_H_
#define LOG_DETAIL_LOG_MSG_H_

#include <kob/log/common.h>
#include <kob/log/detail/format.h>
#include <kob/log/detail/os.h>
#include <string>
#include <utility>

namespace kob {
namespace log {


struct log_msg {
    log_msg() = default;
    log_msg(const std::string *loggers_name, log_level lvl) \
    : logger_name(loggers_name),
      level(lvl)
    {
        time = now();
        thread_id = kob::log::thread_id();
    }


    const std::string       *logger_name;
    log_level                level;
    log_clock                time;
    size_t                   thread_id;
    fermat::MemoryWriter     raw;
    fermat::MemoryWriter     formatted;
private:
    log_msg(const log_msg& other);
    log_msg& operator=(log_msg&& other);
    log_msg(log_msg&& other);

};


}
}
#endif