#include <kob/strings/token.h>
#include <kob/strings/numeric_parser.h>
#include <kob/ksl/ascii.h>


namespace kob {


token::token(bool isIgnore) : _ignored(isIgnore)
{
}


token::~token()
{
}


bool token::start(char c, std::istream& istr)
{
	_value = c;
	return false;
}


bool token::finish(std::istream& istr)
{
	return true;
}


token_type token::token_class() const
{
	return token_type::tt_invalid;
}

		
std::string token::as_string() const
{
	return _value;
}


bool token::as_int64(int64_t &rc) const
{
	return strings::str_to_int(_value, rc);
}


bool token::as_uint64(uint64_t &res) const
{
	return strings::parse_int(_value, res);
}


bool token::as_int(int &rc) const
{
    return  strings::parse_int(_value, rc);
}


bool token::as_uint(unsigned int &rc) const
{
	return  strings::parse_int(_value, rc);
}


bool token::as_float(double &rc) const
{
    return  kob::strings::parse_double(_value, rc);
}


char token::as_char() const
{
	return _value.empty() ? 0 : _value[0];
}


void token::ignore(bool isIgnored)
{
	_ignored = isIgnored;
}

bool token::ignored() const
{
	return _ignored;
}


invalid_token::invalid_token()
{
}


invalid_token::~invalid_token()
{
}


token_type invalid_token::token_class() const
{
	return token_type::tt_invalid;
}


eof_token::eof_token()
{
}


eof_token::~eof_token()
{
}


token_type eof_token::token_class() const
{
	return token_type::tt_eof;
}


whitespace_token::whitespace_token() : token(true)
{
}


whitespace_token::~whitespace_token()
{
}


token_type whitespace_token::token_class() const
{
	return token_type::tt_whitespace;
}


bool whitespace_token::start(char c, std::istream& istr)
{
	if (ksl::ascii::is_space(c)) {
		_value = c;
		return true;
	}
	return false;
};


bool whitespace_token::finish(std::istream& istr)
{
	int c = istr.peek();
	while (ksl::ascii::is_space(c)) {
		istr.get();
		_value += (char) c;
		c = istr.peek();
	}
	return true;
}


} 