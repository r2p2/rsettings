#pragma once

#include <string>
#include <vector>

#include <iostream>

class RRule
{
public:
	RRule(std::string const& current, std::string const& next,
			std::string const& chars)
	: _current(current)
	, _next(next)
	, _chars(chars)
	{
	}

	std::string current() const
	{
		return _current;
	}

	std::string next() const
	{
		return _next;
	}

	bool contains(char c) const
	{
		return _chars.empty() or _chars.find(c) != std::string::npos;
	}

private:
	std::string _current;
	std::string _next;
	std::string _chars;
};

class RToken
{
public:
	RToken()
	: _type()
	, _it_start()
	, _it_end()
	{
	}

	RToken(std::string const& type,
			std::string::const_iterator const& it_start,
			std::string::const_iterator const& it_end)
	: _type(type)
	, _it_start(it_start)
	, _it_end(it_end)
	{
	}
	
	std::string type() const
	{
		return _type;
	}

	std::string value() const
	{
		return std::string(_it_start, _it_end);
	}

private:
	std::string                 _type;
	std::string::const_iterator _it_start;
	std::string::const_iterator _it_end;
};

class RTokenizer
{
public:
	RTokenizer()
	{
	}

	~RTokenizer()
	{
	}

	void start(std::string const& start_token, std::string const& data)
	{
		std::string tk_current = start_token;
		std::string::const_iterator it_start = data.begin();
		std::string::const_iterator it_end = it_start;

		while (true)
		{
			if (it_end == data.end())
			{
				_add_token(tk_current, it_start, it_end);
				_tk_it = _tokens.begin();
				return;
			}
			
			char const c_current = *it_end;
			std::string const tk_next = _check_next_token(tk_current, c_current);

			if (tk_current != tk_next)
			{
				if (it_start != it_end)
				{
					_add_token(tk_current, it_start, it_end);
					it_start = it_end;
				}

				tk_current = tk_next;
			}

			++it_end;
		}
	}

	bool has_next() const
	{
		return _tk_it != _tokens.end();
	}

	RToken next()
	{
		RToken token = *_tk_it;
		++_tk_it;
		return token;
	}

	void add_rule(std::string const& current, std::string const& next,
			std::string const& chars)
	{
		_rules.push_back(RRule(current, next, chars));
	}



private:
	std::string _check_next_token(std::string const& tk_current, char c) const
	{
		std::vector<RRule>::const_iterator it;
		for (it = _rules.begin(); it != _rules.end(); ++it)
		{
			RRule const& rule = *it;
			if (rule.current() == tk_current and rule.contains(c))
			{
				return rule.next();
			}
		}

		return tk_current;
	}

	void _add_token(std::string const& type,
			std::string::const_iterator const& it_start,
			std::string::const_iterator const& it_end)
	{
		_tokens.push_back(RToken(type, it_start, it_end));
	}

private:
	std::vector<RRule>                  _rules;
	std::vector<RToken>                 _tokens;
	std::vector<RToken>::const_iterator _tk_it;
};
