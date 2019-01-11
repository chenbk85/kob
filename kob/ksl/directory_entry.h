//
// Created by 李寅斌 on 2018/11/20.
//

#ifndef KOB_KSL_DIRECTORY_ENTRY_H_
#define KOB_KSL_DIRECTORY_ENTRY_H_

#include <kob/ksl/path_view.h>
#include <kob/ksl/timestamp.h>
#include <kob/ksl/detail/fs_base.h>
#include <cstddef>

namespace ksl {
namespace filesystem {

struct entry_attr {
    size_t            size;
    size_t            links;
    ksl::timestamp    atime;
    ksl::timestamp    mtime;
    ksl::timestamp    ctime;
    file_status       status;
    entry_attr(const path_view &pv);
    void update(const path_view &pv);
    entry_attr() = delete;
    entry_attr(const entry_attr&) = default;
    entry_attr& operator=(const entry_attr&) = default;
    void reset();
};

class directory_entry {
public:
    directory_entry(path_view pv);
private:
    path_view     _p;
};
}
}
#endif //KOB_KSL_DIRECTORY_ENTRY_H_
