#include "date.h"

#include <array>
#include <compare>
#include <cstdlib>
#include <ctime>
#include <format>
#include <istream>
#include <limits>
#include <ostream>
#include <random>
#include <regex>
#include <stdexcept>
#include <string>
#include <utility>

namespace
{
    using namespace cgr;

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

    const std::regex datePattern{ R"(^\d{2}/\d{2}/\d{4}$)" };

    void Validate(int day, int month, int year)
    {
        using enum Date::InvalidDate::Reason;

        if ((day < 1) || (day > 31)) {
            throw Date::InvalidDate{ DAY, std::format("invalid day: {} (the day must be between [1, 31])", day) };
        }

        if ((month < JANUARY) || (month > DECEMBER)) {
            throw Date::InvalidDate{ MONTH,
                std::format("invalid month: {} (the month must be between [1, 12])", month) };
        }

        if (year < Date::MIN_YEAR) {
            throw Date::InvalidDate{ YEAR, std::format("invalid year: {} is less than the min year (1900)", year) };
        }

        if (year > Date::MAX_YEAR) {
            throw Date::InvalidDate{ YEAR, std::format("invalid year: {} is greater than the max year (9999)", year) };
        }

        if ((day == 31) &&
            (month == FEBRUARY || month == APRIL || month == JUNE || month == SEPTEMBER || month == NOVEMBER)) {
            throw Date::InvalidDate{ MONTH, std::format("invalid month: {} cannot have 31 days", monthNames[month]) };
        }

        if ((day == 30) && (month == FEBRUARY)) {
            throw Date::InvalidDate{ DAY, "invalid day: February cannot have 30 days" };
        }

        if ((day == 29) && (month == FEBRUARY) && !Date::IsLeap(year)) {
            throw Date::InvalidDate{ YEAR,
                std::format("invalid year: {} isn't leap. February cannot have 29 days if a year isn't leap", year) };
        }
    }

    [[nodiscard]] int DaysSinceMinYear(const Date &d) noexcept
    {
        int daysSinceMinYear{};
        for (auto year{ Date::MIN_YEAR }; year < d.Year(); ++year) {
            daysSinceMinYear += Date::IsLeap(year) ? 366 : 365;
        }

        daysSinceMinYear += d.DayOfYear();

        return daysSinceMinYear;
    }

    [[nodiscard]] Date DateFromDaysSinceMinYear(int daysSinceMinYear) noexcept
    {
        auto year{ Date::MIN_YEAR };
        while (daysSinceMinYear > (Date::IsLeap(year) ? 366 : 365)) {
            daysSinceMinYear -= (Date::IsLeap(year) ? 366 : 365);
            ++year;
        }

        auto month{ 1 };
        while (daysSinceMinYear > daysInMonths[Date::IsLeap(year)][month]) {
            daysSinceMinYear -= daysInMonths[Date::IsLeap(year)][month];
            ++month;
        }

        auto day{ daysSinceMinYear };

        return { day, month, year };
    }

    [[nodiscard]] Date FirstDayOfFirstWeek(int year) noexcept
    {
        Date firstDayOfFirstWeek;
        if (const auto firstDayOfYear{ Date{ 1, 1, year } }; firstDayOfYear.Weekday() <= THURSDAY) {
            firstDayOfFirstWeek = firstDayOfYear - (firstDayOfYear.Weekday() - MONDAY);
        } else {
            firstDayOfFirstWeek = firstDayOfYear + (SUNDAY - firstDayOfYear.Weekday() + 1);
        }

        return firstDayOfFirstWeek;
    }

    [[nodiscard]] Date LastDayOfLastWeek(int year) noexcept
    {
        if (year == Date::MAX_YEAR) {
            return Date{ 31, 12, Date::MAX_YEAR };
        }

        Date lastDayOfLastWeek;
        if (const auto lastDayOfYear{ Date{ 31, 12, year } }; lastDayOfYear.Weekday() < THURSDAY) {
            lastDayOfLastWeek = lastDayOfYear - lastDayOfYear.Weekday();
        } else {
            lastDayOfLastWeek = lastDayOfYear + (SUNDAY - lastDayOfYear.Weekday());
        }

        return lastDayOfLastWeek;
    }
}

namespace cgr
{
    Date::InvalidDate::InvalidDate(Reason reason, std::string message)
        : std::invalid_argument{ std::move(message) }, m_reason{ reason } {}

    Date::InvalidDate::Reason Date::InvalidDate::GetReason() const noexcept
    {
        return m_reason;
    }

    Date Date::Today()
    {
        return Date{ std::time(nullptr) };
    }

    Date Date::RandomDate(int minYear, int maxYear)
    {
        if ((minYear < MIN_YEAR) || (maxYear < MIN_YEAR)) {
            throw std::invalid_argument{ "a year less than min year (1900) is not allowed" };
        }

        if ((minYear > MAX_YEAR) || (maxYear > MAX_YEAR)) {
            throw std::invalid_argument{ "a year greater than max year (9999) is not allowed" };
        }

        if (minYear > maxYear) {
            throw std::invalid_argument{ "min year cannot be greater than max year" };
        }

        thread_local std::mt19937 rng{ std::random_device{}() };
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

    Date::Date() noexcept : m_day{ 1 }, m_month{ 1 }, m_year{ MIN_YEAR } {}

    Date::Date(int day, int month, int year) : m_day{ day }, m_month{ month }, m_year{ year }
    {
        Validate(m_day, m_month, m_year);
    }

    Date::Date(const char *str)
    {
        if (!std::regex_match(str, datePattern)) {
            throw InvalidDate{ InvalidDate::Reason::FORMAT,
                std::format("invalid date format: {} isn't compatible dd/mm/yyyy", str) };
        }

        m_day = std::atoi(str);
        m_month = std::atoi(str + 3);
        m_year = std::atoi(str + 6);
        Validate(m_day, m_month, m_year);
    }

    Date::Date(const std::string &str) : Date{ str.c_str() } {}

    Date::Date(std::time_t timer)
    {
        std::tm time{};

#ifdef _WIN32
        if (localtime_s(&time, &timer)) {
            throw InvalidDate{ InvalidDate::Reason::EPOCH, "conversion from the time since epoch to the date failed" };
        }
#else
        if (!localtime_r(&timer, &time)) {
            throw InvalidDate{ InvalidDate::Reason::EPOCH, "conversion from the time since epoch to the date failed" };
        }
#endif

        m_day = time.tm_mday;
        m_month = time.tm_mon + 1;
        m_year = time.tm_year + 1900;
        Validate(m_day, m_month, m_year);
    }

    Date::Date(std::istream &is)
    {
        if (std::string input; is >> input) {
            if (std::regex_match(input, datePattern)) {
                const auto newDay{ std::atoi(input.c_str()) };
                const auto newMonth{ std::atoi(input.c_str() + 3) };
                const auto newYear{ std::atoi(input.c_str() + 6) };
                try {
                    Validate(newDay, newMonth, newYear);
                    m_day = newDay;
                    m_month = newMonth;
                    m_year = newYear;
                } catch (const InvalidDate &) {
                    is.setstate(std::ios::failbit);
                    throw;
                }
            } else {
                is.setstate(std::ios::failbit);
                throw Date::InvalidDate{ Date::InvalidDate::Reason::FORMAT,
                    std::format("invalid date format: {} isn't compatible dd/mm/yyyy", input) };
            }
        } else {
            throw Date::InvalidDate{ Date::InvalidDate::Reason::STREAM,
                std::format("input stream isn't in good state") };
        }
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

    int Date::Weekday() const noexcept
    {
        return (DaysSinceMinYear(*this) - 1) % 7 + 1;
    }

    Date::ISOWeek Date::WeekOfYear() const noexcept
    {
        const auto firstDayOfFirstWeek{ FirstDayOfFirstWeek(m_year) };
        const auto lastDayOfLastWeek{ LastDayOfLastWeek(m_year) };
        if ((*this >= firstDayOfFirstWeek) && (*this <= lastDayOfLastWeek)) {
            return { m_year, (*this - firstDayOfFirstWeek) / 7 + 1 };
        }

        if (*this < firstDayOfFirstWeek) {
            return { m_year - 1, (*this - FirstDayOfFirstWeek(m_year - 1)) / 7 + 1 };
        }

        return { m_year + 1, 1 };
    }

    Date &Date::Day(int day)
    {
        Validate(day, m_month, m_year);
        m_day = day;

        return *this;
    }

    Date &Date::Month(int month)
    {
        Validate(m_day, month, m_year);
        m_month = month;

        return *this;
    }

    Date &Date::Year(int year)
    {
        Validate(m_day, m_month, year);
        m_year = year;

        return *this;
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

    std::strong_ordering Date::operator<=>(const Date &rhs) const noexcept
    {
        if (const auto cmp{ m_year <=> rhs.m_year }; cmp != 0) {
            return cmp;
        }

        if (const auto cmp{ m_month <=> rhs.m_month }; cmp != 0) {
            return cmp;
        }

        return m_day <=> rhs.m_day;
    }

    bool Date::IsLeap(int year)
    {
        if (year < MIN_YEAR) {
            throw std::invalid_argument{ std::format("invalid year: {} is less than the min year (1900)", year) };
        }

        if (year > MAX_YEAR) {
            throw std::invalid_argument{ std::format("invalid year: {} is greater than the max year (9999)", year) };
        }

        return (year % 4 == 0) && (year % 100 != 0 || year % 400 == 0);
    }

    Date operator+(const Date &d, int days)
    {
        using enum Date::InvalidDate::Reason;

        if (days < 0) {
            throw Date::InvalidDate{ DAY, std::format("invalid day: {}. a day cannot be negative", days) };
        }

        auto daysSinceMinYear{ DaysSinceMinYear(d) };
        if ((std::numeric_limits<int>::max() - days) < daysSinceMinYear) {
            throw Date::InvalidDate{ RANGE,
                std::format("invalid date: {} days after cannot be represented. max date (31/12/9999)", days) };
        }

        if (daysSinceMinYear + days > DaysSinceMinYear(Date{ 31, 12, Date::MAX_YEAR })) {
            throw Date::InvalidDate{ RANGE,
                std::format("invalid date: {} days after cannot be represented. max date (31/12/9999)", days) };
        }

        return DateFromDaysSinceMinYear(daysSinceMinYear + days);
    }

    Date operator+(int days, const Date &d)
    {
        return d + days;
    }

    Date operator-(const Date &d, int days)
    {
        using enum Date::InvalidDate::Reason;

        if (days < 0) {
            throw Date::InvalidDate{ DAY, std::format("invalid day: {}. a day cannot be negative", days) };
        }

        const auto daysSinceMinYear{ DaysSinceMinYear(d) };
        if (daysSinceMinYear <= days) {
            throw Date::InvalidDate{ RANGE,
                std::format("invalid date: {} days before falls before the min date (01/01/1900)", days) };
        }

        return DateFromDaysSinceMinYear(daysSinceMinYear - days);
    }

    int operator-(const Date &lhs, const Date &rhs) noexcept
    {
        return std::abs(DaysSinceMinYear(lhs) - DaysSinceMinYear(rhs));
    }

    std::istream &operator>>(std::istream &is, Date &d)
    {
        if (std::string input; is >> input) {
            if (std::regex_match(input, datePattern)) {
                const auto newDay{ std::atoi(input.c_str()) };
                const auto newMonth{ std::atoi(input.c_str() + 3) };
                const auto newYear{ std::atoi(input.c_str() + 6) };
                try {
                    Validate(newDay, newMonth, newYear);
                    d.m_day = newDay;
                    d.m_month = newMonth;
                    d.m_year = newYear;
                } catch (const Date::InvalidDate &) {
                    is.setstate(std::ios::failbit);
                }
            } else {
                is.setstate(std::ios::failbit);
            }
        }

        return is;
    }

    std::ostream &operator<<(std::ostream &os, const Date &d)
    {
        return os << d.m_day << ' ' << monthNames[d.m_month] << ' ' << d.m_year << ' ' <<
            weekdayNames[d.Weekday()];
    }
}