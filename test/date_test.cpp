#include "gtest/gtest.h"
#include "date.h"

using namespace pro;

TEST(ConstructorTest, DefaultCtor)
{
    Date testDate;

    EXPECT_EQ(testDate.MonthDay(), 1);
    EXPECT_EQ(testDate.Month(), 1);
    EXPECT_EQ(testDate.Year(), Date::baseYear);

    EXPECT_NO_THROW(Date{});
}

TEST(ConstructorTest, DayMonYearCtor)
{
    Date testDate{ 12, 12, 2024 };

    EXPECT_EQ(testDate.MonthDay(), 12);
    EXPECT_EQ(testDate.Month(), 12);
    EXPECT_EQ(testDate.Year(), 2024);

    EXPECT_NO_THROW(Date(12, 12, 2024));

    EXPECT_THROW(Date(38, 12, 2024), std::invalid_argument);
    EXPECT_THROW(Date(12, 65, 2024), std::invalid_argument);
    EXPECT_THROW(Date(12, 12, 1474), std::invalid_argument);
}

TEST(ConstructorTest, CStringCtor)
{
    Date testDate{ "12/12/2024" };

    EXPECT_EQ(testDate.MonthDay(), 12);
    EXPECT_EQ(testDate.Month(), 12);
    EXPECT_EQ(testDate.Year(), 2024);

    EXPECT_NO_THROW(Date("12/12/2024"));

    EXPECT_THROW(Date{ "38/12/2024" }, std::invalid_argument);
    EXPECT_THROW(Date{ "12/65/2024" }, std::invalid_argument);
    EXPECT_THROW(Date{ "12/12/1474" }, std::invalid_argument);
    EXPECT_THROW(Date{ "12 12 2024" }, std::invalid_argument);
    EXPECT_THROW(Date{ "12.12.2024" }, std::invalid_argument);
    EXPECT_THROW(Date{ "aaaa12/12/2024" }, std::invalid_argument);
    EXPECT_THROW(Date{ "12/bbbb12/2024" }, std::invalid_argument);
    EXPECT_THROW(Date{ "12/12/ccccc2024" }, std::invalid_argument);
}