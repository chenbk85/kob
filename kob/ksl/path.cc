#include <kob/ksl/path.h>
#include <kob/ksl/env.h>
#include <algorithm>
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>


namespace ksl {

namespace filesystem {

path::path()
: _absolute(false)
{
}


path::path(path_type pt)
: _absolute(pt == path_type::pt_absolute ? true : false)
{
	if(_absolute) {
		_absolute = false;
		make_absolute();
	}
}

path::path(ksl::string_view pStr)
{
	assign(pStr);
}



path::path(const path &rhs)
: _name(rhs._name),
  _dirs(rhs._dirs),
  _absolute(rhs._absolute)
{

}


path::path(const path &pathParent, ksl::string_view fileName)
: _name(pathParent._name),
  _dirs(pathParent._dirs),
  _absolute(pathParent._absolute)
{
	make_directory();
	_name.clear();
	_name.append(fileName.begin(), fileName.end());
}



path::path(const path &pathParent, const path &relative)
: _name(pathParent._name),
  _dirs(pathParent._dirs),
  _absolute(pathParent._absolute)
{
	resolve(relative);
}


path::~path()
{
}


path &path::operator=(const path &rhs)
{
	return assign(rhs);
}


path &path::operator=(ksl::string_view pStr)
{
	return assign(pStr);
}


void path::swap(path &rhs)
{
	std::swap(_name, rhs._name);
	std::swap(_dirs, rhs._dirs);
	std::swap(_absolute, rhs._absolute);
}


path &path::assign(const path &rhs)
{
	if (&rhs != this) {
		_name = rhs._name;
		_dirs = rhs._dirs;
		_absolute = rhs._absolute;
	}
		return *this;
}


path &path::assign(ksl::string_view pStr) {

	parse(pStr);
	return *this;
}

std::string path::to_string() const
{
		return build();
}




path &path::parse_directory(ksl::string_view pStr) {
	assign(pStr);
	return make_directory();
}


path &path::make_directory()
{
	push_directory(_name);
	_name.clear();
	return *this;
}


path &path::make_file()
{
	if (!_dirs.empty() && _name.empty()) {
		_name = _dirs.back();
		_dirs.pop_back();

	}
	return *this;
}


path &path::make_absolute()
{
	if(!_absolute) {
		return make_absolute(current());
	}
	return *this;
}

path& path::make_absolute(ksl::string_view base)
{
	path tmp(base);
	make_absolute(tmp);
	return *this;
}

path &path::make_absolute(const path &base)
{
	if (!_absolute) {
		path tmp = base;
		tmp.make_directory();
		for (string_vec::const_iterator it = _dirs.begin(); it != _dirs.end(); ++it) {
			tmp.push_directory(*it);
		}
		_dirs = tmp._dirs;
		_absolute = base._absolute;
	}
	return *this;
}


path path::absolute() const
{
	path result(*this);
	if (!result._absolute) {
		result.make_absolute();
	}
	return result;
}


path path::absolute(const path &base) const
{
	path result(*this);
	if (!result._absolute) {
		result.make_absolute(base);
	}
	return result;
}


path path::parent() const
{
	path p(*this);
	return p.make_parent();
}


path &path::make_parent() {
	if (_name.empty()) {
		if (_dirs.empty()) {
			if (!_absolute)
				_dirs.push_back("..");
		} else {
			if (_dirs.back() == "..")
				_dirs.push_back("..");
			else
				_dirs.pop_back();
		}
	} else {
		_name.clear();
	}
	return *this;
}


path &path::append(const path &tail)
{
	make_directory();
	_dirs.insert(_dirs.end(), tail._dirs.begin(), tail._dirs.end());
	_name = tail._name;
	return *this;
}


path &path::resolve(const path &uri)
{
	if (uri.is_absolute()) {
		assign(uri);
	} else {
		for (int i = 0; i < uri.depth(); ++i)
			push_directory(uri[i]);
		_name = uri._name;
	}
	return *this;
}



const std::string &path::directory(size_t n) const
{
	assert(n <= _dirs.size());

	if (n < _dirs.size()) {
		return _dirs[n];
	} else {
		return _name;
	}
}


const std::string &path::operator[](size_t n) const
{
	assert(n <= _dirs.size());

	if (n < _dirs.size()) {
		return _dirs[n];
	} else {
		return _name;
	}
}


path &path::push_directory(ksl::string_view dir)
{
	if (!dir.empty() && dir != ".") {
		if (dir == "..") {
			if (!_dirs.empty() && _dirs.back() != "..") {
				_dirs.pop_back();
			} else if (!_absolute) {
				_dirs.push_back(std::string(dir.begin(), dir.end()));
			}
		} else {
			_dirs.push_back(std::string(dir.begin(), dir.end()));
		}
	}
	return *this;
}


path &path::pop_directory()
{
	assert(!_dirs.empty());

	_dirs.pop_back();
	return *this;
}


path &path::pop_front_directory()
{
	assert(!_dirs.empty());

	string_vec::iterator it = _dirs.begin();
	_dirs.erase(it);
	return *this;
}


path &path::set_file_name(ksl::string_view name)
{
	_name.assign(name.begin(), name.end());
	return *this;
}


path &path::set_base_name(ksl::string_view name)
{
	std::string ext = get_extension();
	_name.clear();
	_name.append(name.begin(), name.end());
	if (!ext.empty()) {
		_name.append(".");
		_name.append(ext);
	}
	return *this;
}


std::string path::get_base_name() const
{
	std::string::size_type pos = _name.rfind('.');
	if (pos != std::string::npos)
		return _name.substr(0, pos);
	else
		return _name;
}


path &path::set_extension(ksl::string_view extension)
{
	_name = get_base_name();
	if (!extension.empty()) {
		_name.append(".");
		_name.append(extension.begin(), extension.end());
	}
	return *this;
}


std::string path::get_extension() const
{
	std::string::size_type pos = _name.rfind('.');
	if (pos != std::string::npos)
		return _name.substr(pos + 1);
	else
		return std::string();
}


path &path::clear()
{
	_name.clear();
	_dirs.clear();
	_absolute = false;
	return *this;
}


std::string path::current()
{
	std::string path;
	char cwd[kMaxPathLen];
	if (getcwd(cwd, sizeof(cwd))) {
		path = cwd;
	} else {
		path = "";
	}
	std::string::size_type n = path.size();
	if (n > 0 && path[n - 1] != '/') path.append("/");
	return path;
}


std::string path::home()
{
	std::string path;
	struct passwd *pwd = getpwuid(getuid());
	if (pwd) {
		path = pwd->pw_dir;
	} else {
		pwd = getpwuid(geteuid());
		if (pwd)
			path = pwd->pw_dir;
		else
			path = ksl::env::get("HOME");
	}
	std::string::size_type n = path.size();
	if (n > 0 && path[n - 1] != '/') path.append("/");
	return path;
}


std::string path::temp()
{
	std::string result;
	std::string tmp = ksl::env::get("TMPDIR");
	if (tmp.size() > 0 ) {
		result = tmp;
		std::string::size_type n = result.size();
		if (n > 0 && result[n - 1] != '/') result.append("/");
	} else {
		result = "/tmp/";
	}
	return result;
}


std::string path::null()
{
	return "/dev/null";
}


std::string path::expand(ksl::string_view pStr)
{
	std::string result;
	ksl::string_view::const_iterator it = pStr.begin();
	ksl::string_view::const_iterator end = pStr.end();
	if (it != end && *it == '~') {
		++it;
		if (it != end && *it == '/') {
			result += home();
			++it;
		} else {
			result += '~';
		}
	}
	while (it != end) {
		if (*it == '$') {
			std::string var;
			++it;
			if (it != end && *it == '{') {
				++it;
				while (it != end && *it != '}') var += *it++;
				if (it != end) ++it;
			} else {
				while (it != end && (ksl::ascii::is_alpha_numeric(*it) || *it == '_')) var += *it++;
			}
			char *val = getenv(var.c_str());
			if (val) {
				result += val;
			}
		} else {
			result += *it++;
		}
	}
	return result;
}



bool path::parse(ksl::string_view pStr)
{
	clear();

	ksl::string_view::const_iterator it = pStr.begin();
	ksl::string_view::const_iterator end = pStr.end();
	//if path is empty, every thing is empty
	if (it != end) {
		if (*it == '/') {
			_absolute = true;
			++it;
		} else if (*it == '~') {
			++it;
			if (it == end || *it == '/') {
				path cwd(home());
				_dirs = cwd._dirs;
				_absolute = true;
			} else {
				--it;
			}
		}

		while (it != end) {
			std::string name;
			while (it != end && *it != '/') {
				name += *it++;
			}
			if (it != end) {
				if (_dirs.empty()) {
					if (!name.empty() && *(name.rbegin()) == ':') {
						_absolute = true;
					} else {
						push_directory(name);
					}
				} else {
					push_directory(name);
				}
			} else {
				_name = name;
			}
			if (it != end) {
				++it;
			}
		}
	}
	return true;
}



std::string path::build() const
{
	std::string result;
	if (_absolute) {
		result.append("/");
	}

	for (string_vec::const_iterator it = _dirs.begin(); it != _dirs.end(); ++it) {
		result.append(*it);
		result.append("/");
	}
	result.append(_name);
	return result;
}


} //namespace filesystem
} // namespace ksl
