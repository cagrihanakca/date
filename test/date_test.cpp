#include <ctime>
#include <limits>
#include <sstream>
#include <string>
#include <vector>

#include <cgr/date.h>
#include <gtest/gtest.h>

using namespace cgr;

TEST(DateErrorTest, Ctor)
{
    EXPECT_NO_THROW(Date::DateError(Date::DateError::Reason::DAY, "invalid day"));
}

TEST(DateErrorTest, GetReason)
{
    using Date::DateError::Reason::DAY;

    ASSERT_NO_THROW(Date::DateError(DAY, "invalid day"));
    Date::DateError ex{ DAY, "invalid day" };
    EXPECT_EQ(ex.GetReason(), DAY);
}

TEST(DateErrorTest, What)
{
    using Date::DateError::Reason::MONTH;

    ASSERT_NO_THROW(Date::DateError(MONTH, "invalid month"));
    Date::DateError ex{ MONTH, "invalid month" };
    EXPECT_STREQ(ex.what(), "invalid month");
}

TEST(ISOWeekTest, Equality)
{
    EXPECT_EQ(Date{ "12/12/2024" }.WeekOfYear(), Date{ "12/12/2024" }.WeekOfYear());
    EXPECT_NE(Date{ "12/12/2024" }.WeekOfYear(), Date{ "24/12/2024" }.WeekOfYear());
}

TEST(StaticFactoryTest, Today)
{
    std::time_t timer{};
    std::time(&timer);
    const auto *timePtr{ localtime(&timer) };
    Date today{ Date::Today() };
    EXPECT_EQ(today.Day(), timePtr->tm_mday);
    EXPECT_EQ(today.Month(), timePtr->tm_mon + 1);
    EXPECT_EQ(today.Year(), timePtr->tm_year + 1900);
}

TEST(StaticFactoryTest, RandomDate)
{
    ASSERT_NO_THROW(static_cast<void>(Date::RandomDate(1950, 2050)));

    EXPECT_GE(Date::RandomDate().Year(), Date::MIN_YEAR);
    EXPECT_LE(Date::RandomDate().Year(), Date::Today().Year());
    EXPECT_GE(Date::RandomDate(1950, 2000).Year(), 1950);
    EXPECT_LE(Date::RandomDate(1950, 2000).Year(), 2000);

    EXPECT_THROW(static_cast<void>(Date::RandomDate(Date::MIN_YEAR - 1, 2000)), Date::DateError);
    EXPECT_THROW(static_cast<void>(Date::RandomDate(1950, Date::MIN_YEAR - 1)), Date::DateError);
    EXPECT_THROW(static_cast<void>(Date::RandomDate(Date::MAX_YEAR + 1, 2000)), Date::DateError);
    EXPECT_THROW(static_cast<void>(Date::RandomDate(1950, Date::MAX_YEAR + 1)), Date::DateError);
    EXPECT_THROW(static_cast<void>(Date::RandomDate(2000, 1950)), Date::DateError);
}

TEST(CtorTest, DefaultCtor)
{
    ASSERT_NO_THROW(Date{});
    Date d;
    EXPECT_EQ(d.Day(), 1);
    EXPECT_EQ(d.Month(), 1);
    EXPECT_EQ(d.Year(), Date::MIN_YEAR);
}

TEST(CtorTest, DayMonYearCtor)
{
    ASSERT_NO_THROW(Date(12, 12, 2024));
    Date d{ 12, 12, 2024 };
    EXPECT_EQ(d.Day(), 12);
    EXPECT_EQ(d.Month(), 12);
    EXPECT_EQ(d.Year(), 2024);

    EXPECT_THROW(Date(0, 12, 2024), Date::DateError);
    EXPECT_THROW(Date(32, 12, 2024), Date::DateError);
    EXPECT_THROW(Date(12, 0, 2024), Date::DateError);
    EXPECT_THROW(Date(12, 12, Date::MIN_YEAR - 1), Date::DateError);
    EXPECT_THROW(Date(12, 12, Date::MAX_YEAR + 1), Date::DateError);
    EXPECT_THROW(Date(31, 4, 2024), Date::DateError);
    EXPECT_THROW(Date(30, 2, 2024), Date::DateError);
    EXPECT_THROW(Date(29, 2, 2023), Date::DateError);
}

TEST(CtorTest, CStringCtor)
{
    ASSERT_NO_THROW(Date{ "12/12/2024" });
    Date d{ "12/12/2024" };
    EXPECT_EQ(d.Day(), 12);
    EXPECT_EQ(d.Month(), 12);
    EXPECT_EQ(d.Year(), 2024);

    EXPECT_THROW(Date{ nullptr }, Date::DateError);
    EXPECT_THROW(Date{ "0/12/2024" }, Date::DateError);
    EXPECT_THROW(Date{ "32/12/2024" }, Date::DateError);
    EXPECT_THROW(Date{ "12/0/2024" }, Date::DateError);
    EXPECT_THROW(Date{ "12/12/1899" }, Date::DateError);
    EXPECT_THROW(Date{ "12/12/10000" }, Date::DateError);
    EXPECT_THROW(Date{ "31/4/2024" }, Date::DateError);
    EXPECT_THROW(Date{ "30/2/2024" }, Date::DateError);
    EXPECT_THROW(Date{ "29/2/2023" }, Date::DateError);
    EXPECT_THROW(Date{ "2/12/2024" }, Date::DateError);
    EXPECT_THROW(Date{ "12/2/2024" }, Date::DateError);
    EXPECT_THROW(Date{ "12 12 2024" }, Date::DateError);
    EXPECT_THROW(Date{ "12.12.2024" }, Date::DateError);
    EXPECT_THROW(Date{ "aaaa12/12/2024" }, Date::DateError);
    EXPECT_THROW(Date{ "12/bbbb12/2024" }, Date::DateError);
    EXPECT_THROW(Date{ "12/12/ccccc2024" }, Date::DateError);
}

TEST(CtorTest, StringCtor)
{
    using namespace std::literals::string_literals;

    ASSERT_NO_THROW(Date{ "12/12/2024"s });
    Date d{ "12/12/2024"s };
    EXPECT_EQ(d.Day(), 12);
    EXPECT_EQ(d.Month(), 12);
    EXPECT_EQ(d.Year(), 2024);

    EXPECT_THROW(Date{ "0/12/2024"s }, Date::DateError);
    EXPECT_THROW(Date{ "32/12/2024"s }, Date::DateError);
    EXPECT_THROW(Date{ "12/0/2024"s }, Date::DateError);
    EXPECT_THROW(Date{ "12/12/1899"s }, Date::DateError);
    EXPECT_THROW(Date{ "12/12/10000"s }, Date::DateError);
    EXPECT_THROW(Date{ "31/4/2024"s }, Date::DateError);
    EXPECT_THROW(Date{ "30/2/2024"s }, Date::DateError);
    EXPECT_THROW(Date{ "29/2/2023"s }, Date::DateError);
    EXPECT_THROW(Date{ "2/12/2024"s }, Date::DateError);
    EXPECT_THROW(Date{ "12/2/2024"s }, Date::DateError);
    EXPECT_THROW(Date{ "12/12/20242"s }, Date::DateError);
    EXPECT_THROW(Date{ "12 12 2024"s }, Date::DateError);
    EXPECT_THROW(Date{ "12.12.2024"s }, Date::DateError);
    EXPECT_THROW(Date{ "aaaa12/12/2024"s }, Date::DateError);
    EXPECT_THROW(Date{ "12/bbbb12/2024"s }, Date::DateError);
    EXPECT_THROW(Date{ "12/12/ccccc2024"s }, Date::DateError);
}

TEST(CtorTest, CalendarTimeCtor)
{
    std::time_t timer{};
    std::time(&timer);
    const auto *timePtr{ localtime(&timer) };
    Date d{ timer };
    EXPECT_EQ(d.Day(), timePtr->tm_mday);
    EXPECT_EQ(d.Month(), timePtr->tm_mon + 1);
    EXPECT_EQ(d.Year(), timePtr->tm_year + 1900);
}

TEST(CtorTest, IStreamCtor)
{
    std::istringstream iss{ "12/12/2024" };
    ASSERT_NO_THROW(Date{ iss });
    iss.clear();

    iss.str("12/12/2024");
    Date d{ iss };
    EXPECT_EQ(d.Day(), 12);
    EXPECT_EQ(d.Month(), 12);
    EXPECT_EQ(d.Year(), 2024);

    iss.clear();
    EXPECT_THROW(Date{ iss }, Date::DateError);

    const std::vector<std::string> invalidFormats{
        "0/12/2024", "32/12/2024", "12/0/2024", "12/12/1899", "12/12/10000", "31/4/2024", "30/2/2024", "29/2/2023",
        "2/12/2024", "12/2/2024", "12/12/20242", "12 12 2024", "12.12.2024", "aaaa12/12/2024", "12/bbbb12/2024",
        "12/12/ccccc2024"
    };
    for (const auto &invalidFormat : invalidFormats) {
        std::istringstream iss{ invalidFormat };
        EXPECT_THROW(Date{ iss }, Date::DateError);
    }
}

TEST(GetterTest, Day)
{
    EXPECT_EQ(Date{ "01/12/2024" }.Day(), 1);
    EXPECT_EQ(Date{ "12/12/2024" }.Day(), 12);
    EXPECT_EQ(Date{ "31/12/2024" }.Day(), 31);
}

TEST(GetterTest, Month)
{
    EXPECT_EQ(Date{ "12/01/2024" }.Month(), 1);
    EXPECT_EQ(Date{ "12/09/2024" }.Month(), 9);
    EXPECT_EQ(Date{ "12/12/2024" }.Month(), 12);
}

TEST(GetterTest, Year)
{
    EXPECT_EQ(Date{ "12/12/1900" }.Year(), 1900);
    EXPECT_EQ(Date{ "12/12/2026" }.Year(), 2026);
    EXPECT_EQ(Date{ "12/12/9999" }.Year(), 9999);
}

TEST(GetterTest, DayOfYear)
{
    EXPECT_EQ(Date{ "01/01/2023" }.DayOfYear(), 1);
    EXPECT_EQ(Date{ "31/12/2023" }.DayOfYear(), 365);
    EXPECT_EQ(Date{ "31/12/2024" }.DayOfYear(), 366);
}

TEST(GetterTest, Weekday)
{
    EXPECT_EQ(Date{ "06/01/2014" }.Weekday(), 1);
    EXPECT_EQ(Date{ "25/09/2018" }.Weekday(), 2);
    EXPECT_EQ(Date{ "01/01/2014" }.Weekday(), 3);
    EXPECT_EQ(Date{ "31/12/2015" }.Weekday(), 4);
    EXPECT_EQ(Date{ "01/01/2016" }.Weekday(), 5);
    EXPECT_EQ(Date{ "31/12/2016" }.Weekday(), 6);
    EXPECT_EQ(Date{ "25/09/2016" }.Weekday(), 7);
}

TEST(GetterTest, WeekOfYear)
{
    EXPECT_EQ(Date{ "01/01/1900" }.WeekOfYear(), Date::ISOWeek(1900, 1));
    EXPECT_EQ(Date{ "31/12/1900" }.WeekOfYear(), Date::ISOWeek(1901, 1));
    EXPECT_EQ(Date{ "01/01/2014" }.WeekOfYear(), Date::ISOWeek(2014, 1));
    EXPECT_EQ(Date{ "06/01/2014" }.WeekOfYear(), Date::ISOWeek(2014, 2));
    EXPECT_EQ(Date{ "25/09/2014" }.WeekOfYear(), Date::ISOWeek(2014, 39));
    EXPECT_EQ(Date{ "31/12/2014" }.WeekOfYear(), Date::ISOWeek(2015, 1));
    EXPECT_EQ(Date{ "01/01/2015" }.WeekOfYear(), Date::ISOWeek(2015, 1));
    EXPECT_EQ(Date{ "31/12/2015" }.WeekOfYear(), Date::ISOWeek(2015, 53));
    EXPECT_EQ(Date{ "01/01/2016" }.WeekOfYear(), Date::ISOWeek(2015, 53));
    EXPECT_EQ(Date{ "04/01/2016" }.WeekOfYear(), Date::ISOWeek(2016, 1));
    EXPECT_EQ(Date{ "31/12/2016" }.WeekOfYear(), Date::ISOWeek(2016, 52));
    EXPECT_EQ(Date{ "01/01/2017" }.WeekOfYear(), Date::ISOWeek(2016, 52));
    EXPECT_EQ(Date{ "02/01/2017" }.WeekOfYear(), Date::ISOWeek(2017, 1));
    EXPECT_EQ(Date{ "01/01/2018" }.WeekOfYear(), Date::ISOWeek(2018, 1));
    EXPECT_EQ(Date{ "31/12/2018" }.WeekOfYear(), Date::ISOWeek(2019, 1));
    EXPECT_EQ(Date{ "31/12/2020" }.WeekOfYear(), Date::ISOWeek(2020, 53));
    EXPECT_EQ(Date{ "29/02/2020" }.WeekOfYear(), Date::ISOWeek(2020, 9));
    EXPECT_EQ(Date{ "29/12/9999" }.WeekOfYear(), Date::ISOWeek(9999, 52));
    EXPECT_EQ(Date{ "30/12/9999" }.WeekOfYear(), Date::ISOWeek(9999, 52));
    EXPECT_EQ(Date{ "31/12/9999" }.WeekOfYear(), Date::ISOWeek(9999, 52));
    EXPECT_EQ(Date{ "01/01/9999" }.WeekOfYear(), Date::ISOWeek(9998, 53));
}

TEST(SetterTest, Day)
{
    Date d{ "06/12/2024" };
    d.Day(15);
    EXPECT_EQ(d.Day(), 15);

    d = Date{ "12/12/2024" };
    EXPECT_NO_THROW(d.Day(1));
    EXPECT_NO_THROW(d.Day(12));
    EXPECT_NO_THROW(d.Day(31));

    d = Date{ "18/12/2024" };
    EXPECT_THROW(d.Day(0), Date::DateError);
    EXPECT_THROW(d.Day(-12), Date::DateError);
    EXPECT_THROW(d.Day(32), Date::DateError);

    d = Date{ "30/04/2024" };
    EXPECT_THROW(d.Day(31), Date::DateError);

    d = Date{ "28/02/2023" };
    EXPECT_THROW(d.Day(29), Date::DateError);
    EXPECT_THROW(d.Day(30), Date::DateError);
    EXPECT_THROW(d.Day(31), Date::DateError);

    d = Date{ "29/02/2024" };
    EXPECT_THROW(d.Day(30), Date::DateError);
    EXPECT_THROW(d.Day(31), Date::DateError);
}

TEST(SetterTest, Month)
{
    Date d{ "06/12/2024" };
    d.Month(3);
    EXPECT_EQ(d.Month(), 3);

    d = Date{ "12/12/2024" };
    EXPECT_NO_THROW(d.Month(1));
    EXPECT_NO_THROW(d.Month(6));
    EXPECT_NO_THROW(d.Month(12));

    d = Date{ "29/01/2024" };
    EXPECT_NO_THROW(d.Month(2));

    d = Date{ "18/12/2024" };
    EXPECT_THROW(d.Month(0), Date::DateError);
    EXPECT_THROW(d.Month(-12), Date::DateError);
    EXPECT_THROW(d.Month(13), Date::DateError);

    d = Date{ "31/05/2024" };
    EXPECT_THROW(d.Month(4), Date::DateError);

    d = Date{ "29/03/2023" };
    EXPECT_THROW(d.Month(2), Date::DateError);

    d = Date{ "30/04/2023" };
    EXPECT_THROW(d.Month(2), Date::DateError);

    d = Date{ "31/03/2023" };
    EXPECT_THROW(d.Month(2), Date::DateError);
}

TEST(SetterTest, Year)
{
    Date d{ "06/12/2024" };
    d.Year(2026);
    EXPECT_EQ(d.Year(), 2026);

    d = Date{ "12/12/2024" };
    EXPECT_NO_THROW(d.Year(Date::MIN_YEAR));
    EXPECT_NO_THROW(d.Year(5000));
    EXPECT_NO_THROW(d.Year(Date::MAX_YEAR));

    d = Date{ "29/02/2020" };
    EXPECT_NO_THROW(d.Year(2024));

    d = Date{ "18/12/2024" };
    EXPECT_THROW(d.Year(Date::MIN_YEAR - 1), Date::DateError);
    EXPECT_THROW(d.Year(Date::MAX_YEAR + 1), Date::DateError);
    EXPECT_THROW(d.Year(0), Date::DateError);

    d = Date{ "29/02/2020" };
    EXPECT_THROW(d.Year(2021), Date::DateError);
}

TEST(OperatorTest, AdditionAssignment)
{
    Date d{ "12/12/2024" };
    ASSERT_NO_THROW(d += 20);
    EXPECT_EQ(d.Day(), 1);
    EXPECT_EQ(d.Month(), 1);
    EXPECT_EQ(d.Year(), 2025);

    d = Date{ "18/12/2024" };
    d += 0;
    EXPECT_EQ(d.Day(), 18);
    EXPECT_EQ(d.Month(), 12);
    EXPECT_EQ(d.Year(), 2024);

    d = Date{ "29/02/2024" };
    d += 1;
    EXPECT_EQ(d.Day(), 1);
    EXPECT_EQ(d.Month(), 3);
    EXPECT_EQ(d.Year(), 2024);

    d = Date{ "28/02/2023" };
    d += 1;
    EXPECT_EQ(d.Day(), 1);
    EXPECT_EQ(d.Month(), 3);
    EXPECT_EQ(d.Year(), 2023);

    d = Date{ "12/12/2024" };
    EXPECT_THROW(d += -1, Date::DateError);

    d = Date{ "01/01/1900" };
    EXPECT_THROW(d += std::numeric_limits<int>::max(), Date::DateError);

    d = Date{ "31/12/9999" };
    EXPECT_THROW(d += 1, Date::DateError);
}

TEST(OperatorTest, SubtractionAssignment)
{
    Date d{ "12/01/2024" };
    ASSERT_NO_THROW(d -= 20);
    EXPECT_EQ(d.Day(), 23);
    EXPECT_EQ(d.Month(), 12);
    EXPECT_EQ(d.Year(), 2023);

    d = Date{ "12/12/2024" };
    d -= 0;
    EXPECT_EQ(d.Day(), 12);
    EXPECT_EQ(d.Month(), 12);
    EXPECT_EQ(d.Year(), 2024);

    d = Date{ "01/03/2024" };
    d -= 1;
    EXPECT_EQ(d.Day(), 29);
    EXPECT_EQ(d.Month(), 2);
    EXPECT_EQ(d.Year(), 2024);

    d = Date{ "01/03/2023" };
    d -= 1;
    EXPECT_EQ(d.Day(), 28);
    EXPECT_EQ(d.Month(), 2);
    EXPECT_EQ(d.Year(), 2023);

    d = Date{ "12/12/2024" };
    EXPECT_THROW(d -= -1, Date::DateError);

    d = Date{ "01/01/1900" };
    EXPECT_THROW(d -= 1, Date::DateError);

    d = Date{ "31/12/9999" };
    EXPECT_THROW(d -= std::numeric_limits<int>::max(), Date::DateError);
}

TEST(OperatorTest, PrefixIncrement)
{
    Date d{ "31/12/2024" };
    ASSERT_NO_THROW(++d);
    EXPECT_EQ(d.Day(), 1);
    EXPECT_EQ(d.Month(), 1);
    EXPECT_EQ(d.Year(), 2025);

    d = Date{ "12/12/2024" };
    EXPECT_EQ(++d, Date{ "13/12/2024" });
    EXPECT_EQ(d.Day(), 13);
    EXPECT_EQ(d.Month(), 12);
    EXPECT_EQ(d.Year(), 2024);

    d = Date{ "29/02/2024" };
    ++d;
    EXPECT_EQ(d.Day(), 1);
    EXPECT_EQ(d.Month(), 3);
    EXPECT_EQ(d.Year(), 2024);

    d = Date{ "28/02/2023" };
    ++d;
    EXPECT_EQ(d.Day(), 1);
    EXPECT_EQ(d.Month(), 3);
    EXPECT_EQ(d.Year(), 2023);

    d = Date{ "31/12/9999" };
    EXPECT_THROW(++d, Date::DateError);
}

TEST(OperatorTest, PostfixIncrement)
{
    Date d{ "31/12/2024" };
    ASSERT_NO_THROW(d++);
    EXPECT_EQ(d.Day(), 1);
    EXPECT_EQ(d.Month(), 1);
    EXPECT_EQ(d.Year(), 2025);

    d = Date{ "12/12/2024" };
    EXPECT_EQ(d++, Date{ "12/12/2024" });
    EXPECT_EQ(d.Day(), 13);
    EXPECT_EQ(d.Month(), 12);
    EXPECT_EQ(d.Year(), 2024);

    d = Date{ "29/02/2024" };
    d++;
    EXPECT_EQ(d.Day(), 1);
    EXPECT_EQ(d.Month(), 3);
    EXPECT_EQ(d.Year(), 2024);

    d = Date{ "28/02/2023" };
    d++;
    EXPECT_EQ(d.Day(), 1);
    EXPECT_EQ(d.Month(), 3);
    EXPECT_EQ(d.Year(), 2023);

    d = Date{ "31/12/9999" };
    EXPECT_THROW(d++, Date::DateError);
}

TEST(OperatorTest, PrefixDecrement)
{
    Date d{ "01/01/2024" };
    ASSERT_NO_THROW(--d);
    EXPECT_EQ(d.Day(), 31);
    EXPECT_EQ(d.Month(), 12);
    EXPECT_EQ(d.Year(), 2023);

    d = Date{ "12/12/2024" };
    EXPECT_EQ(--d, Date{ "11/12/2024" });
    EXPECT_EQ(d.Day(), 11);
    EXPECT_EQ(d.Month(), 12);
    EXPECT_EQ(d.Year(), 2024);

    d = Date{ "01/03/2024" };
    --d;
    EXPECT_EQ(d.Day(), 29);
    EXPECT_EQ(d.Month(), 2);
    EXPECT_EQ(d.Year(), 2024);

    d = Date{ "01/03/2023" };
    --d;
    EXPECT_EQ(d.Day(), 28);
    EXPECT_EQ(d.Month(), 2);
    EXPECT_EQ(d.Year(), 2023);

    d = Date{ "01/01/1900" };
    EXPECT_THROW(--d, Date::DateError);
}

TEST(OperatorTest, PostfixDecrement)
{
    Date d{ "01/01/2024" };
    ASSERT_NO_THROW(d--);
    EXPECT_EQ(d.Day(), 31);
    EXPECT_EQ(d.Month(), 12);
    EXPECT_EQ(d.Year(), 2023);

    d = Date{ "12/12/2024" };
    EXPECT_EQ(d--, Date{ "12/12/2024" });
    EXPECT_EQ(d.Day(), 11);
    EXPECT_EQ(d.Month(), 12);
    EXPECT_EQ(d.Year(), 2024);

    d = Date{ "01/03/2024" };
    d--;
    EXPECT_EQ(d.Day(), 29);
    EXPECT_EQ(d.Month(), 2);
    EXPECT_EQ(d.Year(), 2024);

    d = Date{ "01/03/2023" };
    d--;
    EXPECT_EQ(d.Day(), 28);
    EXPECT_EQ(d.Month(), 2);
    EXPECT_EQ(d.Year(), 2023);

    d = Date{ "01/01/1900" };
    EXPECT_THROW(d--, Date::DateError);
}

TEST(OperatorTest, Comparisons)
{
    Date d1{ "12/12/2024" };
    Date d2{ "22/12/2024" };
    EXPECT_LT(d1, d2);
    EXPECT_LE(d1, d2);
    EXPECT_GT(d2, d1);
    EXPECT_GE(d2, d1);
    EXPECT_NE(d1, d2);

    d1 = Date{ "01/01/2025" };
    d2 = Date{ "01/02/2025" };
    EXPECT_LT(d1, d2);
    EXPECT_LE(d1, d2);
    EXPECT_GT(d2, d1);
    EXPECT_GE(d2, d1);
    EXPECT_NE(d1, d2);

    d1 = Date{ "01/01/2025" };
    d2 = Date{ "01/01/2026" };
    EXPECT_LT(d1, d2);
    EXPECT_LE(d1, d2);
    EXPECT_GT(d2, d1);
    EXPECT_GE(d2, d1);
    EXPECT_NE(d1, d2);

    d1 = Date{ "31/12/2023" };
    d2 = Date{ "01/01/2024" };
    EXPECT_LT(d1, d2);
    EXPECT_LE(d1, d2);
    EXPECT_GT(d2, d1);
    EXPECT_GE(d2, d1);
    EXPECT_NE(d1, d2);

    d1 = Date{ "31/01/2024" };
    d2 = Date{ "01/02/2024" };
    EXPECT_LT(d1, d2);
    EXPECT_LE(d1, d2);
    EXPECT_GT(d2, d1);
    EXPECT_GE(d2, d1);
    EXPECT_NE(d1, d2);

    d2 = d1;
    EXPECT_LE(d1, d2);
    EXPECT_GE(d1, d2);
    EXPECT_EQ(d1, d2);
}

TEST(OperatorTest, Addition)
{
    Date d{ "06/12/2024" };
    ASSERT_NO_THROW(static_cast<void>(d + 6));
    EXPECT_EQ(d + 6, Date{ "12/12/2024" });
    EXPECT_EQ(d.Day(), 6);
    EXPECT_EQ(d.Month(), 12);
    EXPECT_EQ(d.Year(), 2024);

    d = Date{ "12/12/2024" };
    EXPECT_EQ(d + 0, Date{ "12/12/2024" });

    d = Date{ "29/02/2024" };
    EXPECT_EQ(d + 1, Date{ "01/03/2024" });

    d = Date{ "28/02/2023" };
    EXPECT_EQ(d + 1, Date{ "01/03/2023" });

    EXPECT_THROW(static_cast<void>(Date{ "12/12/2024" } + -1), Date::DateError);
    EXPECT_THROW(static_cast<void>(Date{ "01/01/1900" } + std::numeric_limits<int>::max()), Date::DateError);
    EXPECT_THROW(static_cast<void>(Date{ "31/12/9999" } + 1), Date::DateError);
}

TEST(OperatorTest, CommutativeAddition)
{
    Date d{ "06/12/2024" };
    ASSERT_NO_THROW(static_cast<void>(6 + d));
    EXPECT_EQ(6 + d, Date{ "12/12/2024" });
    EXPECT_EQ(d.Day(), 6);
    EXPECT_EQ(d.Month(), 12);
    EXPECT_EQ(d.Year(), 2024);

    d = Date { "12/12/2024" };
    EXPECT_EQ(0 + d, Date{ "12/12/2024" });

    d = Date{ "29/02/2024" };
    EXPECT_EQ(1 + d, Date{ "01/03/2024" });

    d = Date{ "28/02/2023" };
    EXPECT_EQ(1 + d, Date{ "01/03/2023" });

    EXPECT_THROW(static_cast<void>(-1 + Date{ "12/12/2024" }), Date::DateError);
    EXPECT_THROW(static_cast<void>(std::numeric_limits<int>::max() + Date{ "01/01/1900" }), Date::DateError);
    EXPECT_THROW(static_cast<void>(1 + Date{ "31/12/9999" }), Date::DateError);
}

TEST(OperatorTest, Subtraction)
{
    Date d{ "06/12/2024" };
    ASSERT_NO_THROW(static_cast<void>(d - 6));
    EXPECT_EQ(d - 6, Date{ "30/11/2024" });
    EXPECT_EQ(d.Day(), 6);
    EXPECT_EQ(d.Month(), 12);
    EXPECT_EQ(d.Year(), 2024);

    d = Date{ "12/12/2024" };
    EXPECT_EQ(d - 0, Date{ "12/12/2024" });

    d = Date{ "01/03/2024" };
    EXPECT_EQ(d - 1, Date{ "29/02/2024" });

    d = Date{ "01/03/2023" };
    EXPECT_EQ(d - 1, Date{ "28/02/2023" });

    EXPECT_THROW(static_cast<void>(Date{ "12/12/2024" } - -1), Date::DateError);
    EXPECT_THROW(static_cast<void>(Date{ "01/01/1900" } - 1), Date::DateError);
    EXPECT_THROW(static_cast<void>(Date{ "31/12/9999" } - std::numeric_limits<int>::max()), Date::DateError);
}

TEST(OperatorTest, Difference)
{
    Date d1{ "05/12/2024" };
    Date d2{ "06/12/2024" };
    ASSERT_NO_THROW(static_cast<void>(d1 - d2));
    ASSERT_NO_THROW(static_cast<void>(d2 - d1));
    EXPECT_EQ(d1 - d2, 1);
    EXPECT_EQ(d1.Day(), 5);
    EXPECT_EQ(d1.Month(), 12);
    EXPECT_EQ(d1.Year(), 2024);
    EXPECT_EQ(d2.Day(), 6);
    EXPECT_EQ(d2.Month(), 12);
    EXPECT_EQ(d2.Year(), 2024);

    d1 = Date{ "12/12/2024" };
    d2 = Date{ "12/12/2024" };
    EXPECT_EQ(d1 - d2, 0);
    EXPECT_EQ(d2 - d1, 0);

    d1 = Date{ "11/11/2024" };
    d2 = Date{ "11/12/2024" };
    EXPECT_EQ(d1 - d2, 30);
    EXPECT_EQ(d2 - d1, 30);

    d1 = Date{ "11/01/2025" };
    d2 = Date{ "11/12/2024" };
    EXPECT_EQ(d1 - d2, 31);
    EXPECT_EQ(d2 - d1, 31);

    d1 = Date{ "12/12/2022" };
    d2 = Date{ "12/12/2023" };
    EXPECT_EQ(d1 - d2, 365);
    EXPECT_EQ(d2 - d1, 365);

    d1 = Date{ "12/12/2023" };
    d2 = Date{ "12/12/2024" };
    EXPECT_EQ(d1 - d2, 366);
    EXPECT_EQ(d2 - d1, 366);

    d1 = Date{ "01/01/1900" };
    d2 = Date{ "31/12/9999" };
    EXPECT_EQ(d1 - d2, 2'958'463);
    EXPECT_EQ(d2 - d1, 2'958'463);
}

TEST(StaticUtilityTest, IsLeap)
{
    ASSERT_NO_THROW(static_cast<void>(Date::IsLeap(2020)));

    EXPECT_TRUE(Date::IsLeap(2024));
    EXPECT_FALSE(Date::IsLeap(2025));
    EXPECT_FALSE(Date::IsLeap(1900));
    EXPECT_TRUE(Date::IsLeap(2000));

    EXPECT_THROW(static_cast<void>(Date::IsLeap(Date::MIN_YEAR - 1)), Date::DateError);
    EXPECT_THROW(static_cast<void>(Date::IsLeap(Date::MAX_YEAR + 1)), Date::DateError);
}

TEST(StreamTest, Extractor)
{
    std::istringstream iss{ "12/12/2024" };
    Date d{ "06/12/2024" };
    EXPECT_TRUE(iss >> d);
    EXPECT_EQ(d, Date{ "12/12/2024" });

    d = Date{ "18/12/2024" };
    EXPECT_FALSE(iss >> d);
    EXPECT_EQ(d, Date{ "18/12/2024" });
    iss.clear();

    d = Date{ "24/12/2024" };
    const std::vector<std::string> invalidFormats{
        "0/12/2024", "32/12/2024", "12/0/2024", "12/12/1899", "12/12/10000", "31/4/2024", "30/2/2024", "29/2/2023",
        "2/12/2024", "12/2/2024", "12/12/20242", "12 12 2024", "12.12.2024", "aaaa12/12/2024", "12/bbbb12/2024",
        "12/12/ccccc2024"
    };
    for (const auto &invalidFormat : invalidFormats) {
        std::istringstream iss{ invalidFormat };
        EXPECT_FALSE(iss >> d);
        EXPECT_EQ(d, Date{ "24/12/2024" });
    }
}

TEST(StreamTest, Inserter)
{
    std::ostringstream oss;
    oss << Date{ "06/12/2024" };
    EXPECT_EQ(oss.str(), "6 December 2024 Friday");
    oss.str("");

    oss << Date{ "12/12/2024" };
    EXPECT_EQ(oss.str(), "12 December 2024 Thursday");
    oss.str("");

    oss << Date{ "28/02/2023" };
    EXPECT_EQ(oss.str(), "28 February 2023 Tuesday");
    oss.str("");

    oss << Date{ "29/03/2024" };
    EXPECT_EQ(oss.str(), "29 March 2024 Friday");
    oss.str("");

    oss << Date{ "01/01/1900" };
    EXPECT_EQ(oss.str(), "1 January 1900 Monday");
    oss.str("");

    oss << Date{ "31/12/9999" };
    EXPECT_EQ(oss.str(), "31 December 9999 Friday");
    oss.str("");

    oss << Date{ "12/12/2024" } << ' ' << Date{ "18/12/2024" };
    EXPECT_EQ(oss.str(), "12 December 2024 Thursday 18 December 2024 Wednesday");
}