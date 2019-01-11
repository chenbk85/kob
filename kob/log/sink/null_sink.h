#ifndef LOG_SINK_NULL_SINK_H_
#define LOG_SINK_NULL_SINK_H_

#include <fermat/log/sink/base_sink.h>
#include <fermat/common/mutex.h>

#include <mutex>

namespace kob {
namespace log {

template <class Mutex>
class null_sink : public base_sink <Mutex> {
protected:
    void _sink_it(const log_msg&) 
    {

    }

    void flush() 
    {

    }

};
typedef null_sink<ksl::null_mutex>     null_sink_st;
typedef null_sink<ksl::null_mutex>     null_sink_mt;

}
}

#endif
