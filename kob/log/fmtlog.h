#ifndef KOB_LOG_FMTLOG_H_
#define KOB_LOG_FMTLOG_H_

#include <kob/log/common.h>
#include <kob/log/logger.h>
#include <memory>
#include <functional>
#include <string>

namespace kob {
namespace log {

/*****************************************************
* the logger holder
******************************************************/ 
typedef std::shared_ptr<logger> logger_ptr;

/****************************************************************************
* @return an existing logger or nullptr if a logger with such name doesn't exist.
* Examples:
*
* kob::log::get("mylog")->info("Hello");
* kob::log::logger_ptr logger = kob::log::("mylog");
* logger.info("This is another message" , x, y, z);
* logger.info() << "This is another message" << x << y << z;
******************************************************************************/
std::shared_ptr<logger> get(const std::string& name);

/****************************************************************************
* Set global formatting
* example: kob::log::set_pattern("%Y-%m-%d %H:%M:%S.%e %l : %v");
* %v	The actual text to log	"some user text"
* %t	Thread id	"1232"
* %n	Logger's name	"some logger name"
* %l	The log level of the message	"debug", "info", etc
* %L	Short log level of the message	"D", "I", etc
* %a	Abbreviated weekday name	"Thu"
* %A	Full weekday name	"Thursday"
* %b	Abbreviated month name	"Aug"
* %B	Full month name	"August"
* %c	Date and time representation	"Thu Aug 23 15:35:46 2014"
* %C	Year in 2 digits	"14"
* %Y	Year in 4 digits	"2014"
* %D    Same as %x 
* %x	Short MM/DD/YY date	"08/23/14"
* %m	Month 1-12	"11"
* %d	Day of month 1-31	"29"
* %H	Hours in 24 format 0-23	"23"
* %I	Hours in 12 format 1-12	"11"
* %M	Minutes 0-59	"59"
* %S	Seconds 0-59	"58"
* %e	Millisecond part of the current second 0-999	"678"
* %f	Microsecond part of the current second 0-999999	"056789"
* %F	Nanosecond part of the current second 0-999999999	"256789123"
* %p	AM/PM	"AM"
* %r	12 hour clock	"02:55:02 pm"
* %R	24-hour HH:MM time, equivalent to %H:%M	"23:55"
* %T    Same as %X
* %X	ISO 8601 time format (HH:MM:SS), equivalent to %H:%M:%S	"23:55:59"
* %z	ISO 8601 offset from UTC in timezone ([+/-]HH:MM)	"+02:00"
* %%	The % sign	"%"
* %+	fmtlog's default format	"[2014-31-10 23:46:59.678] [info] [mylogger] Some message"
*****************************************************************************/
void set_pattern(const std::string& format_string);
void set_formatter(formatter_ptr f);

/**************************************************************************
* Set global logging level for
***************************************************************************/
void set_level(log_level log_level);

/*****************************************************************************
* Turn on async mode (off by default) and set the queue size for each 
*  async_logger.
* effective only for loggers created after this call.
* queue_size: size of queue (must be power of 2):
*    Each logger will pre-allocate a dedicated queue with
*    queue_size entries upon construction.
*
* async_overflow_policy (optional, block_retry by default):
*    async_overflow_policy::block_retry
*        - if queue is full, block 
*            until queue has room for the new log entry.
*    async_overflow_policy::discard_log_msg 
*        - never block and discard any new messages when queue  overflows.
*
* worker_warmup_cb (optional):
*     callback function that will be called in worker thread upon 
*     start (can be used to init stuff like thread affinity)
*
* worker_teardown_cb (optional):
*     callback function that will be called in worker thread upon exit
*******************************************************************************/
void set_async_mode(size_t queue_size, 
                    const async_overflow_policy overflow_policy = async_overflow_policy::block_retry,
                    const std::function<void()>& worker_warmup_cb = std::function<void()>(),
                    const ksl::timespan& flush_interval = 0,
                    const std::function<void()>& worker_teardown_cb = std::function<void()>());

/*************************************************************************************
* Turn off async mode
**************************************************************************************/
void set_sync_mode();


/************************************************************************************
* Create and register multi/single basic file logger
***********************************************************************************/
std::shared_ptr<logger> basic_logger_mt(const std::string& logger_name,
                                        const std::string& filename,
                                        bool force_flush = false);
std::shared_ptr<logger> basic_logger_st(const std::string& logger_name, 
                                        const std::string& filename, 
                                        bool force_flush = false);

/******************************************************************
* Create and register multi/single threaded rotating file logger
******************************************************************/
std::shared_ptr<logger> rotating_logger_mt(const std::string& logger_name, 
                                           const std::string& filename, 
                                           size_t max_file_size, 
                                           size_t max_files, 
                                           bool force_flush = false);
std::shared_ptr<logger> rotating_logger_st(const std::string& logger_name, 
                                           const std::string& filename, 
                                           size_t max_file_size, 
                                           size_t max_files, 
                                           bool force_flush = false);

/***********************************************************************************
* Create file logger which creates new file on the given time (default in  midnight):
************************************************************************************/
std::shared_ptr<logger> daily_logger_mt(const std::string& logger_name, 
                                        const std::string& filename, 
                                        int hour=0, 
                                        int minute=0, 
                                        bool force_flush = false);
std::shared_ptr<logger> daily_logger_st(const std::string& logger_name, 
                                        const std::string& filename, 
                                        int hour=0, 
                                        int minute=0, 
                                        bool force_flush = false);

/****************************************************************************************
* Create and register stdout/stderr loggers
*****************************************************************************************/
std::shared_ptr<logger> stdout_logger_mt(const std::string& logger_name, bool color = false);
std::shared_ptr<logger> stdout_logger_st(const std::string& logger_name, bool color = false);
std::shared_ptr<logger> stderr_logger_mt(const std::string& logger_name, bool color = false);
std::shared_ptr<logger> stderr_logger_st(const std::string& logger_name, bool color = false);

/*************************************************************************************
* Create and register a logger a single sink
*************************************************************************************/
std::shared_ptr<logger> create(const std::string& logger_name, const sink_ptr& sink);

/***********************************************************************************
* Create and register a logger with multiple sinks
***********************************************************************************/
std::shared_ptr<logger> create(const std::string& logger_name, sinks_init_list sinks);
template<class It>
std::shared_ptr<logger> create(const std::string& logger_name, const It& sinks_begin, const It& sinks_end);

/****************************************************************************************
* Create and register a logger with templated sink type
* Example: kob::log::create<daily_file_sink_st>("mylog", "dailylog_filename", "txt");
****************************************************************************************/
template <typename Sink, typename... Args>
std::shared_ptr<logger> create(const std::string& logger_name, Args...);

/***********************************************************************************
* Register the given logger with the given name
***********************************************************************************/
void register_logger(std::shared_ptr<logger> logger);

/***********************************************************************
* Drop the reference to the given logger
***********************************************************************/
void drop(const std::string &name);

/******************************************************************
* Drop all references
*******************************************************************/
void drop_all();


/***************************************************************************************
*
* Trace & Debug can be switched on/off at compile time for zero cost debug statements.
* Uncomment FMTLOG_DEBUG_ON/FMTLOG_TRACE_ON in teakme.h to enable.
* FMTLOG_TRACE(..) will also print current file and line.
*
* Example:
* kob::log::set_level(kob::log::ll_trace);
* FMTLOG_TRACE(my_logger, "some trace message");
* FMTLOG_TRACE(my_logger, "another trace message {} {}", 1, 2);
* FMTLOG_DEBUG(my_logger, "some debug message {} {}", 3, 4);
***************************************************************************************/

#ifdef FMTLOG_TRACE_ON
#define FMTLOG_STR_H(x) #x
#define FMTLOG_STR_HELPER(x) FMTLOG_STR_H(x)
#define FMTLOG_TRACE(logger, ...) logger->trace("[" __FILE__ " line #" FMTLOG_STR_HELPER(__LINE__) "] " __VA_ARGS__)
#else
#define FMTLOG_TRACE(logger, ...)
#endif

#ifdef FMTLOG_DEBUG_ON
#define FMTLOG_DEBUG(logger, ...) logger->debug(__VA_ARGS__)
#else
#define FMTLOG_DEBUG(logger, ...)
#endif


}
}
#endif

#include <kob/log/detail/fmtlog_impl.h>
