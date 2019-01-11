//
// Created by 李寅斌 on 2018/11/15.
//

#ifndef KOB_KSL_PATH_VIEW_H_
#define KOB_KSL_PATH_VIEW_H_

#include <kob/ksl/string_view.h>
#include <string>
#include <vector>
#include <algorithm>

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

    basic_path_view(view_type sv) : _raw(sv.begin(), sv.end())
    {
        _need_rebuild = false;
        _absolute = false;
        parse();
    }

    const string_type& raw() const
    {
        return _raw;
    }

    const Char* c_str() const
    {
        if(_need_rebuild) {
            return _fixed.c_str();
        }
        return _raw.c_str();
    }

    view_type operator[](size_t index)
    {
        KOB_ASSERT_MSG(index <_dirs.size(), "over flow");
        return _dirs[index];
    }

    void hash() const
    {
        return _hash_code;
    }

    size_t size() const
    {
        return _dirs.size();
    }

    string_type build()
    {
        if (!_need_rebuild) {
            return _raw;
        }
        return _fixed;
    }

    string_type sub_path(size_t index) const
    {
        size_t limit = std::min(index, (_dirs.size() - 1));
        if (index > limit) {
            return _need_rebuild ? _fixed : _raw;
        }

        string_type result;
        if (_absolute) {
            result.append(1, '/');
        }


        for (size_t i = 0; i < limit; i++) {
            result.append(_dirs[i].begin(), _dirs[i].end());
            result.append(1, '/');
        }
        result.append(_dirs[limit].begin(), _dirs[limit].end());
        return result;
    }

    basic_path_view(const basic_path_view &) = default;
    basic_path_view&operator=(const basic_path_view &) = default;
    ~basic_path_view() = default;
    basic_path_view() = delete;

private:
    void parse();

    void do_build()
    {
        _fixed.clear();
        if (_absolute) {
            _fixed.append(1, '/');
        }
        for (size_t i = 0; i < _dirs.size(); i++) {
            _fixed.append(_dirs[i].begin(), _dirs[i].end());
            _fixed.append(1, '/');
        }
        if (!_name.empty()) {
            _fixed.append(_name.begin(), _name.end());
        } else {
            size_t n = _fixed.size() - 1;
            _fixed.resize(n);
        }
    }
private:
    const string_type _raw;
    string_type       _fixed;
    list_type         _dirs;
    view_type         _name;
    hash_type         _hash;
    size_t            _hash_code;
    bool              _absolute;
    bool              _need_rebuild;
};

template <typename Char, typename Hash>
void basic_path_view<Char, Hash>::parse()
{
    basic_string_view<Char> rawView(_raw);
    typename basic_string_view<Char>::const_iterator bIt = rawView.begin();
    typename basic_string_view<Char>::const_iterator eIt = rawView.end();

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
                    _need_rebuild = true;
                }
                bIt++;
                pIt = bIt;
            } else {
                _name = sv;
            }
        }
        if(_need_rebuild) {
            do_build();
        }
        _hash_code = _hash(_fixed);
    }

}

typedef basic_path_view<char> path_view;
typedef basic_path_view<wchar_t> wpath_view;

} //namespace filesystem
} //namespace ksl
#endif //KOB_KSL_PATH_VIEW_H_
