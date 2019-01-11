#ifndef KOB_LOG_SINK_BASESINK_H_
#define KOB_LOG_SINK_BASESINK_H_

#include <kob/log/sink/sink.h>
#include <kob/log/formatter.h>
#include <kob/log/common.h>
#include <kob/log/detail/log_msg.h>
#include <kob/ksl/mutex.h>


namespace kob {
namespace log {

template<class Mutex>
class base_sink:public sink {
public:
    base_sink():_mutex() {}
    virtual ~base_sink() {};

    void log(const log_msg& msg) 
    {
        ksl::unique_guard<Mutex> lock(_mutex);
        _sink_it(msg);
    }

protected:
    virtual void _sink_it(const log_msg& msg) = 0;
    Mutex _mutex;
private:
    base_sink(const base_sink&);
    base_sink& operator=(const base_sink&);

};

} //namespace log
} //namespace kob
#endif //KOB_LOG_SINK_BASESINK_H_