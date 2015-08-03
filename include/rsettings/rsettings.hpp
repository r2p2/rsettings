#pragma once

#include <map>
#include <vector>
#include <string>
#include <sstream>

#include <iostream>

class RSettings
{
public:
	RSettings()
	: _values()
	{
	}

	~RSettings()
	{
	}

	void parse(std::string const& ini)
	{
		std::stringstream ss(ini);
		std::string line;

		while(std::getline(ss, line))
		{
			if (line.empty())
			{
				continue;
			}

			if (line[0] == ';' or line[0] == '#')
			{
				continue;
			}

			size_t pos_assign = line.find('=');
			if (pos_assign != std::string::npos)
			{
				std::string const key = line.substr(0, pos_assign);
				std::string const val = line.substr(pos_assign + 1);

				update(key, val);
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
};
