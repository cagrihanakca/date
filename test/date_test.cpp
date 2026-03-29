#include <ctime>
#include <limits>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>
#include "gtest/gtest.h"
#include "date.h"

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

    EXPECT_THROW(static_cast<void>(Date::RandomDate(1899, 2000)), std::invalid_argument);
    EXPECT_THROW(static_cast<void>(Date::RandomDate(1950, 1899)), std::invalid_argument);
    EXPECT_THROW(static_cast<void>(Date::RandomDate(10'000, 2000)), std::invalid_argument);
    EXPECT_THROW(static_cast<void>(Date::RandomDate(1950, 10'000)), std::invalid_argument);
    EXPECT_THROW(static_cast<void>(Date::RandomDate(2000, 1950)), std::invalid_argument);
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

    EXPECT_THROW(Date(0, 12, 2024), Date::InvalidDate);
    EXPECT_THROW(Date(32, 12, 2024), Date::InvalidDate);
    EXPECT_THROW(Date(12, 0, 2024), Date::InvalidDate);
    EXPECT_THROW(Date(12, 12, 1899), Date::InvalidDate);
    EXPECT_THROW(Date(12, 12, 10'000), Date::InvalidDate);
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
    EXPECT_THROW(Date{ "12/12/10000" }, Date::InvalidDate);
    EXPECT_THROW(Date{ "31/4/2024" }, Date::InvalidDate);
    EXPECT_THROW(Date{ "30/2/2024" }, Date::InvalidDate);
    EXPECT_THROW(Date{ "29/2/2023" }, Date::InvalidDate);
    EXPECT_THROW(Date{ "2/12/2024" }, Date::InvalidDate);
    EXPECT_THROW(Date{ "12/2/2024" }, Date::InvalidDate);
    EXPECT_THROW(Date{ "12 12 2024" }, Date::InvalidDate);
    EXPECT_THROW(Date{ "12.12.2024" }, Date::InvalidDate);
    EXPECT_THROW(Date{ "aaaa12/12/2024" }, Date::InvalidDate);
    EXPECT_THROW(Date{ "12/bbbb12/2024" }, Date::InvalidDate);
    EXPECT_THROW(Date{ "12/12/ccccc2024" }, Date::InvalidDate);
}

TEST(CtorTest, StringCtor)
{
    using namespace std::literals::string_literals;

    ASSERT_NO_THROW(Date{ "12/12/2024"s });

    Date d{ "12/12/2024"s };
    EXPECT_EQ(d.Day(), 12);
    EXPECT_EQ(d.Month(), 12);
    EXPECT_EQ(d.Year(), 2024);

    EXPECT_THROW(Date{ "0/12/2024"s }, Date::InvalidDate);
    EXPECT_THROW(Date{ "32/12/2024"s }, Date::InvalidDate);
    EXPECT_THROW(Date{ "12/0/2024"s }, Date::InvalidDate);
    EXPECT_THROW(Date{ "12/12/1899"s }, Date::InvalidDate);
    EXPECT_THROW(Date{ "12/12/10000"s }, Date::InvalidDate);
    EXPECT_THROW(Date{ "31/4/2024"s }, Date::InvalidDate);
    EXPECT_THROW(Date{ "30/2/2024"s }, Date::InvalidDate);
    EXPECT_THROW(Date{ "29/2/2023"s }, Date::InvalidDate);
    EXPECT_THROW(Date{ "2/12/2024"s }, Date::InvalidDate);
    EXPECT_THROW(Date{ "12/2/2024"s }, Date::InvalidDate);
    EXPECT_THROW(Date{ "12/12/20242"s }, Date::InvalidDate);
    EXPECT_THROW(Date{ "12 12 2024"s }, Date::InvalidDate);
    EXPECT_THROW(Date{ "12.12.2024"s }, Date::InvalidDate);
    EXPECT_THROW(Date{ "aaaa12/12/2024"s }, Date::InvalidDate);
    EXPECT_THROW(Date{ "12/bbbb12/2024"s }, Date::InvalidDate);
    EXPECT_THROW(Date{ "12/12/ccccc2024"s }, Date::InvalidDate);
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
    std::istringstream iss;
    EXPECT_THROW(Date{ iss }, Date::InvalidDate);

    iss.clear();
    iss.str("12/12/2024");
    ASSERT_NO_THROW(Date{ iss });

    iss.clear();
    iss.str("12/12/2024");
    Date d{ iss };
    EXPECT_EQ(d.Day(), 12);
    EXPECT_EQ(d.Month(), 12);
    EXPECT_EQ(d.Year(), 2024);

    const std::vector<std::string> inputs{
        "0/12/2024", "32/12/2024", "12/0/2024", "12/12/1899", "12/12/10000", "31/4/2024", "30/2/2024", "29/2/2023",
        "2/12/2024", "12/2/2024", "12/12/20242", "12 12 2024", "12.12.2024", "aaaa12/12/2024", "12/bbbb12/2024",
        "12/12/ccccc2024"
    };

    for (const auto &input : inputs) {
        std::istringstream iss{ input };
        EXPECT_THROW(Date{ iss }, Date::InvalidDate);
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
    EXPECT_EQ(Date{ "12/12/1900" }.Year(), Date::MIN_YEAR);
    EXPECT_EQ(Date{ "12/12/2026" }.Year(), 2026);
    EXPECT_EQ(Date{ "12/12/9999" }.Year(), Date::MAX_YEAR);
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
    EXPECT_EQ(Date{ "01/01/1900" }.WeekOfYear(), Date::ISOWeek(Date::MIN_YEAR, 1));
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
    EXPECT_EQ(Date{ "29/12/9999" }.WeekOfYear(), Date::ISOWeek(Date::MAX_YEAR, 52));
    EXPECT_EQ(Date{ "30/12/9999" }.WeekOfYear(), Date::ISOWeek(Date::MAX_YEAR, 52));
    EXPECT_EQ(Date{ "31/12/9999" }.WeekOfYear(), Date::ISOWeek(Date::MAX_YEAR, 52));
    EXPECT_EQ(Date{ "01/01/9999" }.WeekOfYear(), Date::ISOWeek(9998, 53));
}

TEST(SetterTest, Day)
{
    Date d{ "12/12/2024" };
    d.Day(15);
    EXPECT_EQ(d.Day(), 15);

    EXPECT_NO_THROW(Date{ "12/12/2024" }.Day(1));
    EXPECT_NO_THROW(Date{ "12/12/2024" }.Day(12));
    EXPECT_NO_THROW(Date{ "12/12/2024" }.Day(31));

    EXPECT_THROW(Date{ "12/12/2024" }.Day(0), Date::InvalidDate);
    EXPECT_THROW(Date{ "12/12/2024" }.Day(-12), Date::InvalidDate);
    EXPECT_THROW(Date{ "12/12/2024" }.Day(32), Date::InvalidDate);
    EXPECT_THROW(Date{ "30/04/2024" }.Day(31), Date::InvalidDate);
    EXPECT_THROW(Date{ "28/02/2023" }.Day(29), Date::InvalidDate);
    EXPECT_THROW(Date{ "28/02/2023" }.Day(30), Date::InvalidDate);
    EXPECT_THROW(Date{ "28/02/2023" }.Day(31), Date::InvalidDate);
    EXPECT_THROW(Date{ "29/02/2024" }.Day(30), Date::InvalidDate);
    EXPECT_THROW(Date{ "29/02/2024" }.Day(31), Date::InvalidDate);
}

TEST(SetterTest, Month)
{
    Date d{ "12/12/2024" };
    d.Month(3);
    EXPECT_EQ(d.Month(), 3);

    EXPECT_NO_THROW(Date{ "12/12/2024" }.Month(1));
    EXPECT_NO_THROW(Date{ "12/12/2024" }.Month(6));
    EXPECT_NO_THROW(Date{ "12/12/2024" }.Month(12));
    EXPECT_NO_THROW(Date{ "29/01/2024" }.Month(2));

    EXPECT_THROW(Date{ "12/12/2024" }.Month(0), Date::InvalidDate);
    EXPECT_THROW(Date{ "12/12/2024" }.Month(-12), Date::InvalidDate);
    EXPECT_THROW(Date{ "12/12/2024" }.Month(13), Date::InvalidDate);
    EXPECT_THROW(Date{ "31/05/2024" }.Month(4), Date::InvalidDate);
    EXPECT_THROW(Date{ "29/03/2023" }.Month(2), Date::InvalidDate);
    EXPECT_THROW(Date{ "30/04/2023" }.Month(2), Date::InvalidDate);
    EXPECT_THROW(Date{ "31/03/2023" }.Month(2), Date::InvalidDate);
}

TEST(SetterTest, Year)
{
    Date d{ "12/12/2024" };
    d.Year(2026);
    EXPECT_EQ(d.Year(), 2026);

    EXPECT_NO_THROW(Date{ "12/12/2024" }.Year(Date::MIN_YEAR));
    EXPECT_NO_THROW(Date{ "12/12/2024" }.Year(5000));
    EXPECT_NO_THROW(Date{ "12/12/2024" }.Year(Date::MAX_YEAR));
    EXPECT_NO_THROW(Date{ "29/02/2020" }.Year(2024));

    EXPECT_THROW(Date{ "12/12/2024" }.Year(1899), Date::InvalidDate);
    EXPECT_THROW(Date{ "12/12/2024" }.Year(10'000), Date::InvalidDate);
    EXPECT_THROW(Date{ "12/12/2024" }.Year(0), Date::InvalidDate);
    EXPECT_THROW(Date{ "29/02/2020" }.Year(2021), Date::InvalidDate);
}

TEST(OperatorTest, AdditionAssignment)
{
    Date d{ "12/12/2024" };
    ASSERT_NO_THROW(d += 20);
    EXPECT_EQ(d.Day(), 1);
    EXPECT_EQ(d.Month(), 1);
    EXPECT_EQ(d.Year(), 2025);

    d += 0;
    EXPECT_EQ(d.Day(), 1);
    EXPECT_EQ(d.Month(), 1);
    EXPECT_EQ(d.Year(), 2025);

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
    EXPECT_THROW(d += -1, std::invalid_argument);

    d = Date{ "01/01/2024" };
    EXPECT_THROW(d += std::numeric_limits<int>::max(), Date::InvalidDate);

    d = Date{ "31/12/9999" };
    EXPECT_THROW(d += 1, Date::InvalidDate);
}

TEST(OperatorTest, SubtractionAssignment)
{
    Date d{ "12/01/2024" };
    ASSERT_NO_THROW(d -= 20);
    EXPECT_EQ(d.Day(), 23);
    EXPECT_EQ(d.Month(), 12);
    EXPECT_EQ(d.Year(), 2023);

    d -= 0;
    EXPECT_EQ(d.Day(), 23);
    EXPECT_EQ(d.Month(), 12);
    EXPECT_EQ(d.Year(), 2023);

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
    EXPECT_THROW(d -= -1, std::invalid_argument);

    d = Date{ "01/01/1900" };
    EXPECT_THROW(d -= 1, Date::InvalidDate);

    d = Date{ "31/12/9999" };
    EXPECT_THROW(d -= std::numeric_limits<int>::max(), Date::InvalidDate);
}

TEST(OperatorTest, PrefixIncrement)
{
    Date d{ "31/12/2024" };
    ASSERT_NO_THROW(++d);
    EXPECT_EQ(d.Day(), 1);
    EXPECT_EQ(d.Month(), 1);
    EXPECT_EQ(d.Year(), 2025);

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
    EXPECT_THROW(++d, Date::InvalidDate);
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
    EXPECT_THROW(d++, Date::InvalidDate);
}