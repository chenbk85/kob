#ifndef LOG_DETAIL_OS_H_
#define LOG_DETAIL_OS_H_

#include <kob/log/common.h>
#include <kob/ksl/this_thread.h>
#include <kob/ksl/filesystem.h>
#include <cstdio>
#include <ctime>
#include <functional>
#include <string>
#include <string.h>
#include <sys/syscall.h> //Use gettid() syscall under linux to get thread id
#include <sys/stat.h>
#include <unistd.h>
#include <cstdlib>

namespace kob {
namespace log {

inline log_clock now()
{
    return ksl::timestamp();
}
inline std::tm localtime(const std::time_t &time_tt)
{
    std::tm tm;
    localtime_r(&time_tt, &tm);
    return tm;
}

inline std::tm localtime()
{
    std::time_t now_t = time(NULL);
    return localtime(now_t);
}


inline std::tm gmtime(const std::time_t &time_tt)
{

    std::tm tm;
    gmtime_r(&time_tt, &tm);
    return tm;
}

inline std::tm gmtime()
{
    std::time_t now_t = time(NULL);
    return gmtime(now_t);
}
inline bool operator==(const std::tm& tm1, const std::tm& tm2)
{
    return (tm1.tm_sec == tm2.tm_sec &&
            tm1.tm_min == tm2.tm_min &&
            tm1.tm_hour == tm2.tm_hour &&
            tm1.tm_mday == tm2.tm_mday &&
            tm1.tm_mon == tm2.tm_mon &&
            tm1.tm_year == tm2.tm_year &&
            tm1.tm_isdst == tm2.tm_isdst);
}

inline bool operator!=(const std::tm& tm1, const std::tm& tm2)
{
    return !(tm1 == tm2);
}

// eol definition
#if !defined (LOG_EOL)
#define LOG_EOL "\r\n"
#endif

static const char* eol = LOG_EOL;
static int eol_size = sizeof(LOG_EOL) - 1;

inline int fopen_s(FILE** fp, const std::string& filename, const std::string& mode)
{
    *fp = fopen((filename.c_str()), mode.c_str());
    return *fp == NULL? -1 : 0;
}

inline int remove(const filename_t &filename)
{

    return ksl::filesystem::remove(filename);
}

inline int rename(const filename_t& filename1, const filename_t& filename2)
{
    ksl::filesystem::rename(filename1, filename2);
    return 0;
}


//Return if file exists
inline bool file_exists(const filename_t& filename)
{

    return ksl::filesystem::exists(filename);
}

//Return utc offset in minutes or throw spdlog_ex on failure
inline int utc_minutes_offset(const std::tm& tm = localtime())
{
    return static_cast<int>(tm.tm_gmtoff / 60);
}

inline size_t thread_id()
{
    return ksl::this_thread::tid();
}

} 
} 


#endif