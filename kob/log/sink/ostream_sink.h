#ifndef KOB_LOG_SINK_OSTREAM_SINK_H_
#define KOB_LOG_SINK_OSTREAM_SINK_H_

#include <kob/log/sink/base_sink.h>
#include <ostream>
#include <kob/ksl/mutex.h>

namespace kob {
namespace log {
template<class Mutex>
class ostream_sink: public base_sink<Mutex> {
public:
    explicit ostream_sink(std::ostream& os, bool force_flush=false) :_ostream(os), _force_flush(force_flush) {}
    virtual ~ostream_sink() = default;

protected:
    void _sink_it(const details::log_msg& msg) 
    {
        _ostream.write(msg.formatted.data(), msg.formatted.size());
        if (_force_flush)
            _ostream.flush();
    }

    void flush() 
    {
        _ostream.flush();
    }

    std::ostream& _ostream;
    bool _force_flush;
private:
    ostream_sink(const ostream_sink&);
    ostream_sink& operator=(const ostream_sink&);
};

typedef ostream_sink<ksl::mutex>      ostream_sink_mt;
typedef ostream_sink<ksl::null_mutex> ostream_sink_st;

} //namespace log
} //namespace kob

#endif