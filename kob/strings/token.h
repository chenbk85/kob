#ifndef KOB_STRINGS_TOKEN_H_
#define KOB_STRINGS_TOKEN_H_

#include <istream>
#include <cstdint>
#include <string>

namespace kob {


enum class token_type {
	tt_identifier,
	tt_keyword,
	tt_separator,
	tt_operator,
	tt_string_literal,
	tt_char_literal,
	tt_integer_literal,
	tt_long_literal,
	tt_float_literal,
	tt_double_literal,
	tt_comment,
	tt_sepcial_comment,
	tt_preprocessor,
	tt_whitespace,
	tt_eof,
	tt_invalid,
	tt_user
};

class token {
public:

	token(bool ignore = false);


	virtual ~token();


	virtual bool start(char c, std::istream& istr);



	virtual bool finish(std::istream& istr);

	virtual token_type token_class() const;
		

	const std::string& token_string() const;
	
	/*!
	* @return a string representation of the token.
	*/
	virtual std::string as_string() const;

	/*!
	* @returna 64-bit integer representation of the token.
	*/
	virtual bool as_int64(int64_t &rc) const;
      
	/*!
	* @returna an unsigned 64-bit integer representation of the token.
	*/
	virtual bool as_uint64(uint64_t &res) const;
 
	/*!
	* @returna an integer representation of the token.
	*/
	virtual bool as_int(int &rc) const;

	/*!
	* @returna an unsigned integer representation of the token.
	*/
	virtual bool as_uint(unsigned int &rc) const;

	/*!
	* @returna a floating-point representation of the token.
	*/
	virtual bool as_float(double &rc) const;

	/*!
	* @returna a char representation of the token.
	*/
	virtual char as_char() const;

	/*!
	* @returna true iff the token has the given class.
	*/
	bool is(token_type tokenClass) const;
	
	/*!
	* If ignored is true, the token will be marked
	* as ignorable, which means that next() will
	* not return it.
	* If ignored is false, the token will be marked
	* as acceptable, which means that next() will
	* return it.
	*/
	void ignore(bool ignored);

	/*!
	* @returna if the token is ignored or not
	*/
	bool ignored() const;

protected:
	std::string _value;
	bool		_ignored;
	
private:
	token(const token&);
	token& operator = (const token&);
};

/*!
* This token class is used for signaling that
* an invalid character sequence has been encountered
* in the input stream.
*/
class  invalid_token: public token {
public:
	invalid_token();
	~invalid_token();
	token_type token_class() const;
};


/*!
* This token class is used to signal the
* end of the input stream.
*/
class  eof_token: public token {
public:
	eof_token();
	~eof_token();
	token_type token_class() const;
};



/*!
* This pseudo token class is used to eat
* up whitespace in between real tokens.
*/
class  whitespace_token: public token {
public:
	whitespace_token();
	~whitespace_token();
	token_type token_class() const;
	bool start(char c, std::istream& istr);
	bool finish(std::istream& istr);
};


//
// inlines
//
inline const std::string& token::token_string() const
{
	return _value;
}


inline bool token::is(token_type cls) const
{
	return token_class() == cls;
}


} 


#endif //KOB_STRINGS_TOKEN_H_
