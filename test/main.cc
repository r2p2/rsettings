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
	std::string const ini_data = "key = value";
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
	std::string const ini_data = "key1 = value1\nkey2: value2";
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

bool test_tokenizer()
{
	std::string s = "aAaA:bBbB";

	RTokenizer tzr;
	tzr.add_rule("A", "A", "aA");
	tzr.add_rule("A", "S", "");
	tzr.add_rule("S", "S", " :");
	tzr.add_rule("S", "B", "Bb");
	tzr.add_rule("B", "B", "");
	tzr.start("A", s);

	RToken token = tzr.next();
	if (token.type() != "A" or token.value() != "aAaA")
	{
		std::cout << "expected aA got " << token.value() << std::endl;
		return false;
	}

	token = tzr.next();
	if (token.type() != "S" or token.value() != ":")
	{
		std::cout << "expected : got " << token.value() << std::endl;
		return false;
	}

	token = tzr.next();
	if (token.type() != "B" or token.value() != "bBbB")
	{
		std::cout << "expected Bb got " << token.value() << std::endl;
		return false;
	}

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

	if (not test_tokenizer())
		return 4;

	if (not test_parse_single_variable())
		return 5;

	if (not test_parse_two_variables())
		return 6;

	return 0;
}
