#include "gtest/gtest.h"
#include "date.h"

using namespace pro;

TEST(ConstructorTest, DefaultCtor) {
    Date d;
    EXPECT_EQ(d.MonthDay(), 1);
    EXPECT_EQ(d.Month(), 1);
    EXPECT_EQ(d.Year(), Date::baseYear);
    EXPECT_NO_THROW(Date{});
}

TEST(ConstructorTest, DayMonYearCtor) {
    Date d{ 16, 12, 2024 };
    EXPECT_EQ(d.MonthDay(), 16);
    EXPECT_EQ(d.Month(), 12);
    EXPECT_EQ(d.Year(), 2024);
}