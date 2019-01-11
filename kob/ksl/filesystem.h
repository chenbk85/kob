//
// Created by 李寅斌 on 2018/11/15.
//

#ifndef KOB_KSL_FILESYSTEM_H_
#define KOB_KSL_FILESYSTEM_H_

#include <kob/ksl/detail/fs_base.h>
#include <kob/ksl/path_view.h>
#include <kob/ksl/path.h>
#include <kob/ksl/path_view.h>
#include <kob/ksl/timestamp.h>

namespace ksl {
namespace  filesystem {


file_status status(const path_view &filePath);

file_status status(const path_view &filePath, int &ec);


file_status symlink_status(const path_view &filePath);

file_status symlink_status(const path_view &filePath, int &ec);

bool exists(file_status fst);

bool exists(const path_view &filePath);

bool exists(const path_view &filePath, int &ec);

void rename(const path_view &ol, const path_view &ne);

void rename(const path_view &ol, const path_view &ne, int &ec);

bool remove(const path_view &p);

bool remove(const path_view &p, int &ec);

bool remove_all(const path_view &p);

bool remove_all(const path_view &p, int &ec);

bool create_directory(const path_view &p , perms pm = kDefaultFilePerm);

bool create_directory(const path_view &p, int &ec, perms pm = kDefaultFilePerm);

bool create_directories(const path_view &p, perms pm = kDefaultFilePerm);

bool create_directories(const path_view &p, int &ec , perms pm = kDefaultFilePerm);

} //namespace filesystem
} //namespace ksl
#endif //KOB_KSL_FILESYSTEM_H_
