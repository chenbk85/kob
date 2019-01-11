#ifndef COMMON_LOGGING_H_
#define COMMON_LOGGING_H_

#include <kob/log/fmtlog.h>
#include <kob/ksl/singleton.h>

namespace kob {

class kob_inner_logger : public ksl::singleton<kob_inner_logger>{
public:
    kob_inner_logger()
    {
        _logger =  kob::log::stdout_logger_mt("kob-default", true);
        _logger->set_level(kob::log::log_level::ll_trace);
    }
    void set_logger(const kob::log::logger_ptr& logger)
    {
        _logger =  logger;
    }

    kob::log::logger_ptr get_logger()
    {
        return _logger;
    }

private:
    kob::log::logger_ptr _logger;
};

inline kob::log::logger_ptr get_inner_logger()
{
    kob_inner_logger *kinner = kob_inner_logger::get_instance();
    return kinner->get_logger();
}

inline void set_inner_logger(const kob::log::logger_ptr& logger)
{
    kob_inner_logger *kinner = kob_inner_logger::get_instance();
    kinner->set_logger(logger);
}

}

#define LOG_STR_H(x) #x
#define LOG_STR_HELPER(x) LOG_STR_H(x)
#define LOG_TRACE(...) kob::get_inner_logger()->trace("[" __FILE__ " line #" LOG_STR_HELPER(__LINE__) "] " __VA_ARGS__)
#define LOG_DEBUG(...) kob::get_inner_logger()->debug(__VA_ARGS__)
#define LOG_INFO(...) kob::get_inner_logger()->info(__VA_ARGS__)
#define LOG_WARN(...) kob::get_inner_logger()->warn(__VA_ARGS__)
#define LOG_ERROR(...) kob::get_inner_logger()->error(__VA_ARGS__)

#define LOG_SET_LEVEL(l)  kob::get_inner_logger()->set_level((l))

#endif