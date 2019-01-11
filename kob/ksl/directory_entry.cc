//
// Created by 李寅斌 on 2018/11/20.
//

#include <kob/ksl/directory_entry.h>
#include <kob/ksl/detail/fs_base.h>

namespace ksl {
namespace filesystem {

entry_attr::entry_attr(const path_view &pv)
{
    update(pv);
}

void entry_attr::reset()
{
    size = 0;
    links = 0;
    atime = timestamp();
    mtime = timestamp();
    ctime = timestamp();
    status.set_type(file_type::none);
}
void entry_attr::update(const path_view &pv)
{
    fs_stat pst;
    int r = ::lstat(pv.c_str(), &pst);
    status = detail::build_file_status(r, pst, NULL);
}

} //namespace filesystem
} //namespace ksl