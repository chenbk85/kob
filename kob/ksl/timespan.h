#ifndef KOB_KSL_COMMON_TIMESPAN_H_
#define KOB_KSL_COMMON_TIMESPAN_H_
#include <kob/ksl/timestamp.h>


namespace ksl {
/*!
* A class that represents time spans up to microsecond resolution.
*/
class  timespan {
public:
	typedef timestamp::time_diff time_diff;
	/*!
	*Creates a  zero timespan.
	*/
	timespan();
	
	/*!
	*Creates a timespan with given time.
	*/
	timespan(time_diff microseconds);
	
	/*!
	*Creates a timespan with given time.
	*/
	timespan(long seconds, long microseconds);
	
	/*!
	*Creates a timespan with given time
	*/
	timespan(int days, int hours, int minutes, int seconds, int microSeconds);
	

	/*!
	*Creates a timespan with given another one.
	*/
	timespan(const timespan& timespan);
	/*!
	*Destroys the timespan.
	*/
	~timespan();
	
	/*!
	* Assignment operator.
	*/
	timespan& operator = (const timespan& timespan);
	
	/*!
	* Assignment operator.
	*/
	timespan& operator = (time_diff microseconds);
	
	/*!
	* Assigns a new span.
	*/
	timespan& assign(int days, int hours, int minutes, int seconds, int microSeconds);
	
	/*!
	* Assigns a new span.
	*/	
	timespan& assign(long seconds, long microseconds);
	
	/*!
	* Swaps the timespan with another one.
	*/	
	void swap(timespan& timespan);

	bool operator == (const timespan& ts) const;
	bool operator != (const timespan& ts) const;
	bool operator >  (const timespan& ts) const;
	bool operator >= (const timespan& ts) const;
	bool operator <  (const timespan& ts) const;
	bool operator <= (const timespan& ts) const;

	bool operator == (time_diff microSeconds) const;
	bool operator != (time_diff microSeconds) const;
	bool operator >  (time_diff microSeconds) const;
	bool operator >= (time_diff microSeconds) const;
	bool operator <  (time_diff microSeconds) const;
	bool operator <= (time_diff microSeconds) const;
	
	timespan operator + (const timespan& d) const;
	timespan operator - (const timespan& d) const;
	timespan& operator += (const timespan& d);
	timespan& operator -= (const timespan& d);

	timespan operator + (time_diff microSeconds) const;
	timespan operator - (time_diff microSeconds) const;
	timespan& operator += (time_diff microSeconds);
	timespan& operator -= (time_diff microSeconds);

	/*!
	*@return the number of days.
	*/	
	int days() const;
	
	/*!
	*@return the number of hours (0 to 23).
	*/	
	int hours() const;
	
	/*!
	*@return the total number of hours.
	*/		
	int total_hours() const;

	/*!
	*@return the number of minutes (0 to 59).
	*/			
	int minutes() const;
	
	/*!
	*@return the total number of minutes.
	*/		
	int total_minutes() const;

	struct timespec timespec() const;
	/*!
	*@return the number of seconds (0 to 59).
	*/		
	int seconds() const;
	
	/*!
	*@return the total number of seconds.
	*/	
	int total_seconds() const;
	
	/*!
	*@return the number of milliseconds (0 to 999).
	*/	
	int milliseconds() const;
	
	/*!
	*@return the total number of milliseconds.
	*/		
	time_diff total_milliseconds() const;
	
	/*!
	*@return the fractions of a millisecond in microseconds (0 to 999).
	*/		
	int microseconds() const;

	/*!
	*@return  the fractions of a second in microseconds (0 to 999999).
	*/	
	int useconds() const;

	/*!
	*@return  the total number of microseconds.
	*/		
	time_diff total_microseconds() const;

	static const time_diff kMILLISECONDS; //!< The number of microseconds in a millisecond.
	static const time_diff kSECONDS;      //!< The number of microseconds in a second.
	static const time_diff kMINUTES;      //!< The number of microseconds in a minute.
	static const time_diff kHOURS;       //!< The number of microseconds in a hour.
	static const time_diff kDAYS;        //!< The number of microseconds in a day.   

private:
	time_diff _span;
};



KOB_FORCE_INLINE int timespan::days() const
{
	return int(_span/kDAYS);
}


KOB_FORCE_INLINE int timespan::hours() const
{
	return int((_span/kHOURS) % 24);
}

	
KOB_FORCE_INLINE int timespan::total_hours() const
{
	return int(_span/kHOURS);
}

	
KOB_FORCE_INLINE int timespan::minutes() const
{
	return int((_span/kMINUTES) % 60);
}

	
KOB_FORCE_INLINE int timespan::total_minutes() const
{
	return int(_span/kMINUTES);
}

	
KOB_FORCE_INLINE int timespan::seconds() const
{
	return int((_span/kSECONDS) % 60);
}

	
KOB_FORCE_INLINE int timespan::total_seconds() const
{
	return int(_span/kSECONDS);
}

	
KOB_FORCE_INLINE int timespan::milliseconds() const
{
	return int((_span/kMILLISECONDS) % 1000);
}

	
KOB_FORCE_INLINE timespan::time_diff timespan::total_milliseconds() const
{
	return _span/kMILLISECONDS;
}

	
KOB_FORCE_INLINE int timespan::microseconds() const
{
	return int(_span % 1000);
}


KOB_FORCE_INLINE int timespan::useconds() const
{
	return int(_span % 1000000);
}

	
KOB_FORCE_INLINE timespan::time_diff timespan::total_microseconds() const
{
	return _span;
}


KOB_FORCE_INLINE bool timespan::operator == (const timespan& ts) const
{
	return _span == ts._span;
}


KOB_FORCE_INLINE bool timespan::operator != (const timespan& ts) const
{
	return _span != ts._span;
}


KOB_FORCE_INLINE bool timespan::operator >  (const timespan& ts) const
{
	return _span > ts._span;
}


KOB_FORCE_INLINE bool timespan::operator >= (const timespan& ts) const
{
	return _span >= ts._span;
}


KOB_FORCE_INLINE bool timespan::operator <  (const timespan& ts) const
{
	return _span < ts._span;
}


KOB_FORCE_INLINE bool timespan::operator <= (const timespan& ts) const
{
	return _span <= ts._span;
}


KOB_FORCE_INLINE bool timespan::operator == (time_diff microSeconds) const
{
	return _span == microSeconds;
}


KOB_FORCE_INLINE bool timespan::operator != (time_diff microSeconds) const
{
	return _span != microSeconds;
}


KOB_FORCE_INLINE bool timespan::operator >  (time_diff microSeconds) const
{
	return _span > microSeconds;
}


KOB_FORCE_INLINE bool timespan::operator >= (time_diff microSeconds) const
{
	return _span >= microSeconds;
}


KOB_FORCE_INLINE bool timespan::operator <  (time_diff microSeconds) const
{
	return _span < microSeconds;
}


KOB_FORCE_INLINE bool timespan::operator <= (time_diff microSeconds) const
{
	return _span <= microSeconds;
}


KOB_FORCE_INLINE void swap(timespan& s1, timespan& s2)
{
	s1.swap(s2);
}


} //namespace ksl

#endif  //KOB_KSL_COMMON_TIMESPAN_H_