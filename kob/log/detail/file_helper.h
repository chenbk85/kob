#ifndef LOG_DETAIL_FILEHELPER_H_
#define LOG_DETAIL_FILEHELPER_H_

#include <kob/log/detail/os.h>
#include <kob/log/detail/log_msg.h>
#include <kob/ksl/this_thread.h>
#include <cstdio>
#include <string>
#include <thread>
#include <cerrno>

namespace kob {
namespace log {

class file_helper {

public:
    static const int open_tries = 5;
    static const int open_interval = 10;

    explicit file_helper(bool force_flush) :
        _fd(0),
        _force_flush(force_flush)
    {

    }


    ~file_helper()
    {
        close();
    }


    void open(const std::string& fname, bool truncate = false)
    {

        close();
        const char *mode = truncate ? "wb" : "ab";
        _filename = fname;
        for (int tries = 0; tries < open_tries; ++tries) {
            if (!fopen_s(&_fd, _filename.c_str(), mode))
                return;
            ksl::this_thread::sleep_for(open_interval);
        }

        throw log_ex("Failed opening file " + _filename + " for writing", errno);
    }

    void reopen(bool truncate)
    {
        if (_filename.empty())
            throw log_ex("Failed re opening file - was not opened before");
        open(_filename, truncate);

    }

    void flush()
    {
        std::fflush(_fd);
    }

    void close()
    {
        if (_fd) {
            ::fclose(_fd);
            _fd = NULL;
        }
    }

    void write(const log_msg& msg)
    {

        size_t msg_size = msg.formatted.size();
        const char* data = msg.formatted.data();
        if (::fwrite(data, 1, msg_size, _fd) != msg_size)
            throw log_ex("Failed writing to file " + _filename, errno);

        if (_force_flush)
            ::fflush(_fd);
    }

    long size()
    {
        if (!_fd)
            throw log_ex("Cannot use size() on closed file " + _filename);

        auto pos = ftell(_fd);
        if (fseek(_fd, 0, SEEK_END) != 0)
            throw log_ex("fseek failed on file " + _filename, errno);

        auto file_size = ftell(_fd);

        if(fseek(_fd, pos, SEEK_SET) !=0)
            throw log_ex("fseek failed on file " + _filename, errno);

        if (file_size == -1)
            throw log_ex("ftell failed on file " + _filename, errno);

        return file_size;
    }

    const std::string& filename() const
    {
        return _filename;
    }

    static bool file_exists(const std::string& name)
    {

        return file_exists(name);
    }
private:

    file_helper(const file_helper&) ;
    file_helper& operator=(const file_helper&) ;
private:
    FILE*         _fd;
    std::string   _filename;
    bool          _force_flush;
};

}
}
#endif
