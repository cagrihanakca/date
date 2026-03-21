#include <ctime>
#include <sstream>
#include <string>
#include "date.h"
#include "gtest/gtest.h"

using namespace pro;

TEST(ConstructorTest, DefaultCtor)
{
    ASSERT_NO_THROW(Date{});

    Date testDate;

    EXPECT_EQ(testDate.MonthDay(), 1);
    EXPECT_EQ(testDate.Month(), 1);
    EXPECT_EQ(testDate.Year(), Date::baseYear);
}

TEST(ConstructorTest, DayMonYearCtor)
{
    ASSERT_NO_THROW(Date(12, 12, 2024));

    Date testDate{ 12, 12, 2024 };

    EXPECT_EQ(testDate.MonthDay(), 12);
    EXPECT_EQ(testDate.Month(), 12);
    EXPECT_EQ(testDate.Year(), 2024);

    EXPECT_THROW(Date(38, 12, 2024), std::invalid_argument);
    EXPECT_THROW(Date(12, 65, 2024), std::invalid_argument);
    EXPECT_THROW(Date(12, 12, 1474), std::invalid_argument);
}

TEST(ConstructorTest, CStringCtor)
{
    ASSERT_NO_THROW(Date("12/12/2024"));

    Date testDate{ "12/12/2024" };

    EXPECT_EQ(testDate.MonthDay(), 12);
    EXPECT_EQ(testDate.Month(), 12);
    EXPECT_EQ(testDate.Year(), 2024);

    EXPECT_THROW(Date{ "38/12/2024" }, std::invalid_argument);
    EXPECT_THROW(Date{ "12/65/2024" }, std::invalid_argument);
    EXPECT_THROW(Date{ "12/12/1474" }, std::invalid_argument);
    EXPECT_THROW(Date{ "12 12 2024" }, std::invalid_argument);
    EXPECT_THROW(Date{ "12.12.2024" }, std::invalid_argument);
    EXPECT_THROW(Date{ "aaaa12/12/2024" }, std::invalid_argument);
    EXPECT_THROW(Date{ "12/bbbb12/2024" }, std::invalid_argument);
    EXPECT_THROW(Date{ "12/12/ccccc2024" }, std::invalid_argument);
}

TEST(ConstructorTest, StringCtor)
{
    using namespace std::literals::string_literals;

    ASSERT_NO_THROW(Date("12/12/2024"s));

    Date testDate{ "12/12/2024"s };

    EXPECT_EQ(testDate.MonthDay(), 12);
    EXPECT_EQ(testDate.Month(), 12);
    EXPECT_EQ(testDate.Year(), 2024);

    EXPECT_THROW(Date{ "38/12/2024"s }, std::invalid_argument);
    EXPECT_THROW(Date{ "12/65/2024"s }, std::invalid_argument);
    EXPECT_THROW(Date{ "12/12/1474"s }, std::invalid_argument);
    EXPECT_THROW(Date{ "12 12 2024"s }, std::invalid_argument);
    EXPECT_THROW(Date{ "12.12.2024"s }, std::invalid_argument);
    EXPECT_THROW(Date{ "aaaa12/12/2024"s }, std::invalid_argument);
    EXPECT_THROW(Date{ "12/bbbb12/2024"s }, std::invalid_argument);
    EXPECT_THROW(Date{ "12/12/ccccc2024"s }, std::invalid_argument);
}

TEST(ConstructorTest, CalendarTimeCtor)
{
    std::time_t timer{};
    std::time(&timer);

    ASSERT_NO_THROW(Date{ timer });

    Date testDate{ timer };
    const auto *tp{ std::localtime(&timer) };

    EXPECT_EQ(testDate.MonthDay(), tp->tm_mday);
    EXPECT_EQ(testDate.Month(), tp->tm_mon + 1);
    EXPECT_EQ(testDate.Year(), tp->tm_year + 1900);

    EXPECT_THROW(Date{ -1 }, std::runtime_error);
}

TEST(ConstructorTest, IstreamCtor)
{
    std::istringstream istr{ "12/12/2024" };

    ASSERT_NO_THROW(Date{ istr });

    istr.clear();
    istr.str("12/12/2024");

    Date testDate{ istr };

    EXPECT_EQ(testDate.MonthDay(), 12);
    EXPECT_EQ(testDate.Month(), 12);
    EXPECT_EQ(testDate.Year(), 2024);

    const std::vector<std::string> inputs{
        "38/12/2024", "12/65/2024", "12/12/1474", "12 12 2024", "12.12.2024", "aaaa12/12/2024", "12/bbbb12/2024",
        "12/12/ccccc2024"
    };
    for (const auto &in : inputs) {
        std::istringstream istr{ in };
        EXPECT_THROW(Date{ in }, std::invalid_argument);
    }
}