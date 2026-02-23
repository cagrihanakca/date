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
}