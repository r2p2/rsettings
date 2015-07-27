#pragma once

#include "rtokenizer.hpp"

#include <map>
#include <vector>
#include <string>
#include <sstream>

class RSettings
{
public:
	RSettings()
	: _values()
	, _tokenizer()
	{
		_tokenizer.add_rule("root"      , "root"      , " \t\n");
		_tokenizer.add_rule("root"      , "comment"   , ";#");
		_tokenizer.add_rule("comment"   , "root"      , "\n");
		_tokenizer.add_rule("comment"   , "comment"   , "");
		_tokenizer.add_rule("root"      , "key"       , "");
		_tokenizer.add_rule("key"       , "separator" , " \t=:");
		_tokenizer.add_rule("separator" , "separator" , " \t=:");
		_tokenizer.add_rule("separator" , "value"     , "");
		_tokenizer.add_rule("value"     , "value"     , "");
		_tokenizer.add_rule("value"     , "root"      , "\n");
	}

	~RSettings()
	{
	}

	void parse(std::string const& ini)
	{
		enum State { Root, ReadValue } state;
		
		_tokenizer.start("root", ini);

		RToken token_last_key;

		state = Root;
		while (_tokenizer.has_next())
		{
			RToken token = _tokenizer.next();

			if (state == Root)
			{
				if (token.type() == "key")
				{
					token_last_key = token;
					state = ReadValue;
				}
			}
			else if (state == ReadValue)
			{
				if (token.type() == "value")
				{
					update(token_last_key.value(), token.value());
					state = Root;
				}
			}
		}
	}

	std::vector<std::string> keys() const
	{
		std::vector<std::string> ks;
		std::map<std::string, std::string>::const_iterator it;
		
		for(it = _values.begin(); it != _values.end(); ++it)
		{
			ks.push_back(it->first);
		}

		return ks;
	}

	std::vector<std::string> groups() const
	{
		return std::vector<std::string>();
	}

	template<class T>
	T get(std::string const& key, T const& default_value) const
	{
		std::map<std::string, std::string>::const_iterator it;
		it = _values.find(key);
		if (it == _values.end())
			return default_value;

		T type_element;
		std::stringstream ss(it->second);
		ss >> type_element;
		
		return type_element;
	}

	template<class T>
	void update(std::string const& key, T const& value)
	{
		std::stringstream ss;
		ss << value;

		_values[key] = ss.str();
	}

private:
	std::map<std::string, std::string> _values;
	RTokenizer _tokenizer;
};
