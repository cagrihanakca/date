#include "date.h"

#include <cstdlib>
#include <ctime>
#include <istream>
#include <limits>
#include <ostream>
#include <random>
#include <regex>
#include <stdexcept>
#include <string>

namespace pro
{
    Date::Date(int day, int mon, int year) : m_day{ day }, m_mon{ mon }, m_year{ year }
    {
        if (!Valid()) {
            throw std::invalid_argument{ m_ex };
        }
    }

    Date::Date(const char *p)
    {
        using namespace std::literals::string_literals;

        if (!std::regex_match(p, std::regex{ R"(^\d{2}/\d{2}/\d{4}$)" })) {
            throw std::invalid_argument{ "invalid date format: "s + p + " (dd/mm/yyyy)" };
        }

        m_day = std::atoi(p);
        m_mon = std::atoi(p + 3);
        m_year = std::atoi(p + 6);
        if (!Valid()) {
            throw std::invalid_argument{ m_ex };
        }
    }

    Date::Date(const std::string &date) : Date{ date.c_str() } {}

    Date::Date(std::time_t timer)
    {
        const auto *tp{ std::localtime(&timer) };
        if (!tp) {
            throw std::runtime_error{ "calendar time conversion failed" };
        }

        m_day = tp->tm_mday;
        m_mon = tp->tm_mon + 1;
        m_year = tp->tm_year + 1900;
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
            yearDay += monthDays[IsLeap(m_year)][i];
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
        return (TotalDays() - 1) % 7 + 1;
    }

    Date &Date::MonthDay(int day)
    {
        const auto temp{ m_day };
        m_day = day;
        if (!Valid()) {
            m_day = temp;
            throw std::invalid_argument{ m_ex };
        }
        return *this;
    }

    Date &Date::Month(int mon)
    {
        const auto temp{ m_mon };
        m_mon = mon;
        if (!Valid()) {
            m_mon = temp;
            throw std::invalid_argument{ m_ex };
        }
        return *this;
    }

    Date &Date::Year(int year)
    {
        const auto temp{ m_year };
        m_year = year;
        if (!Valid()) {
            m_year = temp;
            throw std::invalid_argument{ m_ex };
        }
        return *this;
    }

    Date &Date::Set(int day, int mon, int year)
    {
        const auto tempDay{ m_day }, tempMon{ m_mon }, tempYear{ m_year };
        m_day = day;
        m_mon = mon;
        m_year = year;
        if (!Valid()) {
            m_day = tempDay;
            m_mon = tempMon;
            m_year = tempYear;
            throw std::invalid_argument{ m_ex };
        }
        return *this;
    }

    Date &Date::SetCurrentDate()
    {
        return *this = CurrentDate();
    }

    Date Date::operator-(int day) const
    {
        if (day < 0) {
            throw std::invalid_argument{ "days cannot be negative" };
        }

        const auto totalDays{ TotalDays() };
        if (totalDays <= day) {
            throw std::invalid_argument{ "a date before base date (01/01/1900)" };
        }
        return DateFromTotalDays(totalDays - day);
    }

    int operator-(const Date &date1, const Date &date2) noexcept
    {
        return std::abs(date1.TotalDays() - date2.TotalDays());
    }

    Date operator+(const Date &date, int n)
    {
        if (n < 0) {
            throw std::invalid_argument{ "days cannot be negative" };
        }

        auto totalDays{ date.TotalDays() };
        if ((std::numeric_limits<int>::max() - n) < totalDays) {
            throw std::invalid_argument{ std::to_string(n) + " days after cannot be represent" };
        }

        return Date::DateFromTotalDays(totalDays + n);
    }

    Date operator+(int n, const Date &date)
    {
        return date + n;
    }

    Date &Date::operator+=(int day)
    {
        return *this = DateFromTotalDays(TotalDays() + day);
    }

    Date &Date::operator-=(int day)
    {
        return *this += -day;
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
        if (*this == Date{ 1, 1, baseYear }) {
            throw std::invalid_argument{ "a date before 01/01/1900" };
        }
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
        return lhs.TotalDays() < rhs.TotalDays();
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
        return lhs.TotalDays() == rhs.TotalDays();
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
            throw std::invalid_argument{ "invalid date input: " + in + " format(dd/mm/yyyy)" };
        }

        date.m_day = std::atoi(in.c_str());
        date.m_mon = std::atoi(in.c_str() + 3);
        date.m_year = std::atoi(in.c_str() + 6);
        if (!date.Valid()) {
            throw std::invalid_argument{ date.m_ex };
        }

        return is;
    }

    std::ostream &operator<<(std::ostream &os, const Date &date)
    {
        return os << date.m_day << ' ' << Date::months[date.m_mon] << ' ' << date.m_year << ' '
            << Date::weekdays[date.Weekday()];
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
        if (year < baseYear) {
            throw std::invalid_argument{ "a year before 1900" };
        }
        return year % 4 == 0 && (year % 100 != 0 || year % 400 == 0);
    }

    Date Date::RandomDate(int randMinYear, int randMaxYear)
    {
        static std::mt19937 eng{ std::random_device{}() };
        std::uniform_int_distribution dayDist{ 1, 31 }, monDist{ 1, 12 }, yearDist{ randMinYear, randMaxYear };
        Date randDate;
        while (true) {
            try {
                randDate.Set(dayDist(eng), monDist(eng), yearDist(eng));
            } catch (const std::exception &) {
                continue;
            }
            break;
        }

        return randDate;
    }

    int Date::TotalDays() const noexcept
    {
        int totalDays{};
        for (auto i{ baseYear }; i < m_year; ++i) {
            totalDays += IsLeap(i) ? 366 : 365;
        }
        totalDays += YearDay();
        return totalDays;
    }

    bool Date::Valid() const
    {
        if (m_day < 1 || m_day > 31) {
            m_ex = "invalid day: " + std::to_string(m_day);
            return false;
        }

        if (m_mon < JANUARY || m_mon > DECEMBER) {
           m_ex = "invalid month: " + std::to_string(m_mon);
           return false;
        }

        if (m_year < baseYear) {
            m_ex =  "invalid year: " + std::to_string(m_year);
            return false;
        }

        if (m_day == 31 &&
            (m_mon == FEBRUARY || m_mon == APRIL || m_mon == JUNE || m_mon == SEPTEMBER || m_mon == NOVEMBER)) {
            m_ex = months[m_mon] + " cannot have 31 days";
            return false;
        }

        if (m_day == 30 && m_mon == FEBRUARY) {
            m_ex = "February cannot have 30 days";
            return false;
        }

        if (m_day == 29 && m_mon == FEBRUARY && !IsLeap(m_year)) {
            m_ex = std::to_string(m_year) + " isn't leap. February cannot have 29 days";
            return false;
        }

        return true;
    }

    Date Date::DateFromTotalDays(int totalDays) noexcept
    {
        auto year{ baseYear };
        while (totalDays >= (IsLeap(year) ? 366 : 365)) {
            totalDays -= (IsLeap(year) ? 366 : 365);
            ++year;
        }

        auto mon{ 1 };
        while (totalDays >= monthDays[IsLeap(year)][mon]) {
            totalDays -= monthDays[IsLeap(year)][mon];
            ++mon;
        }

        return { totalDays, mon, year };
    }
}