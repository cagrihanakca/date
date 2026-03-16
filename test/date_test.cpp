#include "gtest/gtest.h"
#include "date.h"

using namespace pro;

TEST(ConstructorTest, DefaultCtor) {
    Date d;
    EXPECT_EQ(d.MonthDay(), 1);
    EXPECT_EQ(d.Month(), 1);
    EXPECT_EQ(d.Year(), Date::baseYear);
}