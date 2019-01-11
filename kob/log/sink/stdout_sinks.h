#ifndef KOB_LOG_SINK_STDOUT_SINK_H_
#define KOB_LOG_SINK_STDOUT_SINK_H_

#include <kob/log/sink/base_sink.h>
#include <cstdio>
#include <memory>
#include <kob/ksl/mutex.h>

namespace kob {
namespace log {

template <class Mutex>
class stdout_sink : public base_sink<Mutex> {
    typedef stdout_sink<Mutex> MyType;
public:
    stdout_sink() {}
    static std::shared_ptr<MyType> instance()
    {
        static std::shared_ptr<MyType> instance = std::make_shared<MyType>();
        return instance;
    }

    void _sink_it(const log_msg& msg) 
    {
        fwrite(msg.formatted.data(), sizeof(char), msg.formatted.size(), stdout);
        flush();
    }

    void flush() 
    {
        fflush(stdout);
    }
};

typedef stdout_sink<ksl::null_mutex> stdout_sink_st;
typedef stdout_sink<ksl::mutex>      stdout_sink_mt;


template <class Mutex>
class stderr_sink : public base_sink<Mutex>  {
    typedef stderr_sink<Mutex> MyType;
public:
    stderr_sink() {}
    static std::shared_ptr<MyType> instance()
    {
        static std::shared_ptr<MyType> instance = std::make_shared<MyType>();
        return instance;
    }

    void _sink_it(const log_msg& msg) 
    {
        fwrite(msg.formatted.data(), sizeof(char), msg.formatted.size(), stderr);
        flush();
    }

    void flush() 
    {
        fflush(stderr);
    }
};

typedef stderr_sink<ksl::mutex>       stderr_sink_mt;
typedef stderr_sink<ksl::null_mutex>  stderr_sink_st;
}
}
#endif
