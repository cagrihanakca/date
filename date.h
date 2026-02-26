#ifndef DATE_H
#define DATE_H

#include <array>
#include <ctime>
#include <iosfwd>
#include <string>

namespace pro
{
    class Date {
    public:
        enum class Weekday {
            SUNDAY = 1, MONDAY, TUESDAY, WEDNESDAY, THURSDAY, FRIDAY, SATURDAY
        };

        Date() = default;
        Date(int day, int mon, int year);
        explicit Date(const char *p);
        explicit Date(const std::string &date);
        explicit Date(std::time_t timer);
        explicit Date(std::istream &is);

        [[nodiscard]] int GetMonthDay() const;
        [[nodiscard]] int GetMonth() const;
        [[nodiscard]] int GetYear() const;
        [[nodiscard]] int GetYearDay() const;
        [[nodiscard]] Weekday GetWeekDay() const;

        Date &SetMonthDay(int day);
        Date &SetMonth(int mon);
        Date &SetYear(int year);
        Date &Set(int day, int mon, int year);
        Date &SetToCurrentDate();

        [[nodiscard]] Date operator-(int day) const;
        friend int operator-(const Date &date1, const Date &date2);
        friend Date operator+(const Date &date, int n);

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

        friend std::istream &operator>>(std::istream &is, Date::Weekday &wd);
        friend std::ostream &operator<<(std::ostream &os, const Date::Weekday &wd);

        static constexpr int yearBase{ 1900 };
        static constexpr int randomMinYear{ 1900 };
        static constexpr int randomMaxYear{ 2026 };

        [[nodiscard]] static Date CurrentDate();
        [[nodiscard]] static int CurrentMonthDay();
        [[nodiscard]] static int CurrentMonth();
        [[nodiscard]] static int CurrentYear();
        [[nodiscard]] static int CurrentYearDay();
        [[nodiscard]] static Weekday CurrentWeekday();

        [[nodiscard]] static bool IsLeap(int year);
        [[nodiscard]] static Date RandomDate();
    private:
        int m_day{ 1 };
        int m_mon{ 1 };
        int m_year{ yearBase };
        int GetTotalDays() const;
        [[nodiscard]] bool IsValid() const;
        [[nodiscard]] static Date GetDateFromTotalDays(int totalDays);
        enum Month {
            JANUARY = 1, FEBRUARY, MARCH, APRIL, MAY, JUNE, JULY, AUGUST, SEPTEMBER, OCTOBER, NOVEMBER, DECEMBER
        };
        static inline std::array<std::string, 13> months{
            "", "January", "February", "March", "April", "May", "June", "July", "August", "September", "October",
            "November", "December"
        };
        static constexpr std::array<std::array<int, 13>, 2> monthDays{{
            { 0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 },
            { 0, 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 }
        }};
        static inline std::array<std::string, 8> days{
            "", "Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"
        };
    };

    [[nodiscard]] Date operator+(int n, const Date &date);

    [[nodiscard]] bool operator<=(const Date &lhs, const Date &rhs);
    [[nodiscard]] bool operator>(const Date &lhs, const Date &rhs);
    [[nodiscard]] bool operator>=(const Date &lhs, const Date &rhs);
    [[nodiscard]] bool operator!=(const Date &lhs, const Date &rhs);

    Date::Weekday &operator++(Date::Weekday &wd);
    Date::Weekday operator++(Date::Weekday &wd, int);
    Date::Weekday &operator--(Date::Weekday &wd);
    Date::Weekday operator--(Date::Weekday &wd, int);

    [[nodiscard]] Date::Weekday operator+(const Date::Weekday &wd, int n);
    [[nodiscard]] Date::Weekday operator+(int n, const Date::Weekday &wd);
    [[nodiscard]] Date::Weekday operator-(const Date::Weekday &wd, int n);
}

#endif // DATE_H