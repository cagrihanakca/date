#include "date.h"

#include <cstdlib>
#include <ctime>

namespace pro
{
    Date::Date(int day, int mon, int year) : m_day{ day }, m_mon{ mon }, m_year{ year } {}

    Date::Date(const char *p) : m_day{ std::atoi(p) }, m_mon{ std::atoi(p + 3) }, m_year{ std::atoi(p + 6) } {}

    Date::Date(std::time_t timer)
    {
        const auto *tp{ std::localtime(&timer) };
        m_day = tp->tm_mday;
        m_mon = tp->tm_mon + 1;
        m_year = tp->tm_year + 1900;
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

    bool Date::IsLeap(int year)
    {
        return year % 4 == 0 && (year % 100 != 0 || year % 400 == 0);
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
}