#pragma once

#include <string>
#include <sstream>

class Result
{
public:
	Result()
	: _reason()
	, _line_nr(0)
	{}

	Result(int line_nr, std::string const& reason)
	: _reason(reason)
	, _line_nr(line_nr)
	{}

	std::string reason() const
	{
		std::stringstream ss;
		ss << "Error in line " << _line_nr << ": " << _reason << std::endl;

		return ss.str();
	}

	bool is_successful() const
	{
		return _reason.empty();
	}

	operator bool() const
	{
		return is_successful();
	}

private:
	std::string const _reason;
	int const _line_nr;
};
