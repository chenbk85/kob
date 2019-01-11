//
// Created by 李寅斌 on 2018/11/14.
//

#ifndef KOB_KSL_DETAIL_FS_BASE_H_
#define KOB_KSL_DETAIL_FS_BASE_H_
#include <kob/ksl/detail/config.h>
#include <sys/stat.h>

namespace ksl {

namespace filesystem {

enum class file_type : signed {
    none = 0,
    not_found = -1,
    regular = 1,
    directory = 2,
    symlink = 3,
    block = 4,
    character = 5,
    fifo = 6,
    socket = 7,
    unknown = 8
};

enum class perms : unsigned {
    none = 0,
    owner_read = 0400,
    owner_write = 0200,
    owner_exec = 0100,
    owner_all = 0700,

    group_read = 040,
    group_write = 020,
    group_exec = 010,
    group_all = 070,

    others_read = 04,
    others_write = 02,
    others_exec = 01,
    others_all = 07,

    all = 0777,

    set_uid = 04000,
    set_gid = 02000,
    sticky_bit = 01000,
    mask = 07777,
    unknown = 0xFFFF,
};

KOB_FORCE_INLINE KOB_FUNC_CONSTEXPR

perms operator&(perms lhs, perms rhs)
{
    return static_cast<perms>(
            static_cast<unsigned>(lhs) &
            static_cast<unsigned>(rhs)
    );
}

KOB_FORCE_INLINE KOB_FUNC_CONSTEXPR

perms operator|(perms lhs, perms rhs)
{
    return static_cast<perms>(
            static_cast<unsigned>(lhs) |
            static_cast<unsigned>(rhs)
    );
}

KOB_FORCE_INLINE KOB_FUNC_CONSTEXPR

perms operator^(perms lhs, perms rhs)
{
    return static_cast<perms>(
            static_cast<unsigned>(lhs) ^
            static_cast<unsigned>(rhs)
    );
}

KOB_FORCE_INLINE KOB_FUNC_CONSTEXPR

perms operator~(perms lhs)
{
    return static_cast<perms>(
            ~static_cast<unsigned>(lhs)
    );
}

KOB_FORCE_INLINE KOB_FUNC_CONSTEXPR

perms operator&=(perms lhs, perms rhs)
        {
    lhs = lhs & rhs;
    return lhs;
}

KOB_FORCE_INLINE KOB_FUNC_CONSTEXPR

perms& operator|=(perms &lhs, perms rhs)
{
    lhs = lhs | rhs;
    return lhs;
}

KOB_FORCE_INLINE KOB_FUNC_CONSTEXPR

perms& operator^=(perms &lhs, perms rhs)
{
    lhs = lhs ^ rhs;
    return lhs;
}


static const perms kDefaultFilePerm = static_cast<perms>(0644);

static const perms kDefaultAppPerm = static_cast<perms>(0755);

class file_status {
public:
    file_status(file_type ft = file_type::none)
            : _ft(ft)
    {

    }

    file_status(const file_status&) = default;
    file_status& operator=(const file_status&) = default;

    file_status(file_type ft, perms pm)
            : _ft(ft),
              _pm(pm)
    {

    }

    file_type type() const KOB_NOEXCEPT
    {
        return _ft;
    }

    perms perm() const KOB_NOEXCEPT
    {
        return _pm;
    }

    void set_type(file_type t)
    {
        _ft = t;
    }

    void set_perm(perms pm)
    {
        _pm = pm;
    }
private:
    file_type        _ft;
    perms            _pm;
};

typedef struct ::stat fs_stat;

namespace detail {


file_status build_file_status(int ec, const fs_stat &fst, int *err);

perms posix_get_perms(const fs_stat &fst);

} //namespace detail

} //namespace filesystem
} //namespace ksl
#endif //KOB_KSL_DETAIL_FS_BASE_H_
