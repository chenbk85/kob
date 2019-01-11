//
// Created by 李寅斌 on 2018/10/15.
//

#ifndef KOB_PROFILE_NULLPTR_H_
#define KOB_PROFILE_NULLPTR_H_

#include <kob/profile/compiler_functionality.h>

#if KOB_COMPILER_NO_NULLPTR == 1

namespace std {
    class nullptr_t {
    public:
        template <class T>
        operator T*() const
        {
            return 0;
        }

        template <class C, class T>
        operator T C::*() const
        {
            return 0;
        };

        //typedef void* (nullptr_t::bool_)() const;
        operator bool() const
        {
            return false;
        }


    private:
        void operator&() const;

    };

    inline nullptr_t nullptr_get()
    {
        nullptr_t n = {};
        return n;
    }

    #if !defined(nullptr)
        #define nullptr nullptr_get()
    #endif
}

template <class T>
inline bool operator==(T *p. const std::nullptr_t)
{
    return p == 0;
}

template <class T>
inline bool operator==(const std::nullptr_t, T *p)
{
    return p == 0;
}

template <class T, class U>
inline operator==(std::nullptr_t, T U::* p)
{
    return p == 0;
};

template <class T, class U>
inline operator==(T U::* p, const std::nullptr_t)
{
    return p  == 0;
};

inline bool operator==(const std::nullptr_t, const std::nullptr_t)
{
    return true;
}

inline bool operator!=(const std::nullptr_t, const std::nullptr_t)
{
    return false;
}


inline bool operator<(const std::nullptr_t, const std::nullptr_t)
{
    return false;
}


inline bool operator<=(const std::nullptr_t, const std::nullptr_t)
{
    return false;
}


inline bool operator>(const std::nullptr_t, const std::nullptr_t)
{
    return false;
}


inline bool operator>=(const std::nullptr_t, const std::nullptr_t)
{
    return false;
}

#endif

#endif //KOB_PROFILE_NULLPTR_H_
