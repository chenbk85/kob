#ifndef KOB_LOG_SINK_SINK_H_
#define KOB_LOG_SINK_SINK_H_

#include <kob/log/detail/log_msg.h>

namespace kob {
namespace log {

class sink {
public:
    virtual ~sink() {}
    virtual void log(const log_msg& msg) = 0;
    virtual void flush() = 0;
};

} //namespace log
} //namespace kob

#endif //KOB_LOG_SINK_SINK_H_
