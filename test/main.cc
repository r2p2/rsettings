#include <rsettings/rsettings.hpp>

bool test_init_rsettings()
{
	RSettings settings;

	if (not settings.keys().empty())
		return false;

	if (not settings.groups().empty())
		return false;

	return true;
}

bool test_access_default_values()
{
	std::string const default_string = "default";
	int const default_int = 1337;

	RSettings settings;

	std::string const string_value = settings.get<std::string>("global_str",
			default_string);

	if (not (string_value == default_string))
		return false;

	int const int_value = settings.get<int>("global_int", default_int);
	if (not (int_value == default_int))
		return false;

	if (not settings.keys().empty())
		return false;

	if (not settings.groups().empty())
		return false;

	return true;
}

bool test_add_global_keys()
{
	std::string const default_string = "default";
	int const default_int = 1337;

	std::string const update_string = "string_value";
	int const update_int = 42;

	RSettings settings;

	settings.update("global_str", update_string);
	settings.update("global_int", update_int);

	if (settings.keys().size() != 2)
		return false;

	if (not settings.groups().empty())
		return false;

	std::string const string_value = settings.get<std::string>("global_str",
			default_string);
	int const int_value = settings.get<int>("global_int", default_int);

	if (not (string_value == update_string))
		return false;

	if (not (int_value == update_int))
		return false;

	return true;
}

bool test_parse_single_variable()
{
	std::string const ini_data = "key=value";
	RSettings settings;

	settings.parse(ini_data);

	if (settings.keys().size() != 1)
		return false;

	if (not settings.groups().empty())
		return false;

	std::string value = settings.get<std::string>("key", "");
	if (not (value == "value"))
		return false;

	return true;
}

bool test_parse_two_variables()
{
	std::string const ini_data = "key1=value1\nkey2=value2";
	RSettings settings;

	settings.parse(ini_data);

	if (settings.keys().size() != 2)
		return false;

	if (not settings.groups().empty())
		return false;

	std::string value1 = settings.get<std::string>("key1", "");
	if (not (value1 == "value1"))
		return false;

	std::string value2 = settings.get<std::string>("key2", "");
	if (not (value2 == "value2"))
		return false;

	return true;
}

bool test_parse_comments()
{
	std::string const ini_data = ";one comment\n"
		"key1=value1\n"
		"# another one\n"
		"key2=value2\n"
		"# last but not least";
	RSettings settings;

	settings.parse(ini_data);

	if (settings.keys().size() != 2)
		return false;

	if (not settings.groups().empty())
		return false;

	std::string value1 = settings.get<std::string>("key1", "");
	if (not (value1 == "value1"))
		return false;

	std::string value2 = settings.get<std::string>("key2", "");
	if (not (value2 == "value2"))
		return false;

	return true;
}

bool test_parse_utf8()
{
	std::string const ini_data =
		"name=Müller\n"
		"forename=Strauß";
	RSettings settings;

	settings.parse(ini_data);

	if (settings.keys().size() != 2)
		return false;

	if (not settings.groups().empty())
		return false;

	std::string value1 = settings.get<std::string>("name", "");
	if (not (value1 == "Müller"))
		return false;

	std::string value2 = settings.get<std::string>("forename", "");
	if (not (value2 == "Strauß"))
		return false;

	return true;
}

bool test_parse_result()
{
	std::string const ini_data1 =
		"name=Müller\n"
		"forename=Strauß";

	std::string const ini_data2 =
		"name=Müller\n"
		"forename Strauß";

	std::string const ini_data3 =
		"[grp\n"
		"name=Müller\n"
		"forename=Strauß";

	RSettings settings;

	Result result1 = settings.parse(ini_data1);

	if (not result1)
		return false;

	Result result2 = settings.parse(ini_data2);
	if (result2)
		return false;

	Result result3 = settings.parse(ini_data3);
	if (result3)
		return false;

	return true;
}

bool test_parse_groups()
{
	std::string const ini_data =
		"r_key=r_val\n"
		"[grp1]\n"
		";start of the first group\n"
		"g1_key=g1_val\n"
		"[grp2]\n"
		"g2_key=g2_val\n"
		"g3_key=g3_val";
	RSettings settings;

	settings.parse(ini_data);
	if (settings.keys().size() != 1)
		return false;

	if (settings.groups().size() != 2)
		return false;

	std::string value1 = settings.get<std::string>("r_key", "");
	if (not (value1 == "r_val"))
		return false;

	settings.begin_group("grp1");

	if (settings.keys().size() != 1)
		return false;

	if (settings.groups().size() != 2)
		return false;

	std::string value2 = settings.get<std::string>("g1_key", "");
	if (not (value2 == "g1_val"))
		return false;

	settings.begin_group("grp2");

	if (settings.keys().size() != 2)
		return false;

	if (settings.groups().size() != 2)
		return false;

	std::string value3 = settings.get<std::string>("g2_key", "");
	if (not (value3 == "g2_val"))
		return false;

	std::string value4 = settings.get<std::string>("g3_key", "");
	if (not (value4 == "g3_val"))
		return false;

	return true;
}

bool test_parse_errors()
{
	std::string const ini_data =
		"r_key=r_val\n"
		"[]\n"
		";start of the first group\n"
		"g1_key=g1_val\n"
		"[grp2]\n"
		"g2_key=g2_val\n"
		"g3_key=g3_val";
	RSettings settings;

	Result res = settings.parse(ini_data);
	if (res.is_successful())
		return false;

	if (res.reason() != "Error in line 2: An empty string is not valid as "
			"group name.\n")
		return false;

	return true;
}

int main()
{
	if (not test_init_rsettings())
		return 1;

	if (not test_init_rsettings())
		return 2;

	if (not test_add_global_keys())
		return 3;

	if (not test_parse_single_variable())
		return 5;

	if (not test_parse_two_variables())
		return 6;

	if (not test_parse_comments())
		return 7;

	if (not test_parse_utf8())
		return 8;

	if (not test_parse_groups())
		return 9;

	if (not test_parse_result())
		return 10;

	if (not test_parse_errors())
		return 11;

	return 0;
}
