//
// Created by 李寅斌 on 2018/12/11.
//

#ifndef KOB_KSL_DETAIL_TYPE_TRANSFORMATIONS_H_
#define KOB_KSL_DETAIL_TYPE_TRANSFORMATIONS_H_
#include <kob/ksl/detail/config.h>
#include <limits.h>

namespace ksl {

    //add_const
    template <typename T, bool = is_const<T>::value || is_reference<T>::value || is_function<T>::value>
    struct add_const_helper {
        typedef T type;
    };

    template <typename T>
    struct add_const_helper<T, false> {
        typedef const T type;
    };

    template <typename T>
    struct  add_const {
        typedef typename add_const_helper<T>::type type;
    };

    #if KSL_TEMPLATE_ALIASES_ENABLED
        template <typename T>
        using add_const_t = typename add_const<T>::type;
    #endif

    //add_volatile
    template <typename T, bool = is_volatile<T>::value || is_reference<T>::value || is_function<T>::value>
    struct add_volatile_helper {
        typedef T type;
    };

    template <typename T>
    struct add_volatile_helper<T, false> {
        typedef volatile T type;
    };

    template <typename T> struct add_volatile {
        typedef typename add_volatile_helper<T>::type type;
    };

    #if KSL_TEMPLATE_ALIASES_ENABLED
        template <class T>
        using add_volatile_t = typename add_volatile<T>::type;
    #endif

    template<typename T>
    struct add_cv {
        typedef typename add_const<typename add_volatile<T>::type>::type type;
    };

    #if KSL_TEMPLATE_ALIASES_ENABLED
        template <class T>
        using add_cv_t = typename add_cv<T>::type;
    #endif

    //make_signed
    template <typename T>
    struct make_signed {
        typedef T type;
    };

    template <> struct make_signed<unsigned char>            { typedef signed char            type; };
    template <> struct make_signed<const unsigned char>      { typedef const signed char      type; };
    template <> struct make_signed<unsigned short>           { typedef signed short           type; };
    template <> struct make_signed<const unsigned short>     { typedef const signed short     type; };
    template <> struct make_signed<unsigned int>             { typedef signed int             type; };
    template <> struct make_signed<const unsigned int>       { typedef const signed int       type; };
    template <> struct make_signed<unsigned long>            { typedef signed long            type; };
    template <> struct make_signed<const unsigned long>      { typedef const signed long      type; };
    template <> struct make_signed<unsigned long long>       { typedef signed long long       type; };
    template <> struct make_signed<const unsigned long long> { typedef const signed long long type; };

    #if (defined(CHAR_MAX) && defined(UCHAR_MAX) && (CHAR_MAX == UCHAR_MAX))
        template <> struct make_signed<char>                 { typedef signed char            type; };
        template <> struct make_signed<const char>           { typedef signed char            type; };
    #endif

    #if KOB_WCHAR_T_NON_NATIVE == 0
        #if (defined(__WCHAR_MAX__) && (__WCHAR_MAX__ == 4294967295U)) // If wchar_t is a 32 bit unsigned value...
            template<>
            struct make_signed<wchar_t>{
                typedef int32_t type;
            };
        #elif (defined(__WCHAR_MAX__) && (__WCHAR_MAX__ == 65535))     // If wchar_t is a 16 bit unsigned value...
            template<>
            struct make_signed<wchar_t>{
                typedef int16_t type;
            };
        #elif (defined(__WCHAR_MAX__) && (__WCHAR_MAX__ == 255))       // If wchar_t is an 8 bit unsigned value...
            template<>
            struct make_signed<wchar_t>{
                typedef int8_t type;
            };
        #endif
    #endif

    //add_signed
    template<class T>
    struct add_signed : public make_signed<T> {
        typedef typename make_signed<T>::type type;
    };

    template <typename T> struct make_unsigned { typedef T type; };

    template <> struct make_unsigned<signed char>            { typedef unsigned char            type; };
    template <> struct make_unsigned<const signed char>      { typedef const unsigned char      type; };
    template <> struct make_unsigned<signed short>           { typedef unsigned short           type; };
    template <> struct make_unsigned<const signed short>     { typedef const unsigned short     type; };
    template <> struct make_unsigned<signed int>             { typedef unsigned int             type; };
    template <> struct make_unsigned<const signed int>       { typedef const unsigned int       type; };
    template <> struct make_unsigned<signed long>            { typedef unsigned long            type; };
    template <> struct make_unsigned<const signed long>      { typedef const unsigned long      type; };
    template <> struct make_unsigned<signed long long>       { typedef unsigned long long       type; };
    template <> struct make_unsigned<const signed long long> { typedef const unsigned long long type; };

    #if (CHAR_MIN < 0)
        template <> struct make_unsigned<char>                 { typedef unsigned char          type; };
        template <> struct make_unsigned<const char>           { typedef unsigned char          type; };
    #endif

    #ifndef KOB_WCHAR_T_NON_NATIVE
        #if (defined(__WCHAR_MAX__) && (__WCHAR_MAX__ != 4294967295U))
            template<>
            struct make_unsigned<wchar_t>{
                typedef uint32_t type;
            };
        #elif (defined(__WCHAR_MAX__) && (__WCHAR_MAX__ != 65535))
            template<>
            struct make_unsigned<wchar_t>{
                typedef uint16_t type;
            };
        #elif (defined(__WCHAR_MAX__) && (__WCHAR_MAX__ != 255))
                    template<>
                    struct make_unsigned<wchar_t>{
                        typedef uint8_t type;
                    };
        #endif
    #endif

    //add_unsigned
    template<class T>
    struct add_unsigned : public make_unsigned<T> {
        typedef typename make_signed<T>::type type;
    };

    //remove_pointer
    template<typename T> struct remove_pointer                    { typedef T type; };
    template<typename T> struct remove_pointer<T*>                { typedef T type; };
    template<typename T> struct remove_pointer<T* const>          { typedef T type; };
    template<typename T> struct remove_pointer<T* volatile>       { typedef T type; };
    template<typename T> struct remove_pointer<T* const volatile> { typedef T type; };

    #if KSL_TEMPLATE_ALIASES_ENABLED
        template <class T>
        using remove_pointer_t = typename remove_pointer<T>::type;
    #endif

    //add_pointer
    template<class T>
    struct add_pointer {
        typedef typename remove_reference<T>::type* type;
    };

    #if KSL_TEMPLATE_ALIASES_ENABLED
        template <class T>
        using add_pointer_t = typename add_pointer<T>::type;
    #endif

    //remove_extent
    template<class T>           struct remove_extent       { typedef T type; };
    template<class T>           struct remove_extent<T[]>  { typedef T type; };
    template<class T, size_t N> struct remove_extent<T[N]> { typedef T type; };

    #if KSL_TEMPLATE_ALIASES_ENABLED
        template <typename T>
        using remove_extent_t = typename remove_extent<T>::type;
    #endif

    //remove_all_extents
    template<typename T>           struct remove_all_extents       { typedef T type; };
    template<typename T, size_t N> struct remove_all_extents<T[N]> { typedef typename remove_all_extents<T>::type type; };
    template<typename T>           struct remove_all_extents<T[]>  { typedef typename remove_all_extents<T>::type type; };

    #if KSL_TEMPLATE_ALIASES_ENABLED
        template <typename T>
        using remove_all_extents_t = typename remove_all_extents<T>::type;
    #endif

    //aligned_storage

    #if defined(KOB_COMPILER_GNUC) && KOB_COMPILER_VERSION >= 4008
        template<size_t N, size_t Align = KOB_ALIGN_OF(double)>
		struct aligned_storage {
			struct type {
			    unsigned char mCharData[N];
			} KOB_ALIGN(Align);
		};
    #elif KOB_COMPILER_NO_ALIGNAS == 0
        template<size_t N, size_t Align = KOB_ALIGN_OF(double)>
        struct aligned_storage {
            typedef struct {
                alignas(Align) unsigned char mCharData[N];
            } type;
        };
    #elif defined(KOB_COMPILER_GNUC) && KOB_COMPILER_VERSION < 4007
        template<size_t N, size_t Align>
		struct aligned_storage_helper { struct type{ unsigned char mCharData[N]; }; };

		template<size_t N> struct aligned_storage_helper<N,    2> { struct KOB_ALIGN(   2) type{ unsigned char mCharData[N]; }; };
		template<size_t N> struct aligned_storage_helper<N,    4> { struct KOB_ALIGN(   4) type{ unsigned char mCharData[N]; }; };
		template<size_t N> struct aligned_storage_helper<N,    8> { struct KOB_ALIGN(   8) type{ unsigned char mCharData[N]; }; };
		template<size_t N> struct aligned_storage_helper<N,   16> { struct KOB_ALIGN(  16) type{ unsigned char mCharData[N]; }; };
		template<size_t N> struct aligned_storage_helper<N,   32> { struct KOB_ALIGN(  32) type{ unsigned char mCharData[N]; }; };
		template<size_t N> struct aligned_storage_helper<N,   64> { struct KOB_ALIGN(  64) type{ unsigned char mCharData[N]; }; };
		template<size_t N> struct aligned_storage_helper<N,  128> { struct KOB_ALIGN( 128) type{ unsigned char mCharData[N]; }; };
		template<size_t N> struct aligned_storage_helper<N,  256> { struct KOB_ALIGN( 256) type{ unsigned char mCharData[N]; }; };
		template<size_t N> struct aligned_storage_helper<N,  512> { struct KOB_ALIGN( 512) type{ unsigned char mCharData[N]; }; };
		template<size_t N> struct aligned_storage_helper<N, 1024> { struct KOB_ALIGN(1024) type{ unsigned char mCharData[N]; }; };
		template<size_t N> struct aligned_storage_helper<N, 2048> { struct KOB_ALIGN(2048) type{ unsigned char mCharData[N]; }; };
		template<size_t N> struct aligned_storage_helper<N, 4096> { struct KOB_ALIGN(4096) type{ unsigned char mCharData[N]; }; };

		template<size_t N, size_t Align = KOB_ALIGN_OF(double)>
		struct aligned_storage
		{
			typedef typename aligned_storage_helper<N, Align>::type type;
		};
    #else
		template<size_t N, size_t Align = KOB_ALIGN_OF(double)>
		struct aligned_storage
		{
			union type{
				unsigned char mCharData[N];
				struct KOB_ALIGN(Align) mStruct{ };
			};
		};
    #endif

    #if KSL_TEMPLATE_ALIASES_ENABLED
        template <size_t N, size_t Align = KOB_ALIGN_OF(double)>
        using aligned_storage_t = typename aligned_storage<N, Align>::type;
    #endif

    //aligned_union
    template <size_t minSize, typename Type0, typename ...TypeN>
    struct aligned_union {
        static const size_t size            = static_max<minSize, sizeof(Type0), sizeof(TypeN)...>::value;
        static const size_t alignment_value = static_max<KOB_ALIGN_OF(Type0), KOB_ALIGN_OF(TypeN)...>::value;

        typedef typename aligned_storage<size, alignment_value>::type type;
    };

    #if KSL_TEMPLATE_ALIASES_ENABLED
        template <size_t minSize, typename... TypeN>
        using aligned_union_t = typename aligned_union<minSize, TypeN...>::type;
    #endif

    //union_cast
    template <typename DestType, typename SourceType>
    DestType union_cast(SourceType sourceValue)
    {
        KOB_STATIC_ASSERT((sizeof(DestType) == sizeof(SourceType)) &&
                        (KOB_ALIGN_OF(DestType) == KOB_ALIGN_OF(SourceType)));               // To support differening alignments, we would need to use a memcpy-based solution or find a way to make the two union members align with each other.
        union {
            SourceType sourceValue;
            DestType   destValue;
        } u;
        u.sourceValue = sourceValue;

        return u.destValue;
    }

    //void_t
    #if KSL_VARIABLE_TEMPLATES_ENABLED
        template <class...>
            using void_t = void;
    #endif



}
#endif //KOB_KSL_DETAIL_TYPE_TRANSFORMATIONS_H_
