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

int main()
{
	if (not test_init_rsettings)
		return 1;

	if (not test_init_rsettings)
		return 2;

	if (not test_add_global_keys())
		return 3;

	return 0;
}
