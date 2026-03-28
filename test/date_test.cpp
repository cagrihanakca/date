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