#ifndef KOB_KSL_CLOCK_H_
#define KOB_KSL_CLOCK_H_

#include <kob/profile/prof.h>
#include <cstdint>
#include <algorithm>
#include <time.h>
#include <unistd.h>

namespace ksl {

/**
 * @brief nano second 
 * 
 */
enum class clock_type {
    eNon,
    eRealTime,
    eMonotonic,
    eProcessCpuId,
    eThreadCpuId
};

/** 
 * @brief clock value in microsecond resolution.
 */
typedef int64_t clock_val;

/** 
 * @brief  different between two clocks in microsecond
 */
typedef int64_t clock_diff;

template <clock_type Type = clock_type::eRealTime>
class clock{
public:
    static const clock_val kclockValMax = 0x7FFFFFFFFFFFFFFF; //!< maximum clock value.
    static const clock_val kclockValMin = 0; //!< minimum clock value
public:
    /** 
     * @brief  default constructor will use update().
     * @note   
     * @retval 
     */
    clock();

    /** 
     * @brief  create a clock from given value.
     * @note   
     * @param  tv: clock value in microsecond.
     * @retval 
     */
    clock(clock_val tv);

    /** 
     * @brief  cpoy an other clock to this
     * @note   
     * @param  &other: clock object
     * @retval 
     */
    clock(const clock &other);

    /** 
     * @brief  destructor, no operation.
     * @note   
     * @retval 
     */
    ~clock();

    /** 
     * @brief  assign operator
     * @note   you can use this to trans two different
     *         type.
     * @param  &other: 
     * @retval None
     */
    clock& operator = (const clock &other);
    template <typename IntType>
    clock& operator = (IntType  tv);

    /** 
     * @brief  swap two clock
     * @note   
     * @param  &other: 
     * @retval None
     */
    void swap(clock &other);

    /** 
     * @brief  update stored clockval to now.
     * @note   
     * @retval None
     */
    void update();

    operator timespec() const;
    bool operator == (const clock& ts) const;
	bool operator != (const clock& ts) const;
	bool operator >  (const clock& ts) const;
	bool operator >= (const clock& ts) const;
	bool operator <  (const clock& ts) const;
    bool operator <= (const clock& ts) const;
    
    template <typename IntType>
    bool operator == (const IntType& ts) const;
    template <typename IntType>
    bool operator != (const IntType& ts) const;
    template <typename IntType>
    bool operator >  (const IntType& ts) const;
    template <typename IntType>
    bool operator >= (const IntType& ts) const;
    template <typename IntType>
    bool operator <  (const IntType& ts) const;
    template <typename IntType>
    bool operator <= (const IntType& ts) const;

	template <typename IntType>
    clock  operator +  (const IntType &d) const;
    clock  operator +  (const clock &ts) const;
    
    template <typename IntType>
    clock  operator *  (const IntType &d) const;

    template <typename IntType>
    clock  operator /  (const IntType &d) const;

    template <typename IntType>
    clock  operator %  (const IntType &d) const;

    template <typename IntType>
    clock  operator -  (const IntType &d) const;
    clock  operator -  (const clock &ts) const;
    
    template <typename IntType>
    clock& operator += (const IntType &d);
    clock& operator += (const clock &ts);

    template <typename IntType>
    clock& operator -= (const IntType &d);
    clock& operator -= (const clock &ts);

    template <typename IntType>
    clock& operator *= (const IntType &d);

    template <typename IntType>
    clock& operator /= (const IntType &d);

    template <typename IntType>
    clock& operator %= (const IntType &d);

    clock_val micoseconds() const;

    clock_val raw() const;
    
    clock_diff elapsed() const;

    bool is_elapsed(clock_diff interval) const;

    static clock_diff ratio();

    static clock_diff accuracy();

    static clock_type type();
private:
    static clockid_t clock_type_to_system();
private:
    clockid_t        _type;
    clock_val        _clock;
};

// 
// inlines
//

template <clock_type Type>
KOB_FORCE_INLINE clock<Type>::clock()
    : _type(clock_type_to_system()),
      _clock(0)
{
    KOB_STATIC_ASSERT_MSG((Type == clock_type::eRealTime ||
                   Type == clock_type::eMonotonic ||
                   Type == clock_type::eProcessCpuId ||
                   Type == clock_type::eThreadCpuId), "not known type");
    
    update();
}

template <clock_type Type>
KOB_FORCE_INLINE clockid_t clock<Type>::clock_type_to_system()
{
    switch(Type) {
        case clock_type::eRealTime :
            return CLOCK_REALTIME;
        case clock_type::eMonotonic :
            return CLOCK_MONOTONIC;
        case clock_type::eProcessCpuId:
            return CLOCK_PROCESS_CPUTIME_ID;
        case clock_type::eThreadCpuId:
            return CLOCK_THREAD_CPUTIME_ID;
        default:
            return CLOCK_MONOTONIC;
    }
}
template <clock_type Type>
KOB_FORCE_INLINE clock<Type>::~clock()
{
    
}

template <clock_type Type>
KOB_FORCE_INLINE clock<Type>::clock(const clock &other)
{
    _clock = other._clock;
}
template <clock_type Type>
KOB_FORCE_INLINE clock<Type>& clock<Type>::operator = (const clock<Type>& ts)
{
    _clock = ts._clock;
    _type  = ts._type;
    return *this;
}

template <clock_type Type>
template <typename IntType>
KOB_FORCE_INLINE clock<Type>& clock<Type>::operator = (IntType ts)
{
    KOB_STATIC_ASSERT_MSG(std::is_integral<IntType>::value, "parameter should be integral types");
    _clock = ts;
    _type = Type;
    return *this;
}

template <clock_type Type>
KOB_FORCE_INLINE bool clock<Type>::operator == (const clock& ts) const
{
	return _clock == ts._clock;
}

template <clock_type Type>
template <typename IntType>
KOB_FORCE_INLINE bool clock<Type>::operator ==(const IntType &v) const
{
    KOB_STATIC_ASSERT_MSG(std::is_integral<IntType>::value, "parameter should be integral types");
    return _clock == static_cast<int64_t>(v);
}

template <clock_type Type>
KOB_FORCE_INLINE bool clock<Type>::operator != (const clock& ts) const
{
	return _clock != ts._clock;
}

template <clock_type Type>
template <typename IntType>
KOB_FORCE_INLINE bool clock<Type>::operator != (const IntType &v) const
{
    KOB_STATIC_ASSERT_MSG(std::is_integral<IntType>::value, "parameter should be integral types");
    return _clock != static_cast<int64_t>(v);
}

template <clock_type Type>
KOB_FORCE_INLINE bool clock<Type>::operator > (const clock& ts) const
{
	return _clock > ts._clock;
}

template <clock_type Type>
template <typename IntType>
KOB_FORCE_INLINE bool clock<Type>::operator > (const IntType &v) const
{
    KOB_STATIC_ASSERT_MSG(std::is_integral<IntType>::value, "parameter should be integral types");
    return _clock > static_cast<int64_t>(v);
}

template <clock_type Type>
KOB_FORCE_INLINE bool clock<Type>::operator >= (const clock& ts) const
{
	return _clock >= ts._clock;
}

template <clock_type Type>
template <typename IntType>
KOB_FORCE_INLINE bool clock<Type>::operator >= (const IntType &v) const
{
    KOB_STATIC_ASSERT_MSG(std::is_integral<IntType>::value, "parameter should be integral types");
    return _clock >= static_cast<int64_t>(v);
}

template <clock_type Type>
KOB_FORCE_INLINE bool clock<Type>::operator < (const clock& ts) const
{
	return _clock < ts._clock;
}

template <clock_type Type>
template <typename IntType>
KOB_FORCE_INLINE bool clock<Type>::operator < (const IntType &v) const
{
    KOB_STATIC_ASSERT_MSG(std::is_integral<IntType>::value, "parameter should be integral types");
    return _clock < static_cast<int64_t>(v);
}

template <clock_type Type>
KOB_FORCE_INLINE bool clock<Type>::operator <= (const clock& ts) const
{
	return _clock <= ts._clock;
}

template <clock_type Type>
template <typename IntType>
KOB_FORCE_INLINE bool clock<Type>::operator <= (const IntType &v) const
{
    KOB_STATIC_ASSERT_MSG(std::is_integral<IntType>::value, "parameter should be integral types");
    return _clock <= static_cast<int64_t>(v);
}

template <clock_type Type>
KOB_FORCE_INLINE clock<Type> clock<Type>::operator + (const clock<Type> &ts) const
{
    clock_val cv = _clock + ts._clock;
    return clock<Type>(cv);
}

template <clock_type Type>
template <typename IntType>
KOB_FORCE_INLINE clock<Type> clock<Type>::operator + (const IntType &ts) const
{
    KOB_STATIC_ASSERT_MSG(std::is_integral<IntType>::value, "parameter should be integral types");
    clock_val cv = _clock + static_cast<int64_t>(ts);
    return clock<Type>(cv);
}

template <clock_type Type>
template <typename IntType>
KOB_FORCE_INLINE clock<Type> clock<Type>::operator * (const IntType &ts) const
{
    KOB_STATIC_ASSERT_MSG(std::is_integral<IntType>::value, "parameter should be integral types");
    clock_val cv = _clock * static_cast<int64_t>(ts);
    return clock<Type>(cv);
}

template <clock_type Type>
template <typename IntType>
KOB_FORCE_INLINE clock<Type> clock<Type>::operator / (const IntType &ts) const
{
    KOB_STATIC_ASSERT_MSG(std::is_integral<IntType>::value, "parameter should be integral types");
    clock_val cv = _clock / static_cast<int64_t>(ts);
    return clock<Type>(cv);
}

template <clock_type Type>
template <typename IntType>
KOB_FORCE_INLINE clock<Type> clock<Type>::operator % (const IntType &ts) const
{
    KOB_STATIC_ASSERT_MSG(std::is_integral<IntType>::value, "parameter should be integral types");
    clock_val cv = _clock % static_cast<int64_t>(ts);
    return clock<Type>(cv);
}

template <clock_type Type>
KOB_FORCE_INLINE clock<Type> clock<Type>::operator - (const clock<Type> &ts) const
{
    clock_val cv = _clock + ts._clock;
    KOB_ASSERT_MSG(cv > 0, "clock shoud be > 0");
    return clock<Type>(cv);
}

template <clock_type Type>
template <typename IntType>
KOB_FORCE_INLINE clock<Type> clock<Type>::operator - (const IntType &ts) const
{
    KOB_STATIC_ASSERT_MSG(std::is_integral<IntType>::value, "parameter should be integral types");
    clock_val cv = _clock - static_cast<int64_t>(ts);
    KOB_ASSERT_MSG(cv >= 0, "clock shoud be > 0");
    return clock<Type>(cv);
}

template <clock_type Type>
clock<Type>& clock<Type>::operator += (const clock<Type> &ts)
{
    _clock += ts._clock;
    return *this;
}

template <clock_type Type>
template <typename IntType>
KOB_FORCE_INLINE clock<Type>& clock<Type>::operator += (const IntType &ts)
{
    KOB_STATIC_ASSERT_MSG(std::is_integral<IntType>::value, "parameter should be integral types");
    _clock += static_cast<int64_t>(ts);
    KOB_ASSERT_MSG(_clock >= 0, "clock shoud be > 0");
    return *this;
}

template <clock_type Type>
clock<Type>& clock<Type>::operator -= (const clock<Type> & ts)
{
    _clock -= ts._clock;
    KOB_ASSERT_MSG(_clock >= 0, "clock shoud be > 0");
    return *this;
}

template <clock_type Type>
template <typename IntType>
KOB_FORCE_INLINE clock<Type>& clock<Type>::operator -= (const IntType &ts)
{
    KOB_STATIC_ASSERT_MSG(std::is_integral<IntType>::value, "parameter should be integral types");
    _clock -= static_cast<int64_t>(ts);
    KOB_ASSERT_MSG(_clock >= 0, "clock shoud be > 0");
    return *this;
}

template <clock_type Type>
template <typename IntType>
KOB_FORCE_INLINE clock<Type>& clock<Type>::operator *= (const IntType &ts)
{
    KOB_STATIC_ASSERT_MSG(std::is_integral<IntType>::value, "parameter should be integral types");
    _clock *= static_cast<int64_t>(ts);
    KOB_ASSERT_MSG(_clock >= 0, "clock shoud be > 0");
    return *this;
}

template <clock_type Type>
template <typename IntType>
KOB_FORCE_INLINE clock<Type>& clock<Type>::operator /= (const IntType &ts)
{
    KOB_STATIC_ASSERT_MSG(std::is_integral<IntType>::value, "parameter should be integral types");
    _clock /= static_cast<int64_t>(ts);
    KOB_ASSERT_MSG(_clock >= 0, "clock shoud be > 0");
    return *this;
}

template <clock_type Type>
template <typename IntType>
KOB_FORCE_INLINE clock<Type>& clock<Type>::operator %= (const IntType &ts)
{
    KOB_STATIC_ASSERT_MSG(std::is_integral<IntType>::value, "parameter should be integral types");
    _clock *= static_cast<int64_t>(ts);
    KOB_ASSERT_MSG(_clock >= 0, "clock shoud be > 0");
    return *this;
}

template <clock_type Type>
KOB_FORCE_INLINE clock_val clock<Type>::micoseconds() const
{
    return _clock;
}

template <clock_type Type>
KOB_FORCE_INLINE clock_val clock<Type>::raw() const
{
    return _clock;
}

template <clock_type Type>
KOB_FORCE_INLINE clock_val clock<Type>::ratio()
{
    return 1000000;
}

template <clock_type Type>
KOB_FORCE_INLINE void clock<Type>::swap(clock<Type> &other)
{
    clock_val tmp = _clock;
    _clock = other._clock;
    other._clock = tmp;
}

template <clock_type Type>
KOB_FORCE_INLINE clock_type clock<Type>::type()
{
    return Type;
}

template <clock_type Type>
KOB_FORCE_INLINE clock_diff clock<Type>::accuracy()
{
    struct timespec ts;
	::clock_getres(clock_type_to_system(), &ts);
	clock_val acc = clock_val(ts.tv_sec)*ratio() + ts.tv_nsec/1000;
	return acc > 0 ? acc : 1;
}

template <clock_type Type>
KOB_FORCE_INLINE clock_diff clock<Type>::elapsed() const
{
    clock<Type> now;
    now -= *this;
    return now._clock;
}

template <clock_type Type>
KOB_FORCE_INLINE bool clock<Type>::is_elapsed(clock_diff interval) const
{
    clock<Type> now;
    now -= *this;
    return now._clock >= interval;
}

template <clock_type Type>
KOB_FORCE_INLINE void clock<Type>::update()
{
	struct timespec ts;
	::clock_gettime(_type, &ts);
	_clock = clock_val(ts.tv_sec)*ratio() + ts.tv_nsec/1000;
}

template <clock_type Type>
clock<Type>::operator timespec() const
{
    struct timespec ts;
    ts.tv_sec = _clock / ratio();
    ts.tv_nsec = (_clock % ratio()) * 1000;
    return ts;
}

typedef clock<clock_type::eRealTime>      real_time_clock;
typedef clock<clock_type::eMonotonic>     monotonic_time_clock;
typedef clock<clock_type::eProcessCpuId>  process_time_clock;
typedef clock<clock_type::eThreadCpuId>   thread_time_clock;

} //namespace ksl

#endif //KOB_KSL_CLOCK_H_