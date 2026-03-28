#include "date.h"
#include "gtest/gtest.h"

using namespace cgr;

TEST(InvalidDateTest, Ctor)
{
    EXPECT_NO_THROW(Date::InvalidDate(Date::InvalidDate::Reason::DAY, "invalid day"));
}

TEST(InvalidDateTest, GetReason)
{
    using Date::InvalidDate::Reason::DAY;

    ASSERT_NO_THROW(Date::InvalidDate(DAY, "invalid day"));

    Date::InvalidDate ex{ DAY, "invalid day" };
    EXPECT_EQ(ex.GetReason(), DAY);
}

TEST(InvalidDateTest, What)
{
    using Date::InvalidDate::Reason::MONTH;

    ASSERT_NO_THROW(Date::InvalidDate(MONTH, "invalid month"));

    Date::InvalidDate ex{ MONTH, "invalid month" };
    EXPECT_STREQ(ex.what(), "invalid month");
}

TEST(CtorTest, DefaultCtor)
{
    ASSERT_NO_THROW(Date{});

    Date d;
    EXPECT_EQ(d.Day(), 1);
    EXPECT_EQ(d.Month(), 1);
    EXPECT_EQ(d.Year(), 1900);
}

TEST(CtorTest, DayMonYearCtor)
{
    ASSERT_NO_THROW(Date(12, 12, 2024));

    Date d{ 12, 12, 2024 };
    EXPECT_EQ(d.Day(), 12);
    EXPECT_EQ(d.Month(), 12);
    EXPECT_EQ(d.Year(), 2024);

    EXPECT_THROW(Date(0, 12, 2024), Date::InvalidDate);
    EXPECT_THROW(Date(32, 12, 2024), Date::InvalidDate);
    EXPECT_THROW(Date(12, 0, 2024), Date::InvalidDate);
    EXPECT_THROW(Date(12, 12, 1899), Date::InvalidDate);
    EXPECT_THROW(Date(31, 4, 2024), Date::InvalidDate);
    EXPECT_THROW(Date(30, 2, 2024), Date::InvalidDate);
    EXPECT_THROW(Date(29, 2, 2023), Date::InvalidDate);
}

TEST(CtorTest, CStringCtor)
{
    ASSERT_NO_THROW(Date{ "12/12/2024" });

    Date d{ "12/12/2024" };
    EXPECT_EQ(d.Day(), 12);
    EXPECT_EQ(d.Month(), 12);
    EXPECT_EQ(d.Year(), 2024);

    EXPECT_THROW(Date{ "0/12/2024" }, Date::InvalidDate);
    EXPECT_THROW(Date{ "32/12/2024" }, Date::InvalidDate);
    EXPECT_THROW(Date{ "12/0/2024" }, Date::InvalidDate);
    EXPECT_THROW(Date{ "12/12/1899" }, Date::InvalidDate);
    EXPECT_THROW(Date{ "31/4/2024" }, Date::InvalidDate);
    EXPECT_THROW(Date{ "30/2/2024" }, Date::InvalidDate);
    EXPECT_THROW(Date{ "29/2/2023" }, Date::InvalidDate);
    EXPECT_THROW(Date{ "2/12/2024" }, Date::InvalidDate);
    EXPECT_THROW(Date{ "12/2/2024" }, Date::InvalidDate);
    EXPECT_THROW(Date{ "12/12/20242" }, Date::InvalidDate);
    EXPECT_THROW(Date{ "12 12 2024" }, Date::InvalidDate);
    EXPECT_THROW(Date{ "12.12.2024" }, Date::InvalidDate);
    EXPECT_THROW(Date{ "aaaa12/12/2024" }, Date::InvalidDate);
    EXPECT_THROW(Date{ "12/bbbb12/2024" }, Date::InvalidDate);
    EXPECT_THROW(Date{ "12/12/ccccc2024" }, Date::InvalidDate);
}