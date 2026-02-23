#include "date.h"

#include <cstdlib>

namespace pro
{
    Date::Date(int day, int mon, int year) : m_day{ day }, m_mon{ mon }, m_year{ year } {}

    Date::Date(const char *p) : m_day{ std::atoi(p) }, m_mon{ std::atoi(p + 3) }, m_year{ std::atoi(p + 6) } {}
}