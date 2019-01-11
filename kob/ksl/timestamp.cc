#include <kob/ksl/timestamp.h>
#include <kob/ksl/timespan.h>
#include <algorithm>
#undef min
#undef max
#include <limits>
#include <time.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/times.h>


namespace ksl {


const timestamp::time_val timestamp::kTimeValMin = std::numeric_limits<timestamp::time_val>::min();
const timestamp::time_val timestamp::kTimeValMax = std::numeric_limits<timestamp::time_val>::max();


timestamp::timestamp()
{
	update();
}


timestamp::timestamp(time_val tv)
{
	_ts = tv;
}


timestamp::timestamp(const timestamp& other)
{
	_ts = other._ts;
}


timestamp::~timestamp()
{
}


timestamp& timestamp::operator = (const timestamp& other)
{
	_ts = other._ts;
	return *this;
}


timestamp& timestamp::operator = (time_val tv)
{
	_ts = tv;
	return *this;
}


void timestamp::swap(timestamp& stamp)
{
	std::swap(_ts, stamp._ts);
}


timestamp timestamp::from_epoch_time(std::time_t t)
{
	return timestamp(time_val(t)*ratio());
}

timestamp timestamp::now()
{
	return timestamp();
}

timestamp timestamp::from_utc_time(utc_time_val val)
{
	val -= (time_diff(0x01b21dd2) << 32) + 0x13814000;
	val /= 10;
	return timestamp(val);
}


void timestamp::update()
{
	struct timeval tv;
	if (gettimeofday(&tv, NULL)) {
		_ts = 0;
		return ;
	}

	_ts = time_val(tv.tv_sec)*ratio() + tv.tv_usec;
}


timestamp  timestamp::operator + (const timespan& span) const
{

	return *this + span.total_microseconds();
}


timestamp  timestamp::operator - (const timespan& span) const
{
	return *this - span.total_microseconds();
}


timestamp& timestamp::operator += (const timespan& span)
{
	_ts += span.total_microseconds();
	return *this;
}


timestamp& timestamp::operator -= (const timespan& span)
{
	_ts -= span.total_microseconds();
	return *this;
}

struct timespec timestamp::timespec() const
{
	int64_t microseconds = raw();
	if (microseconds < 100) {
		microseconds = 100;
	}
	struct timespec ts;
	ts.tv_sec = static_cast<time_t>(
			microseconds /  ksl::timestamp::ratio());
	ts.tv_nsec = static_cast<long>(
			(microseconds %  ksl::timestamp::ratio()) * 1000);
	return ts;
}

} 