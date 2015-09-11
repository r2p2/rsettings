#include <rsettings/ini.hpp>
#include <gtest/gtest.h>

class IniTest : public ::testing::Test
{
protected:
	IniTest()
	: ::testing::Test()
	, m_generic_ini_data()
	{}

	virtual void SetUp()
	{
		m_generic_ini_data =
			"# Comments can be placed at the top.\n"
			"title=RSettings\n"
			"build=1337\n"
			"# Comments can be placed inside the root section\n"
			"# wrap over multiple lines.\n"
			"\n"
			"# Empty lines are ignored"
			"   \t \n"
			"# Even if there are some whitespace characters.\n"
			"[section I]\n"
			"# Comments also work in sections.\n"
			"section_variable=14.221\n"
			"boolean_int_true=1\n"
			"boolean_int_false=0\n"
			"boolean_word_true=true\n"
			"boolean_word_false=false\n"
			"\n"
			"[section II]\n"
			"special_characters=ピース\n";
	}

	std::string ini_data() const
	{
		return m_generic_ini_data;
	}

private:
	std::string m_generic_ini_data;
};

TEST_F(IniTest, DefaultContructor)
{
	rsettings::Ini const settings;

	EXPECT_TRUE(settings.keys().empty());
	EXPECT_TRUE(settings.groups().empty());
}

TEST_F(IniTest, DefaultValues)
{
	rsettings::Ini settings;
	settings.parse(ini_data());

	EXPECT_STREQ("default", settings.get<std::string>("does_not_exist",
				"default").c_str());
	EXPECT_EQ(1337, settings.get<int>("does_not_exist", 1337));
	EXPECT_EQ(true, settings.get<int>("does_not_exist", true));
	EXPECT_EQ(false, settings.get<int>("does_not_exist", false));
}

TEST_F(IniTest, GroupsAndKeys)
{
	rsettings::Ini settings;
	settings.parse(ini_data());

	EXPECT_EQ(2, settings.groups().size());
	EXPECT_EQ(2, settings.keys().size());

	settings.begin_group("section I");
	EXPECT_EQ(5, settings.keys().size());
}

TEST_F(IniTest, AddKeysToRoot)
{
	rsettings::Ini settings;
	settings.parse(ini_data());

	settings.update("global_str", "new_string");
	settings.update("global_int", 1337);

	EXPECT_STREQ("new_string", settings.get<std::string>("global_str",
				"default").c_str());
	EXPECT_EQ(1337, settings.get<int>("global_int", 0));
}

TEST_F(IniTest, Utf8)
{
	rsettings::Ini settings;
	settings.parse(ini_data());

	settings.begin_group("section II");

	EXPECT_STREQ("ピース", settings.get<std::string>("special_characters",
				"default").c_str());
}

TEST_F(IniTest, Boolean)
{
	rsettings::Ini settings;
	settings.parse(ini_data());

	settings.begin_group("section I");

	EXPECT_TRUE(settings.get<bool>("boolean_int_true", false));
	EXPECT_FALSE(settings.get<bool>("boolean_int_false", true));
	EXPECT_TRUE(settings.get<bool>("boolean_word_true", false));
	EXPECT_FALSE(settings.get<bool>("boolean_word_false", true));
}

TEST_F(IniTest, ResultOk)
{
	rsettings::Ini settings;
	rsettings::Result result = settings.parse(ini_data());

	EXPECT_TRUE(result.is_successful());
	EXPECT_TRUE(result);
}

TEST_F(IniTest, ResultFailEmptyGrpName)
{
	rsettings::Ini settings;
	rsettings::Result result = settings.parse("[]");

	EXPECT_FALSE(result.is_successful());
	EXPECT_FALSE(result);

	EXPECT_STREQ("Error in line 1: An empty string is not valid as group name.\n",
			result.reason().c_str());
}

int main(int argc, char **argv)
{
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
