#ifndef KOB_STRINGS_TOKENIZER_H_
#define KOB_STRINGS_TOKENIZER_H_
#include <kob/ksl/string_view.h>
#include <vector>
#include <string>
#include <cstddef>
#include <assert.h>

/*!
* This is not the most efficent way to split the string,
* eg. map the spliters two a ascii map [1-255] and index it 
* directly will be more quick than this way.
*/
namespace kob {

/*!
* A simple basic_tokenizer that splits a string into
* tokens, which are separated by separator characters.
* An iterator is used to iterate over all tokens.
*/
enum class token_policy{
	tp_non = 0,
	tp_ignore_empty = 1,
	tp_trim = 2,
	tp_ignore_and_trim = 3

};

KOB_FORCE_INLINE bool is_option_non(token_policy tp)
{
	return tp == token_policy::tp_non;
}

KOB_FORCE_INLINE bool is_option_trim(token_policy tp)
{
	return tp == token_policy::tp_trim || tp == token_policy::tp_ignore_and_trim;
}

KOB_FORCE_INLINE bool is_option_ignore(token_policy tp)
{
	return tp == token_policy::tp_ignore_empty || tp == token_policy::tp_ignore_and_trim;
}

template <typename Char>
class  basic_tokenizer {
public:
	typedef std::basic_string<Char>                        value_type;
	typedef ksl::basic_string_view<Char>                   view_type;
	typedef typename std::vector<value_type>               token_vec;
	typedef typename token_vec::const_iterator             const_iterator;
	typedef typename token_vec::size_type                  size_type;

	static const typename token_vec::size_type npos = static_cast<typename token_vec::size_type>(-1);

	basic_tokenizer(const view_type& str,
				const view_type& separators,
			  	token_policy options = token_policy::tp_non);

	~basic_tokenizer(){}


	const_iterator begin() const;
	const_iterator end() const;

	const value_type& operator[](size_type index) const;
	value_type& operator[](size_type index);

	bool has(const view_type& token) const
	{
		size_type index = find(token);
		return index != npos;
	}

	size_type find(const view_type & token, size_type pos = 0) const;

	size_type replace(const view_type& oldToken,
						const view_type& newToken,
						size_type pos = 0)
	{
		size_type cnt = 0;
		size_type index = find(oldToken, pos);
		while(index != npos) {
			cnt++;
			_tokens[index] = value_type(newToken.data(), newToken.size());
			index = find(oldToken, index);
		}
		return cnt;
	}

	size_type count() const;


	size_type count(const view_type& token) const;
		

private:
	basic_tokenizer(const basic_tokenizer&);
	basic_tokenizer& operator = (const basic_tokenizer&);

	void trim(std::string& token);

	token_vec _tokens;
};


//
// inlines
//

template <typename Char>
KOB_FORCE_INLINE typename basic_tokenizer<Char>::const_iterator basic_tokenizer<Char>::begin() const
{
	return _tokens.begin();
}

template <typename Char>
KOB_FORCE_INLINE typename basic_tokenizer<Char>::const_iterator basic_tokenizer<Char>::end() const
{
	return _tokens.end();
}

template <typename Char>
KOB_FORCE_INLINE
typename basic_tokenizer<Char>::value_type&
basic_tokenizer<Char>::operator[](basic_tokenizer<Char>::size_type index)
{
	if (KOB_UNLIKELY(index >= _tokens.size())) {
		KOB_ASSERT_MSG(false, "over flow");
	}
	return _tokens[index];
}

template <typename Char>
KOB_FORCE_INLINE
const typename basic_tokenizer<Char>::value_type&
basic_tokenizer<Char>::operator[](typename basic_tokenizer<Char>::size_type index) const
{
	if (index >= _tokens.size()) {
		KOB_ASSERT_MSG(false, "over flow");
	}
	return _tokens[index];
}

template <typename Char>
KOB_FORCE_INLINE typename basic_tokenizer<Char>::size_type basic_tokenizer<Char>::count() const
{
	return _tokens.size();
}

template <typename Char>
basic_tokenizer<Char>::basic_tokenizer(const typename basic_tokenizer<Char>::view_type& str, const typename basic_tokenizer<Char>::view_type& separators, token_policy options)
{
	typename basic_tokenizer<Char>::view_type::const_iterator it = str.begin();
	typename basic_tokenizer<Char>::view_type::const_iterator iend = str.end();
	bool doTrim = is_option_trim(options);
	bool ignoreEmpty = is_option_ignore(options);
	bool lastToken = false;
	typename basic_tokenizer<Char>::view_type::const_iterator tokenBegin = it;
	typename basic_tokenizer<Char>::view_type token;
	for (; it != iend; ++it) {
		if (separators.find(*it) != basic_tokenizer<Char>::view_type::npos) {
			token = basic_tokenizer<Char>::view_type(tokenBegin, it);
			if (doTrim) {
				token.trim();
			}
			if (!token.empty() || !ignoreEmpty) {
				_tokens.push_back(basic_tokenizer<Char>::value_type(token.data(), token.size()));
			}
			if (!ignoreEmpty) {
				lastToken = true;
			}
			tokenBegin = it +  1;
		} else {
			lastToken = false;
		}
	}
	token = basic_tokenizer<Char>::view_type(tokenBegin, it);
	if (!token.empty()) {
		if (doTrim) {
			token.trim();
		}
		if (!token.empty() || !ignoreEmpty) {
			_tokens.push_back(basic_tokenizer<Char>::value_type(token.data(), token.size()));
		}
	} else if (lastToken)  {
		_tokens.push_back(basic_tokenizer<Char>::value_type());
	}
}

template <typename Char>
typename basic_tokenizer<Char>::size_type
basic_tokenizer<Char>::count(const typename basic_tokenizer<Char>::view_type& token) const
{
	typename basic_tokenizer<Char>::size_type result = 0;
	typename basic_tokenizer<Char>::const_iterator it = _tokens.begin();
	while (it != _tokens.end()) {
		typename basic_tokenizer<Char>::view_type tv(*it);
		if(tv == token) {
			result++;
		}
		it++;
	}
	return result;
}

template <typename Char>
typename basic_tokenizer<Char>::size_type
basic_tokenizer<Char>::find(const typename basic_tokenizer<Char>::view_type& token,
                        typename basic_tokenizer<Char>::size_type pos) const
{
	if(pos > count()) {
		return basic_tokenizer<Char>::npos;
	}

	typename basic_tokenizer<Char>::size_type result = 0;
	typename basic_tokenizer<Char>::const_iterator it = _tokens.begin();
	it += pos;
	while (it != _tokens.end()) {
		typename basic_tokenizer<Char>::view_type tv(*it);
		if(tv == token) {
			return result;
		}
		it++;
		result++;
	}
	return basic_tokenizer<Char>::npos;
}

typedef basic_tokenizer<char> tokenizer;
typedef basic_tokenizer<wchar_t> wtokenizer;

} //namespace kob

#endif //KOB_STRINGS_TOKENIZER_H_
