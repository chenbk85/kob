#ifndef LOG_SINK_FILE_SINK_H_
#define LOG_SINK_FILE_SINK_H_

#include <kob/log/sink/base_sink.h>
#include <kob/ksl/mutex.h>
#include <kob/log/detail/file_helper.h>
#include <kob/log/detail/format.h>
#include <algorithm>
#include <cstdio>
#include <ctime>
#include <string>
#include <cerrno>

namespace kob {
namespace log {
/*
* Trivial file sink with single file as target
*/
template<class Mutex>
class simple_file_sink : public base_sink < Mutex > {
public:
    explicit simple_file_sink(const std::string &filename,
                              bool force_flush = false) :
        _file_helper(force_flush)
    {
        _file_helper.open(filename);
    }
    void flush() 
    {
        _file_helper.flush();
    }

protected:
    void _sink_it(const log_msg& msg) 
    {
        _file_helper.write(msg);
    }
private:
    file_helper _file_helper;
};

typedef simple_file_sink<ksl::mutex> simple_file_sink_mt;
typedef simple_file_sink<ksl::null_mutex> simple_file_sink_st;

/*
* Rotating file sink based on size
*/
template<class Mutex>
class rotating_file_sink : public base_sink < Mutex > {
public:
    rotating_file_sink(const std::string &base_filename, const std::string &extension,
                       std::size_t max_size, std::size_t max_files,
                       bool force_flush = false) :
        _base_filename(base_filename),
        _extension(extension),
        _max_size(max_size),
        _max_files(max_files),
        _current_size(0),
        _file_helper(force_flush)
    {
        _file_helper.open(calc_filename(_base_filename, 0, _extension));
        _current_size = _file_helper.size(); //expensive. called only once
    }

    void flush() 
    {
        _file_helper.flush();
    }

protected:
    void _sink_it(const log_msg& msg) 
    {
        _current_size += msg.formatted.size();
        if (_current_size > _max_size) {
            _rotate();
            _current_size = msg.formatted.size();
        }
        _file_helper.write(msg);
    }

private:
    static std::string calc_filename(const std::string& filename, std::size_t index, const std::string& extension)
    {
        fermat::MemoryWriter w;
        if (index)
            w.write("{}.{}.{}", filename, index, extension);
        else
            w.write("{}.{}", filename, extension);
        return w.str();
    }

    // Rotate files:
    // log.txt -> log.1.txt
    // log.1.txt -> log2.txt
    // log.2.txt -> log3.txt
    // log.3.txt -> delete

    void _rotate()
    {
        _file_helper.close();
        for (std::size_t i = _max_files; i > 0; --i) {
            std::string src = calc_filename(_base_filename, i - 1, _extension);
            std::string target = calc_filename(_base_filename, i, _extension);

            ksl::filesystem::path_view pt(target);
            ksl::filesystem::path_view ps(target);
            if (file_helper::file_exists(target)) {
                ksl::filesystem::remove(pt);
            }
            if (ksl::filesystem::exists(ps)){
                ksl::filesystem::rename(ps, pt);
            }
        }
        _file_helper.reopen(true);
    }
    std::string            _base_filename;
    std::string            _extension;
    std::size_t           _max_size;
    std::size_t           _max_files;
    std::size_t           _current_size;
    file_helper           _file_helper;
};

typedef rotating_file_sink<ksl::mutex> rotating_file_sink_mt;
typedef rotating_file_sink<ksl::null_mutex>rotating_file_sink_st;

/*
* Default generator of daily log file names.
*/
struct default_daily_file_name_calculator
{
    //Create filename for the form basename.YYYY-MM-DD_hh-mm.extension
    static std::string calc_filename(const std::string& basename, const std::string& extension)
    {
        std::tm tm = localtime();
        fermat::MemoryWriter w;
        w.write("{}_{:04d}-{:02d}-{:02d}_{:02d}-{:02d}.{}", basename, tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, extension);
        return w.str();
    }
};

/*
* Generator of daily log file names in format basename.YYYY-MM-DD.extension
*/
struct dateonly_daily_file_name_calculator
{
    //Create filename for the form basename.YYYY-MM-DD.extension
    static std::string calc_filename(const std::string& basename, const std::string& extension)
    {
        std::tm tm = localtime();
        fermat::MemoryWriter w;
        w.write("{}_{:04d}-{:02d}-{:02d}.{}", basename, tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, extension);
        return w.str();
    }
};

/*
* Rotating file sink based on date. rotates at midnight
*/
template<class Mutex, class FileNameCalc = default_daily_file_name_calculator>
class daily_file_sink :public base_sink < Mutex > {
public:
    //create daily file sink which rotates on given time
    daily_file_sink(
        const std::string& base_filename,
        const std::string& extension,
        int rotation_hour,
        int rotation_minute,
        bool force_flush = false)
         : _base_filename(base_filename),
           _extension(extension),
           _rotation_h(rotation_hour),
           _rotation_m(rotation_minute),
           _file_helper(force_flush)
    {
        if (rotation_hour < 0 || rotation_hour > 23 || rotation_minute < 0 || rotation_minute > 59)
            throw log_ex("daily_file_sink: Invalid rotation time in ctor");
        _rotation_tp = _next_rotation_tp();
        _file_helper.open(FileNameCalc::calc_filename(_base_filename, _extension));
    }

    void flush() 
    {
        _file_helper.flush();
    }

protected:
    void _sink_it(const log_msg& msg) 
    {
        if (ksl::timestamp() >= _rotation_tp)
        {
            _file_helper.open(FileNameCalc::calc_filename(_base_filename, _extension));
            _rotation_tp = _next_rotation_tp();
        }
        _file_helper.write(msg);
    }

private:
   ksl::timestamp _next_rotation_tp()
    {        
        ksl::timestamp now;
        time_t tnow = now.epoch_time();
        tm date = localtime(tnow);
        date.tm_hour = _rotation_h;
        date.tm_min = _rotation_m;
        date.tm_sec = 0;
        ksl::timestamp rotation_time = ksl::timestamp::from_epoch_time(std::mktime(&date));
        if (rotation_time > now)
            return rotation_time;
        else
            return rotation_time + 24 * 60 * 60 * ksl::timestamp::ratio();
    }
private:
    std::string         _base_filename;
    std::string         _extension;
    int                _rotation_h;
    int                _rotation_m;
    ksl::timestamp  _rotation_tp;
    file_helper        _file_helper;
};

typedef daily_file_sink<ksl::mutex>      daily_file_sink_mt;
typedef daily_file_sink<ksl::null_mutex>  daily_file_sink_st;
}
}

#endif