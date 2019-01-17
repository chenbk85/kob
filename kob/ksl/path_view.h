//
// Created by 李寅斌 on 2018/11/15.
//

#ifndef KOB_KSL_PATH_VIEW_H_
#define KOB_KSL_PATH_VIEW_H_

#include <kob/ksl/string_view.h>
#include <string>
#include <vector>
#include <algorithm>
#include <iostream>

namespace ksl {
namespace filesystem {

template <typename Char, typename Hash = std::hash<std::basic_string<Char>>>
class basic_path_view {
public:
    typedef  Char                                  value_type;
    typedef  ksl::basic_string_view<Char>          view_type;
    typedef  std::vector<string_view>              list_type;
    typedef  std::basic_string<Char>               string_type;
    typedef  Hash                                  hash_type;
    typedef  typename string_type::const_iterator  const_iterator;
    typedef  const_iterator                        iterator;

    basic_path_view()
    :_raw(),
     _dirs(),
     _name(),
     _hash(),
     _hash_code(0),
     _absolute(false)
    {

    }

    basic_path_view(const Char* str)
            : _raw(str),
              _dirs(),
              _name(),
              _hash(),
              _hash_code(0),
              _absolute(false)
    {
        build();
    }

    basic_path_view(view_type sv)
    : _raw(sv.begin(), sv.end()),
      _dirs(),
      _name(),
      _hash(),
      _hash_code(0),
     _absolute(false)
    {
        build();
    }

    basic_path_view(std::string &&str)
    :_raw(std::move(str)),
     _dirs(),
     _name(),
     _hash(),
     _hash_code(0),
     _absolute(false)
    {
        build();
    }

    basic_path_view& assign(std::string &&str)
    {
        _raw = std::move(str);
        build();
        return *this;
    }

    basic_path_view& assign(view_type sv)
    {
        clear();
        _raw.append(sv.begin(), sv.end());
        build();
        return *this;
    }

    basic_path_view& assign(const Char *s, const Char *e)
    {
        view_type sv(s, e);

        return assign(sv);
    }

    basic_path_view& assign(const Char *s, size_t n)
    {
        view_type sv(s, n);

        return assign(sv);
    }

    basic_path_view& assign(const Char *s)
    {
        view_type sv(s);

        return assign(sv);
    }

    void clear()
    {
        _raw.clear();
        _dirs.clear();
        _name.clear();
        _hash_code = 0;
        _absolute = false;
    }
    const string_type& raw() const
    {
        return _raw;
    }

    const Char* c_str() const
    {
        return _raw.c_str();
    }

    view_type operator[](size_t index)
    {
        KOB_ASSERT_MSG(index <_dirs.size(), "over flow");
        return _dirs[index];
    }

    size_t hash() const
    {
        return _hash_code;
    }

    size_t size() const
    {
        return _dirs.size();
    }

    basic_path_view sub_path(size_t index) const
    {
        size_t limit = std::min(index, (_dirs.size() - 1));
        if (index > limit) {
            return basic_path_view(_raw);
        }

        string_type result;
        if (_absolute) {
            result.append(1, '/');
        }


        for (size_t i = 0; i < limit; i++) {
            result.append(_dirs[i].begin(), _dirs[i].size());
            result.append(1, '/');
        }
        result.append(_dirs[limit].begin(), _dirs[limit].size());
        basic_path_view p;
        return p.assign(std::move(result));
    }

    basic_path_view(const basic_path_view &) = default;
    basic_path_view&operator=(const basic_path_view &) = default;
    ~basic_path_view() = default;
private:
    bool parse();
    void do_build();
    void build()
    {
        bool np = parse();
        if(np) {
            do_build();
            _dirs.clear();
            _name.clear();
            _hash_code = 0;
            parse();
        }
        _hash_code = _hash(_raw);
    }
private:
    string_type       _raw;
    list_type         _dirs;
    view_type         _name;
    hash_type         _hash;
    size_t            _hash_code;
    bool              _absolute;
};

template <typename Char, typename Hash>
KOB_FORCE_INLINE bool basic_path_view<Char, Hash>::parse()
{

    basic_string_view<Char> rawView(_raw);
    typename basic_string_view<Char>::const_iterator bIt = rawView.begin();
    typename basic_string_view<Char>::const_iterator eIt = rawView.end();
    bool need_fix =false;
    if (bIt != eIt) {
        if (*bIt == '/') {
            _absolute = true;
            ++bIt;
        }

        typename basic_string_view<Char>::const_iterator pIt = bIt;
        while(bIt != eIt) {
            while (bIt != eIt && *bIt != '/') {
                bIt++;
            }
            basic_string_view<Char> sv(pIt, bIt);
            if (bIt != eIt) {
                if (!sv.empty()) {
                    _dirs.push_back(sv);
                } else {
                    need_fix = true;
                }
                bIt++;
                pIt = bIt;
            } else {
                _name = sv;
            }
        }
    }
    return need_fix;

}

template <typename Char, typename Hash>
KOB_FORCE_INLINE void basic_path_view<Char, Hash>::do_build()
{
   std::string fixed;
    if (_absolute) {
        fixed.append(1, '/');
    }
    for (size_t i = 0; i < _dirs.size(); i++) {
        fixed.append(_dirs[i].begin(), _dirs[i].end());
        fixed.append(1, '/');
    }
    if (!_name.empty()) {
        fixed.append(_name.begin(), _name.end());
    } else {
        size_t n = fixed.size() - 1;
        fixed.resize(n);
    }
    _raw.swap(fixed);
}

typedef basic_path_view<char> path_view;
typedef basic_path_view<wchar_t> wpath_view;

} //namespace filesystem
} //namespace ksl
#endif //KOB_KSL_PATH_VIEW_H_
