#ifndef LOG_SINK_DIST_SINK_H_
#define LOG_SINK_DIST_SINK_H_
#include <kob/log/detail/log_msg.h>
#include <kob/ksl/mutex.h>
#include <kob/log/sink/base_sink.h>
#include <kob/log/sink/sink.h>
#include <algorithm>
#include <memory>
#include <vector>

namespace kob {
namespace log {

template<class Mutex>
class dist_sink: public base_sink<Mutex> {
public:
    explicit dist_sink() :_sinks() {}
    virtual ~dist_sink() {};
public:
    void flush() 
    {
        ksl::unique_guard<Mutex> lock(base_sink<Mutex>::_mutex);
        for (auto iter = _sinks.begin(); iter != _sinks.end(); iter++) {
            (*iter)->flush();
        }
    }

    void add_sink(std::shared_ptr<sink> sink)
    {
        ksl::unique_guard<Mutex> lock(base_sink<Mutex>::_mutex);
        if (sink &&
                _sinks.end() == std::find(_sinks.begin(), _sinks.end(), sink)) {
            _sinks.push_back(sink);
        }
    }

    void remove_sink(std::shared_ptr<sink> sink)
    {
        ksl::unique_guard<Mutex> lock(base_sink<Mutex>::_mutex);
        std::vector<std::shared_ptr<sink>>::iterator pos = std::find(_sinks.begin(), _sinks.end(), sink);
        if (pos != _sinks.end()) {
            _sinks.erase(pos);
        }
    }
private:
    dist_sink(const dist_sink&);
    dist_sink& operator=(const dist_sink&);
protected:

    void _sink_it(const log_msg& msg) 
    {
        for ( std::vector<std::shared_ptr<sink>>::iterator iter = _sinks.begin(); iter != _sinks.end(); iter++) {
            (*iter)->log(msg);
        }
    }

    std::vector<std::shared_ptr<sink>> _sinks;
};

typedef dist_sink<ksl::mutex>      dist_sink_mt;
typedef dist_sink<ksl::null_mutex> dist_sink_st;
}
}
#endif