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

    constexpr std::array<std::array<int, 13>, 2> daysInMonths{{
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

    [[nodiscard]] int DaysSinceBase(const Date &d) noexcept
    {
        int daysSinceBase{};
        for (auto year{ BASE_YEAR }; year < d.Year(); ++year) {
            daysSinceBase += Date::IsLeap(year) ? 366 : 365;
        }

        daysSinceBase += d.DayOfYear();

        return daysSinceBase;
    }

    [[nodiscard]] Date DateFromDaysSinceBase(int daysSinceBase) noexcept
    {
        auto year{ BASE_YEAR };
        while (daysSinceBase > (Date::IsLeap(year) ? 366 : 365)) {
            daysSinceBase -= (Date::IsLeap(year) ? 366 : 365);
            ++year;
        }

        auto month{ 1 };
        while (daysSinceBase > daysInMonths[Date::IsLeap(year)][month]) {
            daysSinceBase -= daysInMonths[Date::IsLeap(year)][month];
            ++month;
        }

        auto day{ daysSinceBase };

        return { day, month, year };
    }
}

namespace cgr
{
    Date::InvalidDate::InvalidDate(Reason reason, const std::string &message)
        : std::invalid_argument{ message }, m_reason{ reason } {}

    Date::InvalidDate::Reason Date::InvalidDate::GetReason() const noexcept
    {
        return m_reason;
    }

    Date Date::CurrentDate()
    {
        return Date{ std::time(nullptr) };
    }

    Date Date::RandomDate(int minYear, int maxYear)
    {
        if ((minYear < BASE_YEAR) || (maxYear < BASE_YEAR)) {
            throw std::invalid_argument{ "a year less than base year (1900) is not allowed" };
        }

        if (minYear > maxYear) {
            throw std::invalid_argument{ "min year cannot be greater than max year" };
        }

        static std::mt19937 rng{
            static_cast<std::mt19937::result_type>(std::chrono::system_clock::now().time_since_epoch().count())
        };
        auto year{ std::uniform_int_distribution{ minYear, maxYear }(rng) };
        auto month{ std::uniform_int_distribution{ 1, 12 }(rng) };
        int day{};
        switch (month) {
            case APRIL: case JUNE: case SEPTEMBER: case NOVEMBER:
                day = std::uniform_int_distribution{ 1, 30 }(rng);
                break;
            case FEBRUARY:
                day = std::uniform_int_distribution{ 1, IsLeap(year) ? 29 : 28 }(rng);
                break;
            default:
                day = std::uniform_int_distribution{ 1, 31 }(rng);
        }

        return { day, month, year };
    }

    Date::Date() : m_day{ 1 }, m_month{ 1 }, m_year{ BASE_YEAR } {}

    Date::Date(int day, int month, int year) : m_day{ day }, m_month{ month }, m_year{ year }
    {
        Validate(m_day, m_month, m_year);
    }

    Date::Date(const char *str)
    {
        using namespace std::literals::string_literals;

        if (!std::regex_match(str, std::regex{ R"(^\d{2}/\d{2}/\d{4}$)" })) {
            throw InvalidDate{ InvalidDate::Reason::FORMAT, "invalid date format: "s + str +
                " isn't compatible dd/mm/yyyy" };
        }

        m_day = std::atoi(str);
        m_month = std::atoi(str + 3);
        m_year = std::atoi(str + 6);
        Validate(m_day, m_month, m_year);
    }

    Date::Date(const std::string &str) : Date{ str.c_str() } {}

    Date::Date(std::time_t timer)
    {
        const auto *timePtr{ std::localtime(&timer) };
        if (!timePtr) {
            throw InvalidDate{ InvalidDate::Reason::EPOCH, "conversion from the time since epoch to the date failed" };
        }

        m_day = timePtr->tm_mday;
        m_month = timePtr->tm_mon + 1;
        m_year = timePtr->tm_year + 1900;
        Validate(m_day, m_month, m_year);
    }

    Date::Date(std::istream &is)
    {
        is >> *this;
    }

    int Date::Day() const noexcept
    {
        return m_day;
    }

    int Date::Month() const noexcept
    {
        return m_month;
    }

    int Date::Year() const noexcept
    {
        return m_year;
    }

    int Date::DayOfYear() const noexcept
    {
        auto dayOfYear{ m_day };
        for (auto month{ 1 }; month < m_month; ++month) {
            dayOfYear += daysInMonths[IsLeap(m_year)][month];
        }
        return dayOfYear;
    }

    int Date::WeekOfYear() const noexcept
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

    Date &Date::Day(int day)
    {
        const auto oldDay{ m_day };
        m_day = day;
        try {
            Validate(m_day, m_month, m_year);
        } catch (const InvalidDate &) {
            m_day = oldDay;
            throw;
        }
        return *this;
    }

    Date &Date::Month(int month)
    {
        const auto oldMonth{ m_month };
        m_month = month;
        try {
            Validate(m_day, m_month, m_year);
        } catch(const InvalidDate &) {
            m_month = oldMonth;
            throw;
        }
        return *this;
    }

    Date &Date::Year(int year)
    {
        const auto oldYear{ m_year };
        m_year = year;
        try {
            Validate(m_day, m_month, m_year);
        } catch (const InvalidDate &) {
            m_year = oldYear;
            throw;
        }
        return *this;
    }

    Date &Date::Set(int day, int month, int year)
    {
        const auto oldDay{ m_day }, oldMonth{ m_month }, oldYear{ m_year };
        m_day = day;
        m_month = month;
        m_year = year;
        try {
            Validate(m_day, m_month, m_year);
        } catch (const InvalidDate &) {
            m_day = oldDay;
            m_month = oldMonth;
            m_year = oldYear;
            throw;
        }
        return *this;
    }

    Date &Date::SetCurrentDate()
    {
        return *this = CurrentDate();
    }

    Date &Date::operator+=(int days)
    {
        return *this = *this + days;
    }

    Date &Date::operator-=(int days)
    {
        return *this = *this - days;
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

    bool Date::IsLeap(int year)
    {
        if (year < BASE_YEAR) {
            throw std::invalid_argument{ "invalid year: " + std::to_string(year) +
                " is less than the base year (1900)" };
        }
        return year % 4 == 0 && (year % 100 != 0 || year % 400 == 0);
    }

    int Date::CurrentMonthDay()
    {
        return CurrentDate().Day();
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
        return CurrentDate().DayOfYear();
    }

    int Date::CurrentWeekday()
    {
        return CurrentDate().Weekday();
    }

    Date operator+(const Date &d, int days)
    {
        using enum Date::InvalidDate::Reason;

        if (days < 0) {
            throw Date::InvalidDate{ DAY, "invalid day: " + std::to_string(days) + ". a day cannot be negative" };
        }

        auto daysSinceBase{ DaysSinceBase(d) };
        if ((std::numeric_limits<int>::max() - days) < daysSinceBase) {
            throw Date::InvalidDate{ RANGE, "invalid date: " + std::to_string(days) +
                " days after cannot be represented" };
        }

        return DateFromDaysSinceBase(daysSinceBase + days);
    }

    Date operator+(int days, const Date &d)
    {
        return d + days;
    }

    Date operator-(const Date &d, int days)
    {
        using enum Date::InvalidDate::Reason;

        if (days < 0) {
            throw Date::InvalidDate{ DAY, "invalid day: " + std::to_string(days) + ". a day cannot be negative" };
        }

        const auto daysSinceBase{ DaysSinceBase(d) };
        if (daysSinceBase <= days) {
            throw Date::InvalidDate{ RANGE, "invalid date: " + std::to_string(days) +
                " days before falls before the base date (01/01/1900)" };
        }

        return DateFromDaysSinceBase(daysSinceBase - days);
    }

    int operator-(const Date &lhs, const Date &rhs) noexcept
    {
        return std::abs(DaysSinceBase(lhs) - DaysSinceBase(rhs));
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

    std::istream &operator>>(std::istream &is, Date &d)
    {
        std::string input;
        is >> input;
        if (!std::regex_match(input, std::regex{ R"(^\d{2}/\d{2}/\d{4}$)" })) {
            throw Date::InvalidDate{ Date::InvalidDate::Reason::FORMAT, "invalid date format: " + input +
                " isn't compatible dd/mm/yyyy" };
        }

        d.m_day = std::atoi(input.c_str());
        d.m_month = std::atoi(input.c_str() + 3);
        d.m_year = std::atoi(input.c_str() + 6);
        Validate(d.m_day, d.m_month, d.m_year);

        return is;
    }

    std::ostream &operator<<(std::ostream &os, const Date &d)
    {
        return os << d.m_day << ' ' << monthNames[d.m_month] << ' ' << d.m_year << ' '
            << weekdayNames[d.Weekday()];
    }
}