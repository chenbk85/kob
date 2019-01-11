#ifndef COMMON_PATH_H_
#define COMMON_PATH_H_
#include <kob/ksl/path_view.h>
#include <kob/ksl/string_view.h>
#include <string>
#include <vector>
#include <cstddef>

namespace ksl {
namespace filesystem {

enum class path_type {
	pt_relative,
	pt_absolute
};

class  path{
public:
	static const size_t kMaxPathLen = 1024;
	typedef std::vector<std::string> string_vec;

	path();

	path(path_type pt);
		

	path(ksl::string_view pStr);


	path(const path& path);

	path(const path& parent, ksl::string_view fileName);

	path(const path& parent, const path& relative);


	~path();

	path& operator = (const path& path);

	path& operator = (ksl::string_view pStr);

	void swap(path& path);	


	path& assign(ksl::string_view pStr);
		

	path& assign(const path& path);


	std::string to_string() const;

	void to_string(std::string &ret) const;

	bool parse(ksl::string_view sv);


	path& parse_directory(ksl::string_view pStr);
	

	path& make_directory();


	path& make_file();


	path& make_parent();


	path& make_absolute();
	

	path& make_absolute(const path& base);

	path& make_absolute(ksl::string_view base);

	path& append(const path& path);
		

	path& resolve(const path& path);


	bool is_absolute() const;

	/*!	
	*@eturn true iff the path is relative.
	*/
	bool is_relative() const;
		

	bool is_directory() const;
	

	bool is_file() const;


	int depth() const;

	const std::string& directory(size_t n) const;

	const std::string& operator [] (size_t n) const;
	

	path& push_directory(ksl::string_view dir);


	path& pop_directory();
	

	path& pop_front_directory();
	

	path& set_file_name(ksl::string_view name);


	const std::string& get_file_name() const;
		

	path& set_base_name(ksl::string_view name);


	std::string get_base_name() const;


	path& set_extension(ksl::string_view extension);


	std::string get_extension() const;
	

	path& clear();


	path parent() const;


	path absolute() const;
	

	path absolute(const path& base) const;


	static path for_directory(ksl::string_view pStr);
		
	/*!	
	* @return '/' on unix like system.
	*/
	static char separator();

	/*!	
	* @return ':' on unix like system.
	*/
	static char path_separator();

	/*!	
	* @return the current working directory.
	*/
	static std::string current();

	/*!	
	* @return the user's home directory.
	*/
	static std::string home();
		
	/*!	
	* @return the temporary directory.
	*/
	static std::string temp();

	/*!	
	* @return the name of the null device.
	*/	
	static std::string null();

	/*!	
	* @return Expands all environment variables contained in the path.
	* On Unix, a tilde as first character in the path is
	* replaced with the path to user's home directory.
	*/		
	static std::string expand(ksl::string_view path);


protected:
	std::string build() const;


private:
	std::string   _name;
	string_vec    _dirs;
	bool          _absolute;
};


//
// inlines
//
inline bool path::is_absolute() const
{
	return _absolute;
}

	
inline bool path::is_relative() const
{
	return !_absolute;
}


inline bool path::is_directory() const
{
	return _name.empty();
}


inline bool path::is_file() const
{
	return !_name.empty();
}







inline const std::string& path::get_file_name() const
{
	return _name;
}


inline int path::depth() const
{
	return int(_dirs.size());
}


inline path path::for_directory(ksl::string_view pStr)
{
	path p;
	return p.parse_directory(pStr);
}
	

inline char path::separator()
{
	return '/';
}


inline char path::path_separator()
{
	return ':';
}


inline void swap(path& p1, path& p2)
{
	p1.swap(p2);
}


}  //namespace filesystem
} //namespace ksl

#endif 
