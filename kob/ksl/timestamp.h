#ifndef KOB_KSL_TIMESTAMP_H_
#define KOB_KSL_TIMESTAMP_H_
#include <kob/ksl/detail/config.h>
#include <ctime>
#include <cstdint>

namespace ksl {

class timespan;

/*!
* A timestamp stores a monotonic* time value
* with (theoretical) microseconds resolution.
* timestamps can be compared with each other
* and simple arithmetic is supported.
*
* [*] Note that timestamp values are only monotonic as
* long as the systems's clock is monotonic as well
* (and not, e.g. set back due to time synchronization
* or other reasons).
*
* timestamps are UTC (Coordinated Universal Time)
* based and thus independent of the timezone
* in effect on the system.
*
* The internal reference time is the Unix epoch, 
* midnight, January 1, 1970.
	*/
class timestamp {
public:
	/*!
	* Monotonic UTC time value in microsecond resolution,
	* with base time midnight, January 1, 1970.
	*/
	typedef uint64_t time_val;

	/*!
	* Monotonic UTC time value in 100 nanosecond resolution,
	* with base time midnight, October 15, 1582.
	*/
	typedef uint64_t utc_time_val;

	/*!
	* Difference between two time_val values in microseconds.
	*/	
	typedef uint64_t time_diff;

	static const time_val   kTimeValMin; //!< Minimum timestamp value.
	static const time_val   kTimeValMax; //!< Maximum timestamp value.
	static const time_diff  kRatio = 1000000;
	/*!
	* Creates a timestamp with the current time.
	*/
	timestamp();
	
	/*!
	* Creates a timestamp from the given time value
	* (microseconds since midnight, January 1, 1970).
	*/
	timestamp(time_val tv);

	/*!
	* Copy constructor, copy a timestamp from another one.
	*/
	timestamp(const timestamp& other);
		
	/*!
	* Destroys the timestamp
	*/
	~timestamp();

	/*!
	* assign operators
	*/	
	timestamp& operator = (const timestamp& other);
	timestamp& operator = (time_val tv);
	
	/*!
	* Swaps the timestamp with another one.
	*/
	void swap(timestamp& timestamp);
	
	/*!
	*Updates the timestamp with the current time.
	*/
	void update();
	
	bool operator == (const timestamp& ts) const;
	bool operator != (const timestamp& ts) const;
	bool operator >  (const timestamp& ts) const;
	bool operator >= (const timestamp& ts) const;
	bool operator <  (const timestamp& ts) const;
	bool operator <= (const timestamp& ts) const;

	
	timestamp  operator +  (time_diff d) const;
	timestamp  operator +  (const timespan& span) const;
	timestamp  operator -  (time_diff d) const;
	timestamp  operator -  (const timespan& span) const;
	time_diff   operator -  (const timestamp& ts) const;
	timestamp& operator += (time_diff d);
	timestamp& operator += (const timespan& span);
	timestamp& operator -= (time_diff d);
	timestamp& operator -= (const timespan& span);

	timestamp operator <<  (int bits);
	
	
	/*!
	*@return the timestamp expressed in time_t.
	* time_t base time is midnight, January 1, 1970.
	* Resolution is one second.
	*/
	std::time_t epoch_time() const;
	
	/*!
	*@return the timestamp expressed in UTC-based
	* time. UTC base time is midnight, October 15, 1582.
	* Resolution is 100 nanoseconds.
	*/	
	utc_time_val utc_time() const;

	/*!
	*@return  the timestamp expressed in microseconds
	* since the Unix epoch, midnight, January 1, 1970.
	*/	
	time_val epoch_microseconds() const;
	
	/*!
	*@return   the time elapsed since the time denoted by
	* the timestamp. Equivalent to timestamp() - *this.
	*/	
	time_diff elapsed() const;
	
	/*!
	*@return  true iff the given interval has passed
	* since the time denoted by the timestamp.
	*/	
	bool is_elapsed(time_diff interval) const;

	struct timespec timespec() const;
	/*!
	*@return  the raw time value.
	* Same as epoch_microseconds().
	*/
	time_val raw() const;
	
	/*!
	*@return  Creates a timestamp from a std::time_t.
	*/
	static timestamp from_epoch_time(std::time_t t);
	
	/*!
	*@return  Creates a timestamp from a UTC time value
	* (100 nanosecond intervals since midnight,
	* October 15, 1582).
	*/
	static timestamp from_utc_time(utc_time_val val);
	
	/*!
	*@return  the resolution in units per second.
	* Since the timestamp has microsecond resolution,
	* the returned value is always 1000000.
	*/
	static time_diff ratio();

	static timestamp now();

private:
	time_val _ts;
};


KOB_FORCE_INLINE bool timestamp::operator == (const timestamp& ts) const
{
	return _ts == ts._ts;
}


KOB_FORCE_INLINE bool timestamp::operator != (const timestamp& ts) const
{
	return _ts != ts._ts;
}


KOB_FORCE_INLINE bool timestamp::operator >  (const timestamp& ts) const
{
	return _ts > ts._ts;
}


KOB_FORCE_INLINE bool timestamp::operator >= (const timestamp& ts) const
{
	return _ts >= ts._ts;
}


KOB_FORCE_INLINE bool timestamp::operator <  (const timestamp& ts) const
{
	return _ts < ts._ts;
}


KOB_FORCE_INLINE bool timestamp::operator <= (const timestamp& ts) const
{
	return _ts <= ts._ts;
}


KOB_FORCE_INLINE timestamp timestamp::operator + (timestamp::time_diff d) const
{
	return timestamp(_ts + d);
}


KOB_FORCE_INLINE timestamp timestamp::operator - (timestamp::time_diff d) const
{
	return timestamp(_ts - d);
}


KOB_FORCE_INLINE timestamp::time_diff timestamp::operator - (const timestamp& ts) const
{
	return _ts - ts._ts;
}


KOB_FORCE_INLINE timestamp& timestamp::operator += (timestamp::time_diff d)
{
	_ts += d;
	return *this;
}


KOB_FORCE_INLINE timestamp& timestamp::operator -= (timestamp::time_diff d)
{
	_ts -= d;
	return *this;
}


KOB_FORCE_INLINE std::time_t timestamp::epoch_time() const
{
	return std::time_t(_ts/ratio());
}


KOB_FORCE_INLINE timestamp::utc_time_val timestamp::utc_time() const
{
	return _ts*10 + (time_diff(0x01b21dd2) << 32) + 0x13814000;
}


KOB_FORCE_INLINE timestamp::time_val timestamp::epoch_microseconds() const
{
	return _ts;
}


KOB_FORCE_INLINE timestamp::time_diff timestamp::elapsed() const
{
	timestamp dummy;
	return dummy - *this;
}


KOB_FORCE_INLINE bool timestamp::is_elapsed(timestamp::time_diff interval) const
{
	timestamp dummy;
	timestamp::time_diff diff = dummy - *this;
	return diff >= interval;
}


KOB_FORCE_INLINE timestamp::time_diff timestamp::ratio()
{
	return kRatio;
}


KOB_FORCE_INLINE void swap(timestamp& s1, timestamp& s2)
{
	s1.swap(s2);
}


KOB_FORCE_INLINE timestamp::time_val timestamp::raw() const
{
	return _ts;
}

KOB_FORCE_INLINE timestamp timestamp::operator <<  (int bits)
{
	uint64_t rc = _ts << bits;
	return timestamp(rc);
}

} //namespace ksl


#endif //KOB_KSL_TIMESTAMP_H_


