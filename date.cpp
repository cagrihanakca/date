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
}