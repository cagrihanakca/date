#include "date.h"

#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <istream>
#include <ostream>
#include <random>
#include <stdexcept>
#include <string>

namespace pro
{
    Date::Date(int day, int mon, int year) : m_day{ day }, m_mon{ mon }, m_year{ year }
    {
        if (!IsValid()) {
            throw std::invalid_argument{ m_ex };
        }
    }

    Date::Date(const char *p) : m_day{ std::atoi(p) }, m_mon{ std::atoi(p + 3) }, m_year{ std::atoi(p + 6) } {}

    Date::Date(const std::string &date) : Date{ date.c_str() } {}

    Date::Date(std::time_t timer)
    {
        const auto *tp{ std::localtime(&timer) };
        m_day = tp->tm_mday;
        m_mon = tp->tm_mon + 1;
        m_year = tp->tm_year + 1900;
    }

    Date::Date(std::istream &is)
    {
        is >> *this;
    }

    int Date::GetMonthDay() const
    {
        return m_day;
    }

    int Date::GetMonth() const
    {
        return m_mon;
    }

    int Date::GetYear() const
    {
        return m_year;
    }

    int Date::GetYearDay() const
    {
        int yearDay{ m_day };
        for (int i{ 1 }; i < m_mon; ++i) {
            yearDay += monthDays[IsLeap(m_year)][i];
        }
        return yearDay;
    }

    Date::Weekday Date::GetWeekDay() const
    {
        return static_cast<Date::Weekday>(GetTotalDays() % 7 + 1);
    }

    Date &Date::SetMonthDay(int day)
    {
        m_day = day;
        return *this;
    }

    Date &Date::SetMonth(int mon)
    {
        m_mon = mon;
        return *this;
    }

    Date &Date::SetYear(int year)
    {
        m_year = year;
        return *this;
    }

    Date &Date::Set(int day, int mon, int year)
    {
        m_day = day;
        m_mon = mon;
        m_year = year;
        return *this;
    }

    Date &Date::SetToCurrentDate()
    {
        return *this = CurrentDate();
    }

    Date Date::operator-(int day) const
    {
        return GetDateFromTotalDays(GetTotalDays() - day);
    }

    int operator-(const Date &date1, const Date &date2)
    {
        return std::abs(date1.GetTotalDays() - date2.GetTotalDays());
    }

    Date operator+(const Date &date, int n)
    {
        return Date::GetDateFromTotalDays(date.GetTotalDays() + n);
    }

    Date operator+(int n, const Date &date)
    {
        return date + n;
    }

    Date &Date::operator+=(int day)
    {
        return *this = GetDateFromTotalDays(GetTotalDays() + day);
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
        auto ret{ *this };
        ++*this;
        return ret;
    }

    Date &Date::operator--()
    {
        return *this -= 1;
    }

    Date Date::operator--(int)
    {
        auto ret{ *this };
        --*this;
        return ret;
    }

    bool operator<(const Date &lhs, const Date &rhs)
    {
        return lhs.GetTotalDays() < rhs.GetTotalDays();
    }

    bool operator<=(const Date &lhs, const Date &rhs)
    {
        return !(rhs < lhs);
    }

    bool operator>(const Date &lhs, const Date &rhs)
    {
        return rhs < lhs;
    }

    bool operator>=(const Date &lhs, const Date &rhs)
    {
        return !(lhs < rhs);
    }

    bool operator==(const Date &lhs, const Date &rhs)
    {
        return lhs.GetTotalDays() == rhs.GetTotalDays();
    }

    bool operator!=(const Date &lhs, const Date &rhs)
    {
        return !(lhs == rhs);
    }

    std::istream &operator>>(std::istream &is, Date &date)
    {
        std::string temp;
        is >> temp;

        date.m_day = std::atoi(temp.c_str());
        date.m_mon = std::atoi(temp.c_str() + 3);
        date.m_year = std::atoi(temp.c_str() + 6);

        return is;
    }

    std::ostream &operator<<(std::ostream &os, const Date &date)
    {
        return os << date.m_day << ' ' << Date::months[date.m_mon] << ' ' << date.m_year << ' '
            << Date::days[static_cast<std::size_t>(date.GetWeekDay())];
    }

    Date Date::CurrentDate()
    {
        return Date{ std::time(nullptr) };
    }

    int Date::CurrentMonthDay()
    {
        return CurrentDate().GetMonthDay();
    }

    int Date::CurrentMonth()
    {
        return CurrentDate().GetMonth();
    }

    int Date::CurrentYear()
    {
        return CurrentDate().GetYear();
    }

    int Date::CurrentYearDay()
    {
        return CurrentDate().GetYearDay();
    }

    Date::Weekday Date::CurrentWeekday()
    {
        return CurrentDate().GetWeekDay();
    }

    bool Date::IsLeap(int year)
    {
        return year % 4 == 0 && (year % 100 != 0 || year % 400 == 0);
    }

    Date Date::RandomDate(int randMinYear, int randMaxYear)
    {
        static std::mt19937 eng{ std::random_device{}() };
        std::uniform_int_distribution dayDist{ 1, 31 };
        std::uniform_int_distribution monDist{ 1, 12 };
        std::uniform_int_distribution yearDist{ randMinYear, randMaxYear };

        Date randDate{ dayDist(eng), monDist(eng), yearDist(eng) };
        while (!randDate.IsValid()) {
            randDate.Set(dayDist(eng), monDist(eng), yearDist(eng));
        }

        return randDate;
    }

    int Date::GetTotalDays() const
    {
        int totalDays{};
        for (int i{ yearBase }; i < m_year; ++i) {
            totalDays += IsLeap(i) ? 366 : 365;
        }
        totalDays += GetYearDay();
        return totalDays;
    }

    bool Date::IsValid() const
    {
        using namespace std::literals::string_literals;

        if (m_day < 1 || m_day > 31) {
            m_ex = "invalid day: "s += std::to_string(m_day);
            return false;
        }

        if (m_mon < JANUARY || m_mon > DECEMBER) {
           m_ex = "invalid month: "s += std::to_string(m_mon);
           return false;
        }

        if (m_year < yearBase) {
            m_ex =  "invalid year: "s += std::to_string(m_year);
            return false;
        }

        if (m_day == 31 &&
            (m_mon == FEBRUARY || m_mon == APRIL || m_mon == JUNE || m_mon == SEPTEMBER || m_mon == NOVEMBER)) {
            m_ex = months[m_mon] += " cannot have 31 days";
            return false;
        }

        if (m_day == 30 && m_mon == FEBRUARY) {
            m_ex = "February cannot have 30 days";
            return false;
        }

        if (m_day == 29 && m_mon == FEBRUARY && !IsLeap(m_year)) {
            m_ex = std::to_string(m_year) += " isn't leap. February cannot have 29 days";
            return false;
        }

        return true;
    }

    Date Date::GetDateFromTotalDays(int totalDays)
    {
        int year{ yearBase };
        while (totalDays >= (IsLeap(year) ? 366 : 365)) {
            totalDays -= (IsLeap(year) ? 366 : 365);
            ++year;
        }

        int mon{ 1 };
        while (totalDays >= monthDays[IsLeap(year)][mon]) {
            totalDays -= monthDays[IsLeap(year)][mon];
            ++mon;
        }

        return { totalDays, mon, year };
    }

    std::istream &operator>>(std::istream &is, Date::Weekday &wd)
    {
        std::string input;
        is >> input;

        wd = static_cast<Date::Weekday>(std::find(Date::days.cbegin(), Date::days.cend(), input) - Date::days.cbegin());

        return is;
    }

    std::ostream &operator<<(std::ostream &os, const Date::Weekday &wd)
    {
        return os << Date::days[static_cast<std::size_t>(wd)];
    }

    Date::Weekday &operator++(Date::Weekday &wd)
    {
        return wd = (wd == Date::Weekday::SATURDAY) ?
            Date::Weekday::SUNDAY : static_cast<Date::Weekday>(static_cast<int>(wd) + 1);
    }

    Date::Weekday operator++(Date::Weekday &wd, int)
    {
        auto ret{ wd };
        ++wd;
        return ret;
    }

    Date::Weekday &operator--(Date::Weekday &wd)
    {
        return wd = (wd == Date::Weekday::SUNDAY) ?
            Date::Weekday::SATURDAY : static_cast<Date::Weekday>(static_cast<int>(wd) - 1);
    }

    Date::Weekday operator--(Date::Weekday &wd, int)
    {
        auto ret{ wd };
        --wd;
        return ret;
    }

    Date::Weekday operator+(const Date::Weekday &wd, int n)
    {
        int daysAfter{ static_cast<int>(wd) + n % 7 };
        return static_cast<Date::Weekday>(daysAfter <= 7 ? daysAfter : daysAfter % 7);
    }

    Date::Weekday operator+(int n, const Date::Weekday &wd)
    {
        return wd + n;
    }

    Date::Weekday operator-(const Date::Weekday &wd, int n)
    {
        int daysBefore{ static_cast<int>(wd) - n % 7 };
        return static_cast<Date::Weekday>(daysBefore >= 1 ? daysBefore : 7 - (n % 7 - static_cast<int>(wd)));
    }
}