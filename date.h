#ifndef DATE_H
#define DATE_H

#include <ctime>
#include <iosfwd>

namespace pro
{
    class Date {
    public:
        enum class Weekday {
            SUNDAY, MONDAY, TUESDAY, WEDNESDAY, THURSDAY, FRIDAY, SATURDAY
        };

        Date() = default;
        Date(int day, int mon, int year);
        explicit Date(const char *p);
        explicit Date(std::time_t timer);

        [[nodiscard]] int GetMonthDay() const;
        [[nodiscard]] int GetMonth() const;
        [[nodiscard]] int GetYear() const;
        [[nodiscard]] int GetYearDay() const;
        [[nodiscard]] Weekday GetWeekDay() const;

        Date &SetMonthDay(int day);
        Date &SetMonth(int mon);
        Date &SetYear(int year);
        Date &Set(int day, int mon, int year);

        [[nodiscard]] Date operator-(int day) const;
        Date &operator+=(int day);
        Date &operator-=(int day);
        Date &operator++();
        Date operator++(int);
        Date &operator--();
        Date operator--(int);

        friend bool operator<(const Date &lhs, const Date &rhs);
        friend bool operator==(const Date &lhs, const Date &rhs);

        friend std::istream &operator>>(std::istream &is, Date &date);
        friend std::ostream &operator<<(std::ostream &os, const Date &date);

        static constexpr int yearBase{ 1900 };
        static constexpr int randomMinYear{ 1900 };
        static constexpr int randomMaxYear{ 2026 };

        static constexpr bool IsLeap(int year);
        [[nodiscard]] static Date RandomDate();
    private:
        int m_day{ 1 };
        int m_mon{ 1 };
        int m_year{ yearBase };
    };

    [[nodiscard]] bool operator<=(const Date &lhs, const Date &rhs);
    [[nodiscard]] bool operator>(const Date &lhs, const Date &rhs);
    [[nodiscard]] bool operator>=(const Date &lhs, const Date &rhs);
    [[nodiscard]] bool operator!=(const Date &lhs, const Date &rhs);

    [[nodiscard]] int operator-(const Date &date1, const Date &date2);
    [[nodiscard]] Date operator+(const Date &date, int n);
    [[nodiscard]] Date operator+(int n, const Date &date);

    Date::Weekday &operator++(Date::Weekday &wd);
    Date::Weekday operator++(Date::Weekday &wd, int);
    Date::Weekday &operator--(Date::Weekday &wd);
    Date::Weekday operator--(Date::Weekday &wd, int);
}

#endif // DATE_H