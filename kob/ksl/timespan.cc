#include <kob/ksl/timespan.h>
#include <algorithm>
#include <kob/log/logging.h>

namespace ksl {


const timespan::time_diff timespan::kMILLISECONDS = 1000;
const timespan::time_diff timespan::kSECONDS      = 1000*timespan::kMILLISECONDS;
const timespan::time_diff timespan::kMINUTES      =   60*timespan::kSECONDS;
const timespan::time_diff timespan::kHOURS        =   60*timespan::kMINUTES;
const timespan::time_diff timespan::kDAYS         =   24*timespan::kHOURS;


timespan::timespan():
	_span(0)
{
}

	
timespan::timespan(time_diff microSeconds):
	_span(microSeconds)
{
}


timespan::timespan(long otherSeconds, long otherMicroSeconds):
	_span(time_diff(otherSeconds)*kSECONDS + otherMicroSeconds)
{
}

	
timespan::timespan(int otherDays, int otherHours, int otherMinutes, int otherSeconds, int otherMicroSeconds):
	_span(time_diff(otherMicroSeconds) + time_diff(otherSeconds)*kSECONDS + time_diff(otherMinutes)*kMINUTES + time_diff(otherHours)*kHOURS + time_diff(otherDays)*kDAYS)
{
}


timespan::timespan(const timespan& span):
	_span(span._span)
{
}


timespan::~timespan()
{
}


timespan& timespan::operator = (const timespan& span)
{
	_span = span._span;
	return *this;
}


timespan& timespan::operator = (time_diff microSeconds)
{

	_span = microSeconds;
	return *this;
}


timespan& timespan::assign(int otherDays, int otherHours, int otherMinutes, int otherSeconds, int otherMicroSeconds)
{
	_span = time_diff(otherMicroSeconds) + 
			time_diff(otherSeconds)*kSECONDS + 
			time_diff(otherMinutes)*kMINUTES + 
			time_diff(otherHours)*kHOURS + 
			time_diff(otherDays)*kDAYS;
	return *this;
}


timespan& timespan::assign(long otherSeconds, long otherMicroSeconds)
{
	_span = time_diff(otherSeconds)*kSECONDS + time_diff(otherMicroSeconds);
	return *this;
}


void timespan::swap(timespan& span)
{
	std::swap(_span, span._span);
}


timespan timespan::operator + (const timespan& d) const
{
	return timespan(_span + d._span);
}


timespan timespan::operator - (const timespan& d) const
{
	return timespan(_span - d._span);
}


timespan& timespan::operator += (const timespan& d)
{
	_span += d._span;
	return *this;
}


timespan& timespan::operator -= (const timespan& d)
{
	_span -= d._span;
	return *this;
}


timespan timespan::operator + (time_diff microSeconds) const
{
	return timespan(_span + microSeconds);
}


timespan timespan::operator - (time_diff microSeconds) const
{
	return timespan(_span - microSeconds);
}


timespan& timespan::operator += (time_diff microSeconds)
{
	_span += microSeconds;
	return *this;
}


timespan& timespan::operator -= (time_diff microSeconds)
{
	_span -= microSeconds;
	return *this;
}

struct timespec timespan::timespec() const
{
	int64_t micoSeconds = total_microseconds();
	if (micoSeconds < 100) {
		micoSeconds = 100;
	}
	struct timespec ts;
	ts.tv_sec = static_cast<time_t>(
			micoSeconds /  ksl::timestamp::ratio());
	ts.tv_nsec = static_cast<long>(
			(micoSeconds %  ksl::timestamp::ratio()) * 1000);
	return ts;
}

} //namespace ksl