//
// Created by 李寅斌 on 2018/11/19.
//

#include <kob/ksl/filesystem.h>
#include <kob/ksl/detail/fs_base.h>
#include <errno.h>

namespace ksl {
namespace filesystem {

namespace detail {

perms posix_get_perms(const fs_stat &fst)
{
    return static_cast<perms>(fst.st_mode) & perms::mask;
}

file_status build_file_status(int ec, const fs_stat &fst, int *err)
{
    if (err) {
        *err = errno;
    }
    if (ec != 0) {
        if (errno == ENOENT || errno == ENOTDIR) {
            return file_status(file_type::not_found);
        }
        return file_status(file_type::none);
    }

    file_status fs_tmp;
    auto const mode = fst.st_mode;
    if (S_ISLNK(mode))
        fs_tmp.set_type(file_type::symlink);
    else if (S_ISREG(mode))
        fs_tmp.set_type(file_type::regular);
    else if (S_ISDIR(mode))
        fs_tmp.set_type(file_type::directory);
    else if (S_ISBLK(mode))
        fs_tmp.set_type(file_type::block);
    else if (S_ISCHR(mode))
        fs_tmp.set_type(file_type::character);
    else if (S_ISFIFO(mode))
        fs_tmp.set_type(file_type::fifo);
    else if (S_ISSOCK(mode))
        fs_tmp.set_type(file_type::socket);
    else
        fs_tmp.set_type(file_type::unknown);

    fs_tmp.set_perm(posix_get_perms(fst));
    return fs_tmp;
}

bool do_remove(const path_view &p, int *ec)
{
    if (::remove(p.c_str()) == -1) {
        if (errno != ENOENT) {
            if (ec) {
                *ec = errno;
            }
        }
        return false;
    }
    return true;
}

} //namespaec detail

file_status status(const path_view &p)
{
    fs_stat pst;
    int ec = ::stat(p.c_str(), &pst);
    return detail::build_file_status(ec, pst, NULL);
}

file_status status(const path_view &p, int &ec)
{
    fs_stat pst;
    int r = ::stat(p.c_str(), &pst);
    return detail::build_file_status(r, pst, &ec);
}


file_status symlink_status(const path_view &p)
{
    fs_stat pst;
    int r = ::lstat(p.c_str(), &pst);
    return detail::build_file_status(r, pst, NULL);
}

file_status symlink_status(const path_view &p, int &ec)
{
    fs_stat pst;
    int r = ::lstat(p.c_str(), &pst);
    return detail::build_file_status(r, pst, &ec);
}



bool exists(file_status fst)
{
    return fst.type() != file_type::not_found && fst.type() != file_type::none;
}

bool exists(const path_view &filePath)
{
    return exists(status(filePath));
}

bool exists(const path_view &filePath, int &ec)
{
    file_status tmp = status(filePath, ec);
    if (tmp.type() != file_type::none) {
        ec = 0;
    }
    return exists(tmp);
}

void rename(const path_view &ol, const path_view &ne)
{
    ::rename(ol.c_str(), ne.c_str());
}

void rename(const path_view &ol, const path_view &ne, int &ec)
{
    ec = 0;
    int r = ::rename(ol.c_str(), ne.c_str());
    if (r != 0) {
        ec = errno;
    }
}

bool remove(const path_view &p)
{
    return detail::do_remove(p, NULL);
}

bool remove(const path_view &p, int &ec)
{
    return detail::do_remove(p, &ec);
}

bool create_directory(const path_view &p, perms pm)
{
    if (::mkdir(p.c_str(), static_cast<int>(pm)) == 0) {
        return true;
    }
    return false;
}

bool create_directory(const path_view &p, int &ec, perms pm)
{
    if (::mkdir(p.c_str(), static_cast<int>(pm)) == 0) {
        return true;
    }
    ec = errno;
    return false;
}


bool create_directories(const path_view &p, perms pm)
{
    for(size_t i = 0; i <= p.size(); i++) {
        std::string d = p.sub_path(i);
        if (::mkdir(d.c_str(), static_cast<int>(pm)) != 0) {
            if (errno != EEXIST) {
                return false;
            }
        }
    }
    return true;
}

bool create_directories(const path_view &p, int &ec, perms pm)
{
    for(size_t i = 0; i <= p.size(); i++) {
        std::string d = p.sub_path(i);
        if (::mkdir(d.c_str(), static_cast<int>(pm)) != 0) {
            if (errno != EEXIST) {
                ec = errno;
                return false;
            }
        }
    }
    return true;
}

} //namespace filesystem
} //namespace ksl