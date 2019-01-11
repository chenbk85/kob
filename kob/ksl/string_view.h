#ifndef KOB_KSL_STRING_VIEW_H_
#define KOB_KSL_STRING_VIEW_H_


#include <kob/ksl/detail/config.h>
#include <kob/ksl/detail/char_traits.h>
#include <kob/ksl/ascii.h>
#include <algorithm>
#include <iterator>
#include <limits>
#include <string>

namespace ksl {
	template <typename T>
	class basic_string_view {
	public:
		typedef basic_string_view<T>						this_type;
		typedef T 											value_type;
		typedef T* 											pointer;
		typedef const T* 									const_pointer;
		typedef T& 											reference;
		typedef const T& 									const_reference;
		typedef T* 											iterator;
		typedef const T* 									const_iterator;
		typedef std::reverse_iterator<iterator> 			reverse_iterator;
		typedef std::reverse_iterator<const_iterator> 	    const_reverse_iterator;
		typedef size_t 										size_type;
		typedef ptrdiff_t 									difference_type;

		static const KOB_CONSTEXPR size_type npos = size_type(-1);

	protected:
		const_pointer _begin = nullptr;
		size_type     _count = 0;

	public:
		// 21.4.2.1, construction and assignment
		KOB_FUNC_CONSTEXPR basic_string_view() KOB_NOEXCEPT
		: _begin(nullptr),
		  _count(0)
		{

		}
		KOB_FUNC_CONSTEXPR basic_string_view(const basic_string_view& other) KOB_NOEXCEPT = default;

		KOB_FUNC_CONSTEXPR basic_string_view(const T* s, size_type count)
		: _begin(s),
		  _count(count)
  		{

  		}
		KOB_FUNC_CONSTEXPR basic_string_view(const T* s)
		: _begin(s),
		  _count(s != nullptr ? ksl::detail::char_strlen(s) : 0)
  		{

  		}
        KOB_FUNC_CONSTEXPR basic_string_view(const std::string &s)
        : _begin(s.data()),
          _count(s.size())
	  	{

	  	}

		KOB_FUNC_CONSTEXPR basic_string_view(const_iterator ib, const_iterator ie)
		: _begin(ib),
		  _count(ie - ib)
		{

		}



        basic_string_view& operator=(const basic_string_view& view) = default;

        basic_string_view& operator=(const value_type *str)
        {
            basic_string_view sv(str);
            *this = sv;
            return *this;
        }
		// 21.4.2.2, iterator support
		KOB_FUNC_CONSTEXPR const_iterator begin() const KOB_NOEXCEPT { return _begin; }
		KOB_FUNC_CONSTEXPR const_iterator cbegin() const KOB_NOEXCEPT { return _begin; }
		KOB_FUNC_CONSTEXPR const_iterator end() const KOB_NOEXCEPT { return _begin + _count; }
		KOB_FUNC_CONSTEXPR const_iterator cend() const KOB_NOEXCEPT { return _begin + _count; }
		KOB_FUNC_CONSTEXPR const_reverse_iterator rbegin() const KOB_NOEXCEPT { return const_reverse_iterator(_begin + _count); }
		KOB_FUNC_CONSTEXPR const_reverse_iterator crbegin() const KOB_NOEXCEPT { return const_reverse_iterator(_begin + _count); }
		KOB_FUNC_CONSTEXPR const_reverse_iterator rend() const KOB_NOEXCEPT { return const_reverse_iterator(_begin); }
		KOB_FUNC_CONSTEXPR const_reverse_iterator crend() const KOB_NOEXCEPT { return const_reverse_iterator(_begin); }


		// 21.4.2.4, element access
		KOB_FUNC_CONSTEXPR const_pointer data() const { return _begin; }
		KOB_FUNC_CONSTEXPR const_reference front() const
		{
			return [&] { KOB_ASSERT_MSG(!empty(), "behavior is undefined if string_view is empty"); }(), _begin[0];
		}

		KOB_FUNC_CONSTEXPR const_reference back() const
		{
			return [&] { KOB_ASSERT_MSG(!empty(), "behavior is undefined if string_view is empty"); }(), _begin[_count - 1];
		}

		KOB_FUNC_CONSTEXPR const_reference operator[](size_type pos) const
		{
			// As per the standard spec: No bounds checking is performed: the behavior is undefined if pos >= size().
			return _begin[pos];
		}

		KOB_FUNC_CONSTEXPR const_reference at(size_type pos) const
		{

			if(KOB_UNLIKELY(pos >= _count))
				KOB_FAIL_MSG("string_view::at -- out of range");

			return _begin[pos];
		}


		// 21.4.2.3, capacity
		KOB_FUNC_CONSTEXPR size_type size() const KOB_NOEXCEPT { return _count; }
		KOB_FUNC_CONSTEXPR size_type length() const KOB_NOEXCEPT { return _count; }

		// avoid macro expansion of max(...) from windows headers (potentially included before this file)
		// by wrapping function name in brackets
		KOB_FUNC_CONSTEXPR size_type max_size() const KOB_NOEXCEPT { return (std::numeric_limits<size_type>::max)(); }
		KOB_FUNC_CONSTEXPR bool empty() const KOB_NOEXCEPT { return _count == 0; }


		// 21.4.2.5, modifiers
		KOB_FUNC_CONSTEXPR void swap(basic_string_view& v)
		{
			std::swap(_begin, v._begin);
			std::swap(_count, v._count);
		}

		KOB_FUNC_CONSTEXPR void remove_prefix(size_type n)
		{
			KOB_ASSERT_MSG(n <= _count, "behavior is undefined if moving past the end of the string");
			_begin += n;
			_count -= n;
		}

		KOB_FUNC_CONSTEXPR void trim_left()
		{
			while (_count > 0 && ksl::ascii::is_space(*_begin)) {
				_begin++;
				_count--;
			}
		}

		KOB_FUNC_CONSTEXPR void trim_right()
		{
			const value_type* pEnd = _begin + (_count -1);
			while (_count > 0 && ksl::ascii::is_space(*pEnd)) {
				pEnd--;
				_count--;
			}
		}

		KOB_FUNC_CONSTEXPR void trim()
		{
			trim_left();
			trim_right();
		}

		KOB_FUNC_CONSTEXPR void remove_suffix(size_type n)
		{
			KOB_ASSERT_MSG(n <= _count, "behavior is undefined if moving past the beginning of the string");
			_count -= n;
		}


		// 21.4.2.6, string operations
		size_type copy(T* pDestination, size_type count, size_type pos = 0) const
		{

			if(KOB_UNLIKELY(pos > _count))
				KOB_FAIL_MSG("string_view::copy -- out of range");

			count = std::min(count, _count - pos);
			auto* pResult = CharStringUninitializedCopy(_begin + pos, _begin + pos + count, pDestination);
			// *pResult = 0; // don't write the null-terminator
			return pResult - pDestination;
		}

		KOB_FUNC_CONSTEXPR basic_string_view substr(size_type pos = 0, size_type count = npos) const
		{
			if (KOB_UNLIKELY(pos > _count))
					KOB_FAIL_MSG("string_view::substr -- out of range");

			count = std::min(count, _count - pos);
			return this_type(_begin + pos, count);
		}

		static KOB_FUNC_CONSTEXPR int compare(const T* pBegin1, const T* pEnd1, const T* pBegin2, const T* pEnd2)
		{
			const ptrdiff_t n1   = pEnd1 - pBegin1;
			const ptrdiff_t n2   = pEnd2 - pBegin2;
			const ptrdiff_t nMin = std::min(n1, n2);
			const int       cmp  = ksl::detail::compare(pBegin1, pBegin2, (size_t)nMin);

			return (cmp != 0 ? cmp : (n1 < n2 ? -1 : (n1 > n2 ? 1 : 0)));
		}

		static KOB_FUNC_CONSTEXPR int compare_icase(const T* pBegin1, const T* pEnd1, const T* pBegin2, const T* pEnd2)
		{
			const ptrdiff_t n1   = pEnd1 - pBegin1;
			const ptrdiff_t n2   = pEnd2 - pBegin2;
			const ptrdiff_t nMin = std::min(n1, n2);
			const int       cmp  = ksl::detail::compare_icase(pBegin1, pBegin2, (size_t)nMin);

			return (cmp != 0 ? cmp : (n1 < n2 ? -1 : (n1 > n2 ? 1 : 0)));
		}

		KOB_FUNC_CONSTEXPR int compare(basic_string_view sw) const KOB_NOEXCEPT
		{
			return compare(_begin, _begin + _count, sw._begin, sw._begin + sw._count);
		}

		KOB_FUNC_CONSTEXPR int compare_icase(basic_string_view sw) const KOB_NOEXCEPT
		{
			return compare_icase(_begin, _begin + _count, sw._begin, sw._begin + sw._count);
		}

		KOB_FUNC_CONSTEXPR int compare(size_type pos1, size_type count1, basic_string_view sw) const
		{
			return substr(pos1, count1).compare(sw);
		}

		KOB_FUNC_CONSTEXPR int compare_icase(size_type pos1, size_type count1, basic_string_view sw) const
		{
			return substr(pos1, count1).compare_icase(sw);
		}

		KOB_FUNC_CONSTEXPR int compare(size_type pos1,
		                         size_type count1,
		                         basic_string_view sw,
		                         size_type pos2,
		                         size_type count2) const
		{
			return substr(pos1, count1).compare(sw.substr(pos2, count2));
		}

		KOB_FUNC_CONSTEXPR int compare_icase(size_type pos1,
									   size_type count1,
									   basic_string_view sw,
									   size_type pos2,
									   size_type count2) const
		{
			return substr(pos1, count1).compare_icase(sw.substr(pos2, count2));
		}

		KOB_FUNC_CONSTEXPR int compare(const T* s) const
		{
			return compare(basic_string_view(s));
		}

		KOB_FUNC_CONSTEXPR int compare_icase(const T* s) const
		{
			return compare_icase(basic_string_view(s));
		}


		KOB_FUNC_CONSTEXPR int compare(size_type pos1, size_type count1, const T* s) const
		{
			return substr(pos1, count1).compare(basic_string_view(s));
		}

		KOB_FUNC_CONSTEXPR int compare_icase(size_type pos1, size_type count1, const T* s) const
		{
			return substr(pos1, count1).compare_icase(basic_string_view(s));
		}

		KOB_FUNC_CONSTEXPR int compare(size_type pos1, size_type count1, const T* s, size_type count2) const
		{
			return substr(pos1, count1).compare(basic_string_view(s, count2));
		}

		KOB_FUNC_CONSTEXPR int compare_icase(size_type pos1, size_type count1, const T* s, size_type count2) const
		{
			return substr(pos1, count1).compare_icase(basic_string_view(s, count2));
		}

		KOB_FUNC_CONSTEXPR size_type find(basic_string_view sw, size_type pos = 0) const KOB_NOEXCEPT
		{
			auto* pEnd = _begin + _count;
			if (KOB_LIKELY(((npos - sw.size()) >= pos) && (pos + sw.size()) <= _count))
			{
				const value_type* const pTemp = std::search(_begin + pos, pEnd, sw.data(), sw.data() + sw.size());

				if ((pTemp != pEnd) || (sw.size() == 0))
					return (size_type)(pTemp - _begin);
			}
			return npos;
		}

		KOB_FUNC_CONSTEXPR size_type find(T c, size_type pos = 0) const KOB_NOEXCEPT
		{
			return find(basic_string_view(&c, 1), pos);
		}

		KOB_FUNC_CONSTEXPR size_type find(const T* s, size_type pos, size_type count) const
		{
			return find(basic_string_view(s, count), pos);
		}

		KOB_FUNC_CONSTEXPR size_type find(const T* s, size_type pos = 0) const { return find(basic_string_view(s), pos); }

		KOB_FUNC_CONSTEXPR size_type rfind(basic_string_view sw, size_type pos = npos) const KOB_NOEXCEPT
		{
			return rfind(sw._begin, pos, sw._count);
		}

		KOB_FUNC_CONSTEXPR size_type rfind(T c, size_type pos = npos) const KOB_NOEXCEPT
		{
			if (KOB_LIKELY(_count))
			{
				const value_type* const pEnd = _begin + std::min(_count - 1, pos) + 1;
				const value_type* const pResult = ksl::detail::char_type_string_rfind(pEnd, _begin, c);

				if (pResult != _begin)
					return (size_type)((pResult - 1) - _begin);
			}
			return npos;
		}

		KOB_FUNC_CONSTEXPR size_type rfind(const T* s, size_type pos, size_type n) const
		{

			if (KOB_LIKELY(n <= _count)) {
				if (KOB_LIKELY(n)) {
					const const_iterator pEnd = _begin + std::min(_count - n, pos) + n;
					const const_iterator pResult = ksl::detail::char_type_string_rsearch(_begin, pEnd, s, s + n);

					if (pResult != pEnd)
						return (size_type)(pResult - _begin);
				} else {
                    return std::min(_count, pos);
                }
			}
			return npos;
		}

		KOB_FUNC_CONSTEXPR size_type rfind(const T* s, size_type pos = npos) const
		{
			return rfind(s, pos, (size_type)ksl::detail::char_strlen(s));
		}

		KOB_FUNC_CONSTEXPR size_type find_first_of(basic_string_view sw, size_type pos = 0) const KOB_NOEXCEPT
		{
			return find_first_of(sw._begin, pos, _count);
		}

		KOB_FUNC_CONSTEXPR size_type find_first_of(T c, size_type pos = 0) const KOB_NOEXCEPT
		{
			return find(c, pos);
		}

		KOB_FUNC_CONSTEXPR size_type find_first_of(const T* s, size_type pos, size_type n) const
		{
			// If position is >= size, we return npos.
			if (KOB_LIKELY((pos < _count))) {
				const value_type* const pBegin = _begin + pos;
				const value_type* const pEnd = _begin + _count;
				const const_iterator pResult = ksl::detail::char_type_string_find_first_of(pBegin, pEnd, s, s + n);

				if (pResult != pEnd) {
					return (size_type) (pResult - _begin);
				}
			}
			return npos;
		}

		KOB_FUNC_CONSTEXPR size_type find_first_of(const T* s, size_type pos = 0) const
		{
			return find_first_of(s, pos, (size_type)char_strlen(s));
		}

		KOB_FUNC_CONSTEXPR size_type find_last_of(basic_string_view sw, size_type pos = npos) const KOB_NOEXCEPT
		{
			return find_last_of(sw._begin, pos, sw._count);
		}

		KOB_FUNC_CONSTEXPR size_type find_last_of(T c, size_type pos = npos) const KOB_NOEXCEPT
		{
			return rfind(c, pos);
		}

		KOB_FUNC_CONSTEXPR size_type find_last_of(const T* s, size_type pos, size_type n) const
		{
			// If n is zero or position is >= size, we return npos.
			if (KOB_LIKELY(_count)) {
				const value_type* const pEnd = _begin + std::min(_count - 1, pos) + 1;
				const value_type* const pResult = ksl::detail::char_type_string_rfind_first_of(pEnd, _begin, s, s + n);

				if (pResult != _begin) {
					return (size_type) ((pResult - 1) - _begin);
				}
			}
			return npos;
		}

		KOB_FUNC_CONSTEXPR size_type find_last_of(const T* s, size_type pos = npos) const
		{
			return find_last_of(s, pos, (size_type)char_strlen(s));
		}

		KOB_FUNC_CONSTEXPR size_type find_first_not_of(basic_string_view sw, size_type pos = 0) const KOB_NOEXCEPT
		{
			return find_first_not_of(sw._begin, pos, sw._count);
		}

		KOB_FUNC_CONSTEXPR size_type find_first_not_of(T c, size_type pos = 0) const KOB_NOEXCEPT
		{
			if (KOB_LIKELY(pos <= _count))
			{
				const auto pEnd = _begin + _count;
				const const_iterator pResult = ksl::detail::char_type_string_find_first_not_of(_begin + pos, pEnd, &c, &c + 1);

				if (pResult != pEnd)
					return (size_type)(pResult - _begin);
			}
			return npos;
		}

		KOB_FUNC_CONSTEXPR size_type find_first_not_of(const T* s, size_type pos, size_type n) const
		{
			if (KOB_LIKELY(pos <= _count))
			{
				const auto pEnd = _begin + _count;
				const const_iterator pResult = ksl::detail::char_type_string_find_first_not_of(_begin + pos, pEnd, s, s + n);

				if (pResult != pEnd)
					return (size_type)(pResult - _begin);
			}
			return npos;
		}

		KOB_FUNC_CONSTEXPR size_type find_first_not_of(const T* s, size_type pos = 0) const
		{
			return find_first_not_of(s, pos, (size_type)ksl::detail::char_strlen(s));
		}

		KOB_FUNC_CONSTEXPR size_type find_last_not_of(basic_string_view sw, size_type pos = npos) const KOB_NOEXCEPT
		{
			return find_last_not_of(sw._begin, pos, sw._count);
		}

		KOB_FUNC_CONSTEXPR size_type find_last_not_of(T c, size_type pos = npos) const KOB_NOEXCEPT
		{
			if (KOB_LIKELY(_count)) {
				const value_type* const pEnd = _begin + std::min(_count - 1, pos) + 1;
				const value_type* const pResult = ksl::detail::char_type_string_rfind_first_not_of(pEnd, _begin, &c, &c + 1);

				if (pResult != _begin)
					return (size_type)((pResult - 1) - _begin);
			}
			return npos;
		}

		KOB_FUNC_CONSTEXPR size_type find_last_not_of(const T* s, size_type pos, size_type n) const
		{
			if (KOB_LIKELY(_count))
			{
				const value_type* const pEnd = _begin + std::min(_count - 1, pos) + 1;
				const value_type* const pResult = ksl::detail::char_type_string_rfind_first_not_of(pEnd, _begin, s, s + n);

				if (pResult != _begin)
					return (size_type)((pResult - 1) - _begin);
			}
			return npos;
		}

		KOB_FUNC_CONSTEXPR size_type find_last_not_of(const T* s, size_type pos = npos) const
		{
			return find_last_not_of(s, pos, (size_type)ksl::detail::char_strlen(s));
		}

		// start_with
		KOB_FUNC_CONSTEXPR bool start_with(basic_string_view x) const KOB_NOEXCEPT
		{
			return (size() >= x.size()) && (compare(0, x.size(), x) == 0);
		}

		KOB_FUNC_CONSTEXPR bool start_with_icase(basic_string_view x) const KOB_NOEXCEPT
		{
			return (size() >= x.size()) && (compare_icase(0, x.size(), x) == 0);
		}

		KOB_FUNC_CONSTEXPR bool start_with(T x) const KOB_NOEXCEPT
		{
			return start_with(basic_string_view(&x, 1));
		}

		KOB_FUNC_CONSTEXPR bool start_with_icase(T x) const KOB_NOEXCEPT
		{
			return start_with_icase(basic_string_view(&x, 1));
		}

		KOB_FUNC_CONSTEXPR bool start_with(const T* s) const
		{
			return start_with(basic_string_view(s));
		}

		KOB_FUNC_CONSTEXPR bool start_with_icase(const T* s) const
		{
			return start_with_icase(basic_string_view(s));
		}

		// end_with
		KOB_FUNC_CONSTEXPR bool end_with(basic_string_view x) const KOB_NOEXCEPT
		{
			return (size() >= x.size()) && (compare(size() - x.size(), npos, x) == 0);
		}

		KOB_FUNC_CONSTEXPR bool end_with_icase(basic_string_view x) const KOB_NOEXCEPT
		{
			return (size() >= x.size()) && (compare_icase(size() - x.size(), npos, x) == 0);
		}

		KOB_FUNC_CONSTEXPR bool end_with(T x) const KOB_NOEXCEPT
		{
			return end_with(basic_string_view(&x, 1));
		}

		KOB_FUNC_CONSTEXPR bool end_with_icase(T x) const KOB_NOEXCEPT
		{
			return end_with_icase(basic_string_view(&x, 1));
		}

		KOB_FUNC_CONSTEXPR bool end_with(const T* s) const
		{
			return end_with(basic_string_view(s));
		}

		KOB_FUNC_CONSTEXPR bool end_with_icase(const T* s) const
		{
			return end_with_icase(basic_string_view(s));
		}
	};


	// global operators

    template <typename CharT>
    inline KOB_FUNC_CONSTEXPR bool operator==(basic_string_view<CharT> lhs, const CharT* rhsStr)
    {
        basic_string_view<CharT> rhs(rhsStr);
        return (lhs.size() == rhs.size()) && (lhs.compare(rhs) == 0);
    }

    template <typename CharT>
    inline KOB_FUNC_CONSTEXPR bool operator==(const CharT* lhsStr, basic_string_view<CharT> rhs)
    {
        basic_string_view<CharT> lhs(lhsStr);
        return (lhs.size() == rhs.size()) && (lhs.compare(rhs) == 0);
    }

	template <typename CharT>
	inline KOB_FUNC_CONSTEXPR bool operator==(basic_string_view<CharT> lhs, basic_string_view<CharT> rhs)
	{
		return (lhs.size() == rhs.size()) && (lhs.compare(rhs) == 0);
	}

	template <typename CharT>
	inline KOB_FUNC_CONSTEXPR bool operator!=(basic_string_view<CharT> lhs, basic_string_view<CharT> rhs)
	{
		return !(lhs == rhs);
	}

	template <typename CharT>
	inline KOB_FUNC_CONSTEXPR bool operator!=(basic_string_view<CharT> lhs, const CharT* rhsStr)
	{
		basic_string_view<CharT> rhs(rhsStr);
		return !(lhs == rhs);
	}

	template <typename CharT>
	inline KOB_FUNC_CONSTEXPR bool operator!=(const CharT* lhsStr, basic_string_view<CharT> rhs)
	{
		basic_string_view<CharT> lhs(lhsStr);
		return !(lhs == rhs);
	}

	template <class CharT>
	inline KOB_FUNC_CONSTEXPR bool operator<(basic_string_view<CharT> lhs, basic_string_view<CharT> rhs)
	{
		return lhs.compare(rhs) < 0;
	}

	template <class CharT>
	inline KOB_FUNC_CONSTEXPR bool operator<=(basic_string_view<CharT> lhs, basic_string_view<CharT> rhs)
	{
		return !(rhs < lhs);
	}

	template <class CharT>
	inline KOB_FUNC_CONSTEXPR bool operator>(basic_string_view<CharT> lhs, basic_string_view<CharT> rhs)
	{
		return rhs < lhs;
	}

	template <class CharT>
	inline KOB_FUNC_CONSTEXPR bool operator>=(basic_string_view<CharT> lhs, basic_string_view<CharT> rhs)
	{
		return !(lhs < rhs);
	}

	template <class T>
	class basic_cstring_view : public basic_string_view<T> {
	public:
		typedef basic_string_view<T>						base_type;
		typedef basic_cstring_view<T>                       this_type;
		typedef T 											value_type;
		typedef T* 											pointer;
		typedef const T* 									const_pointer;
		typedef T& 											reference;
		typedef const T& 									const_reference;
		typedef T* 											iterator;
		typedef const T* 									const_iterator;
		typedef std::reverse_iterator<iterator> 			reverse_iterator;
		typedef std::reverse_iterator<const_iterator> 	    const_reverse_iterator;
		typedef size_t 										size_type;
		typedef ptrdiff_t 									difference_type;

	public:
		explicit  basic_cstring_view(const T *ptr)
		: basic_string_view<T>(ptr)
		{

		}

		basic_cstring_view& operator = (const T *ptr)
		{
			basic_cstring_view dummy(ptr);
			this = dummy;
			return *this;
		}

		const_pointer c_str() const
		{
			return base_type::data();
		}
	};
	// string_view / wstring_view 
	typedef basic_string_view<char> string_view;
	typedef basic_string_view<wchar_t> wstring_view;

	typedef basic_cstring_view<char> cstring_view;
	typedef basic_cstring_view<wchar_t> wcstring_view;


	// C++17 string types
	typedef basic_string_view<char8_t> u8string_view;  // Actually not a C++17 type, but added for consistency.
	typedef basic_string_view<char16_t> u16string_view;
	typedef basic_string_view<char32_t> u32string_view;


	/// hash<string_view>
	///
	/// We provide EASTL hash function objects for use in hash table containers.
	///
	/// Example usage:
	///    #include <EASTL/hash_set.h>
	///    hash_set<string_view> stringHashSet;
	///
	template <typename T> struct hash;

	template<> struct hash<string_view> {
		size_t operator()(const string_view& x) const
		{
			string_view::const_iterator p = x.cbegin();
			string_view::const_iterator end = x.cend();
			uint32_t result = 2166136261U; // We implement an FNV-like string hash.
			while (p != end)
				result = (result * 16777619) ^ (uint8_t)*p++;
			return (size_t)result;
		}
	};

	template<> struct hash<u16string_view> {
		size_t operator()(const u16string_view& x) const
		{
			u16string_view::const_iterator p = x.cbegin();
			u16string_view::const_iterator end = x.cend();
			uint32_t result = 2166136261U;
			while (p != end)
				result = (result * 16777619) ^ (uint16_t)*p++;
			return (size_t)result;
		}
	};

	template<> struct hash<u32string_view> {
		size_t operator()(const u32string_view& x) const
		{
			u32string_view::const_iterator p = x.cbegin();
			u32string_view::const_iterator end = x.cend();
			uint32_t result = 2166136261U;
			while (p != end)
				result = (result * 16777619) ^ (uint32_t)*p++;
			return (size_t)result;
		}
	};


} // namespace ksl

#endif // KOB_KSL_STRING_VIEW_H_
