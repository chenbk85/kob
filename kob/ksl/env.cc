#include <kob/ksl/env.h>
#include <string>
#include <map>
#include <cstring>
#include <unistd.h>
#include <stdlib.h>
#include <sys/utsname.h>
#include <sys/param.h>
#include <pthread.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <net/if.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <net/if.h>
#include <net/if_arp.h>
#include <unistd.h>

namespace ksl {


std::map<std::string, std::string> _map;


std::string env::get(const std::string& name)
{
	
	const char* val = getenv(name.c_str());
	if (val)
		return std::string(val);
	else
		return "";
}


bool env::has(const std::string& name)
{
	return getenv(name.c_str()) != 0;
}


bool env::set(const std::string& name, const std::string& value)
{
	
	std::string var = name;
	var.append("=");
	var.append(value);
	_map[name] = var;
	if (putenv((char*) _map[name].c_str())) {
		return false;
	}
	return true;
}


std::string env::os_name()
{
	struct utsname uts;
	uname(&uts);
	return uts.sysname;
}


std::string env::os_version()
{
	struct utsname uts;
	uname(&uts);
	return uts.release;
}


std::string env::os_architecture()
{
	struct utsname uts;
	uname(&uts);
	return uts.machine;
}


std::string env::node_name()
{
	struct utsname uts;
	uname(&uts);
	return uts.nodename;
}

int env::processor_count()
{
	int count = sysconf(_SC_NPROCESSORS_ONLN);
	if (count <= 0) { 
		count = 1;
	}
	return static_cast<int>(count);
}
} 

