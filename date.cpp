#include "date.h"

#include <array>
#include <chrono>
#include <cstdlib>
#include <ctime>
#include <istream>
#include <limits>
#include <ostream>
#include <random>
#include <regex>
#include <stdexcept>
#include <string>

namespace
{
    using namespace cgr;

    constexpr int BASE_YEAR{ 1900 };

    enum Weekday {
        MONDAY = 1, TUESDAY, WEDNESDAY, THURSDAY, FRIDAY, SATURDAY, SUNDAY
    };

    enum Month {
        JANUARY = 1, FEBRUARY, MARCH, APRIL, MAY, JUNE, JULY, AUGUST, SEPTEMBER, OCTOBER, NOVEMBER, DECEMBER
    };

    constexpr std::array<std::array<int, 13>, 2> daysInMonth{{
        { 0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 },
        { 0, 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 }
    }};

    const std::array<std::string, 13> monthNames{
        "", "January", "February", "March", "April", "May", "June", "July", "August", "September", "October",
        "November", "December"
    };

    const std::array<std::string, 8> weekdayNames{
        "", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday", "Sunday"
    };

    void Validate(int day, int month, int year)
    {
        using enum Date::InvalidDate::Reason;

        if (day < 1 || day > 31) {
            throw Date::InvalidDate{ DAY, "invalid day: " + std::to_string(day) +
                " (the day must be between [1, 31])" };
        }

        if (month < JANUARY || month > DECEMBER) {
            throw Date::InvalidDate{ MONTH, "invalid month: " + std::to_string(month) +
                " (the month must be between [1, 12])" };
        }

        if (year < BASE_YEAR) {
            throw Date::InvalidDate{ YEAR, "invalid year: " + std::to_string(year) +
                " is less than the base year (1900)" };
        }

        if (day == 31 &&
            (month == FEBRUARY || month == APRIL || month == JUNE || month == SEPTEMBER || month == NOVEMBER)) {
            throw Date::InvalidDate{ MONTH, "invalid month: " + monthNames[month] + " cannot have 31 days" };
        }

        if (day == 30 && month == FEBRUARY) {
            throw Date::InvalidDate{ DAY, "invalid day: February cannot have 30 days" };
        }

        if (day == 29 && month == FEBRUARY && !Date::IsLeap(year)) {
            throw Date::InvalidDate{ YEAR, "invalid year: " + std::to_string(year) +
                " isn't leap. February cannot have 29 days if a year isn't leap" };
        }
    }

    [[nodiscard]] int DaysSinceBase(const Date &date) noexcept
    {
        int totalDays{};
        for (auto i{ BASE_YEAR }; i < date.Year(); ++i) {
            totalDays += Date::IsLeap(i) ? 366 : 365;
        }

        totalDays += date.YearDay();

        return totalDays;
    }

    [[nodiscard]] Date DateFromDaysSinceBase(int totalDays) noexcept
    {
        auto year{ BASE_YEAR };
        while (totalDays > (Date::IsLeap(year) ? 366 : 365)) {
            totalDays -= (Date::IsLeap(year) ? 366 : 365);
            ++year;
        }

        auto mon{ 1 };
        while (totalDays > daysInMonth[Date::IsLeap(year)][mon]) {
            totalDays -= daysInMonth[Date::IsLeap(year)][mon];
            ++mon;
        }

        return { totalDays, mon, year };
    }
}

namespace cgr
{
    Date::InvalidDate::InvalidDate(Reason reason, const std::string &msg)
        : std::invalid_argument{ msg }, m_reason{ reason } {}

    Date::InvalidDate::Reason Date::InvalidDate::GetReason() const noexcept
    {
        return m_reason;
    }

    Date::Date() : m_day{ 1 }, m_mon{ 1 }, m_year{ BASE_YEAR } {}

    Date::Date(int day, int mon, int year) : m_day{ day }, m_mon{ mon }, m_year{ year }
    {
        Validate(m_day, m_mon, m_year);
    }

    Date::Date(const char *p)
    {
        using namespace std::literals::string_literals;

        if (!std::regex_match(p, std::regex{ R"(^\d{2}/\d{2}/\d{4}$)" })) {
            throw InvalidDate{ InvalidDate::Reason::FORMAT, "invalid date format: "s + p +
                " isn't compatible dd/mm/yyyy" };
        }

        m_day = std::atoi(p);
        m_mon = std::atoi(p + 3);
        m_year = std::atoi(p + 6);
        Validate(m_day, m_mon, m_year);
    }

    Date::Date(const std::string &date) : Date{ date.c_str() } {}

    Date::Date(std::time_t timer)
    {
        const auto *tp{ std::localtime(&timer) };
        if (!tp) {
            throw InvalidDate{ InvalidDate::Reason::EPOCH, "conversion from the time since epoch to the date failed" };
        }

        m_day = tp->tm_mday;
        m_mon = tp->tm_mon + 1;
        m_year = tp->tm_year + 1900;
        Validate(m_day, m_mon, m_year);
    }

    Date::Date(std::istream &is)
    {
        is >> *this;
    }

    int Date::MonthDay() const noexcept
    {
        return m_day;
    }

    int Date::Month() const noexcept
    {
        return m_mon;
    }

    int Date::Year() const noexcept
    {
        return m_year;
    }

    int Date::YearDay() const noexcept
    {
        auto yearDay{ m_day };
        for (auto i{ 1 }; i < m_mon; ++i) {
            yearDay += daysInMonth[IsLeap(m_year)][i];
        }
        return yearDay;
    }

    int Date::YearWeek() const noexcept
    {
        Date firstWeek;
        if (const auto firstDay{ Date{ 1, 1, m_year } }; firstDay.Weekday() > THURSDAY) {
            firstWeek = firstDay + (8 - firstDay.Weekday());
        } else {
            firstWeek = firstDay - (firstDay.Weekday() - 1);
        }

        return (*this - firstWeek) / 7 + 1;
    }

    int Date::Weekday() const noexcept
    {
        return (DaysSinceBase(*this) - 1) % 7 + 1;
    }

    Date &Date::MonthDay(int day)
    {
        const auto temp{ m_day };
        m_day = day;
        try {
            Validate(m_day, m_mon, m_year);
        } catch (const InvalidDate &) {
            m_day = temp;
            throw;
        }
        return *this;
    }

    Date &Date::Month(int mon)
    {
        const auto temp{ m_mon };
        m_mon = mon;
        try {
            Validate(m_day, m_mon, m_year);
        } catch(const InvalidDate &) {
            m_mon = temp;
            throw;
        }
        return *this;
    }

    Date &Date::Year(int year)
    {
        const auto temp{ m_year };
        m_year = year;
        try {
            Validate(m_day, m_mon, m_year);
        } catch (const InvalidDate &) {
            m_year = temp;
            throw;
        }
        return *this;
    }

    Date &Date::Set(int day, int mon, int year)
    {
        const auto tempDay{ m_day }, tempMon{ m_mon }, tempYear{ m_year };
        m_day = day;
        m_mon = mon;
        m_year = year;
        try {
            Validate(m_day, m_mon, m_year);
        } catch (const InvalidDate &) {
            m_day = tempDay;
            m_mon = tempMon;
            m_year = tempYear;
            throw;
        }
        return *this;
    }

    Date &Date::SetCurrentDate()
    {
        return *this = CurrentDate();
    }

    Date Date::operator-(int day) const
    {
        using enum InvalidDate::Reason;

        if (day < 0) {
            throw InvalidDate{ DAY, "invalid day: " + std::to_string(day) + ". a day cannot be negative" };
        }

        const auto totalDays{ DaysSinceBase(*this) };
        if (totalDays <= day) {
            throw InvalidDate{ RANGE, "invalid date: " + std::to_string(day) +
                " days before falls before the base date (01/01/1900)" };
        }
        return DateFromDaysSinceBase(totalDays - day);
    }

    int operator-(const Date &date1, const Date &date2) noexcept
    {
        return std::abs(DaysSinceBase(date1) - DaysSinceBase(date2));
    }

    Date operator+(const Date &date, int n)
    {
        using enum Date::InvalidDate::Reason;

        if (n < 0) {
            throw Date::InvalidDate{ DAY, "invalid day: " + std::to_string(n) + ". a day cannot be negative" };
        }

        auto totalDays{ DaysSinceBase(date) };
        if ((std::numeric_limits<int>::max() - n) < totalDays) {
            throw Date::InvalidDate{ RANGE, "invalid date: " + std::to_string(n) +
                " days after cannot be represented" };
        }

        return DateFromDaysSinceBase(totalDays + n);
    }

    Date operator+(int n, const Date &date)
    {
        return date + n;
    }

    Date &Date::operator+=(int n)
    {
        return *this = *this + n;
    }

    Date &Date::operator-=(int n)
    {
        return *this = *this - n;
    }

    Date &Date::operator++()
    {
        return *this += 1;
    }

    Date Date::operator++(int)
    {
        const auto ret{ *this };
        ++*this;
        return ret;
    }

    Date &Date::operator--()
    {
        return *this -= 1;
    }

    Date Date::operator--(int)
    {
        const auto ret{ *this };
        --*this;
        return ret;
    }

    bool operator<(const Date &lhs, const Date &rhs) noexcept
    {
        return DaysSinceBase(lhs) < DaysSinceBase(rhs);
    }

    bool operator<=(const Date &lhs, const Date &rhs) noexcept
    {
        return !(rhs < lhs);
    }

    bool operator>(const Date &lhs, const Date &rhs) noexcept
    {
        return rhs < lhs;
    }

    bool operator>=(const Date &lhs, const Date &rhs) noexcept
    {
        return !(lhs < rhs);
    }

    bool operator==(const Date &lhs, const Date &rhs) noexcept
    {
        return DaysSinceBase(lhs) == DaysSinceBase(rhs);
    }

    bool operator!=(const Date &lhs, const Date &rhs) noexcept
    {
        return !(lhs == rhs);
    }

    std::istream &operator>>(std::istream &is, Date &date)
    {
        std::string in;
        is >> in;
        if (!std::regex_match(in, std::regex{ R"(^\d{2}/\d{2}/\d{4}$)" })) {
            throw Date::InvalidDate{ Date::InvalidDate::Reason::FORMAT, "invalid date format: " + in +
                " isn't compatible dd/mm/yyyy" };
        }

        date.m_day = std::atoi(in.c_str());
        date.m_mon = std::atoi(in.c_str() + 3);
        date.m_year = std::atoi(in.c_str() + 6);
        Validate(date.m_day, date.m_mon, date.m_year);

        return is;
    }

    std::ostream &operator<<(std::ostream &os, const Date &date)
    {
        return os << date.m_day << ' ' << monthNames[date.m_mon] << ' ' << date.m_year << ' '
            << weekdayNames[date.Weekday()];
    }

    Date Date::CurrentDate()
    {
        return Date{ std::time(nullptr) };
    }

    int Date::CurrentMonthDay()
    {
        return CurrentDate().MonthDay();
    }

    int Date::CurrentMonth()
    {
        return CurrentDate().Month();
    }

    int Date::CurrentYear()
    {
        return CurrentDate().Year();
    }

    int Date::CurrentYearDay()
    {
        return CurrentDate().YearDay();
    }

    int Date::CurrentWeekday()
    {
        return CurrentDate().Weekday();
    }

    bool Date::IsLeap(int year)
    {
        if (year < BASE_YEAR) {
            throw std::invalid_argument{ "invalid year: " + std::to_string(year) +
                " is less than the base year (1900)" };
        }
        return year % 4 == 0 && (year % 100 != 0 || year % 400 == 0);
    }

    Date Date::RandomDate(int minYear, int maxYear)
    {
        if ((minYear < BASE_YEAR) || (maxYear < BASE_YEAR)) {
            throw std::invalid_argument{ "a year less than base year (1900) is not allowed" };
        }

        if (minYear > maxYear) {
            throw std::invalid_argument{ "min year cannot be greater than max year" };
        }

        static std::mt19937 eng{
            static_cast<std::mt19937::result_type>(std::chrono::system_clock::now().time_since_epoch().count())
        };
        auto year{ std::uniform_int_distribution{ minYear, maxYear }(eng) };
        auto mon{ std::uniform_int_distribution{ 1, 12 }(eng) };
        int day{};
        switch (mon) {
            case APRIL: case JUNE: case SEPTEMBER: case NOVEMBER:
                day = std::uniform_int_distribution{ 1, 30 }(eng);
                break;
            case FEBRUARY:
                day = std::uniform_int_distribution{ 1, IsLeap(year) ? 29 : 28 }(eng);
                break;
            default:
                day = std::uniform_int_distribution{ 1, 31 }(eng);
        }

        return { day, mon, year };
    }
}