#ifndef KOB_LOG_LOG_FORMATTER_H_
#define KOB_LOG_LOG_FORMATTER_H_

#include <kob/log/detail/log_msg.h>
#include <vector>
#include <string>
#include <memory>

namespace kob {
namespace log {
class flag_formatter;


class formatter {
public:
    virtual ~formatter() {}
    virtual void format(log_msg& msg) = 0;
};


class pattern_formatter : public formatter {

public:
    explicit pattern_formatter(const std::string& pattern);
    void format(log_msg& msg);
private:
    const std::string _pattern;
    std::vector<std::unique_ptr<flag_formatter>> _formatters;
    void handle_flag(char flag);
    void compile_pattern(const std::string& pattern);
    pattern_formatter(const pattern_formatter&);
    pattern_formatter& operator=(const pattern_formatter&);
};

} //namespace log
} //namespace kob
#include <kob/log/detail/pattern_formatter_impl.h>
#endif //KOB_LOG_LOG_FORMATTER_H_



