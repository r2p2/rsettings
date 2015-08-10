#pragma once

#include <map>
#include <vector>
#include <string>
#include <sstream>

#include <iostream>

class RSettings
{
	typedef std::map<std::string, std::string> map_str_str_t;
	typedef std::map<std::string, map_str_str_t> map_str_map_t;
public:
	RSettings()
	: _values()
	, _current_group()
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

			if (line[0] == '[')
			{
				size_t pos_bracket = line.find(']');
				std::string const grp = line.substr(1, pos_bracket-1);
				begin_group(grp);
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

		end_group();
	}

	std::string write()
	{
		return _pp_groups();
	}

	std::vector<std::string> keys(std::string const& group) const
	{
		std::vector<std::string> ks;

		map_str_map_t::const_iterator it_group;
		it_group = _values.find(group);

		if (it_group == _values.end())
		{
			return ks;
		}

		map_str_str_t::const_iterator it;

		for(it = it_group->second.begin();
				it != it_group->second.end(); ++it)
		{
			ks.push_back(it->first);
		}

		return ks;
	}

	std::vector<std::string> keys() const
	{
		return keys(_current_group);
	}

	std::vector<std::string> groups() const
	{
		std::vector<std::string> grps;

		map_str_map_t::const_iterator it;
		for (it = _values.begin(); it != _values.end(); ++it)
		{
			if (it->first.empty())
				continue;

			grps.push_back(it->first);
		}

		return grps;
	}

	template<class T>
	T get(std::string const& group, std::string const& key,
			T const& default_value) const
	{
		map_str_map_t::const_iterator it_group;
		it_group = _values.find(group);

		if (it_group == _values.end())
			return default_value;

		map_str_str_t const& map_in_grp = it_group->second;
		map_str_str_t::const_iterator it_key;
		it_key = map_in_grp.find(key);

		if (it_key == map_in_grp.end())
			return default_value;

		T type_element;
		std::stringstream ss(it_key->second);
		ss >> type_element;

		return type_element;
	}

	template<class T>
	T get(std::string const& key, T const& default_value) const
	{
		return get(_current_group, key, default_value);
	}

	template<class T>
	void update(std::string const& group, std::string const& key,
			T const& value)
	{
		std::stringstream ss;
		ss << value;

		_values[group][key] = ss.str();

	}

	template<class T>
	void update(std::string const& key, T const& value)
	{
		update(_current_group, key, value);
	}

	void begin_group(std::string const& group)
	{
		_current_group = group;
	}

	void end_group()
	{
		_current_group.clear();
	}

private:
	std::string _pp_groups()
	{
		std::string const grp_backup = _current_group;
		std::stringstream ss;

		end_group();

		_pp_current_group();

		std::vector<std::string> list_groups = groups();
		std::vector<std::string>::const_iterator it;
		for (it = list_groups.begin(); it != list_groups.end(); ++it)
		{
			std::string const& group = *it;

			if (group.empty())
			{
				continue;
			}

			ss << "[" << group << "]" << std::endl;

			begin_group(group);
			ss << _pp_current_group();
		}

		begin_group(grp_backup);
		return ss.str();
	}

	std::string _pp_current_group() const
	{
		std::stringstream ss;

		std::vector<std::string> const list_keys = keys();
		std::vector<std::string>::const_iterator  it;

		for (it = list_keys.begin(); it != list_keys.end(); ++it)
		{
			std::string const& key = *it;
			std::string const& val = get<std::string>(key, "");

			if (val.empty())
			{
				continue;
			}

			ss << key << "=" << val << std::endl;
		}

		return ss.str();
	}

private:
	map_str_map_t _values;
	std::string _current_group;
};
