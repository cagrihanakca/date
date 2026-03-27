#ifndef DATE_H
#define DATE_H

#include <ctime>
#include <iosfwd>
#include <stdexcept>
#include <string>

namespace cgr
{
    class Date {
    public:
        class InvalidDate : public std::invalid_argument {
        public:
            enum class Reason {
                DAY, MONTH, YEAR, FORMAT, EPOCH, RANGE
            };
            InvalidDate(Reason reason, const std::string &errMsg);
            [[nodiscard]] Reason GetReason() const noexcept;
        private:
            Reason m_reason;
        };

        [[nodiscard]] static Date CurrentDate();
        [[nodiscard]] static Date RandomDate(int minYear, int maxYear);

        Date();
        Date(int day, int mon, int year);
        explicit Date(const char *p);
        explicit Date(const std::string &date);
        explicit Date(std::time_t timer);
        explicit Date(std::istream &is);

        [[nodiscard]] int MonthDay() const noexcept;
        [[nodiscard]] int Month() const noexcept;
        [[nodiscard]] int Year() const noexcept;
        [[nodiscard]] int YearDay() const noexcept;
        [[nodiscard]] int YearWeek() const noexcept;
        [[nodiscard]] int Weekday() const noexcept;

        Date &MonthDay(int day);
        Date &Month(int mon);
        Date &Year(int year);
        Date &Set(int day, int mon, int year);
        Date &SetCurrentDate();

        Date &operator+=(int day);
        Date &operator-=(int day);

        Date &operator++();
        Date operator++(int);
        Date &operator--();
        Date operator--(int);

        [[nodiscard]] static bool IsLeap(int year);

        [[nodiscard]] static int CurrentMonthDay();
        [[nodiscard]] static int CurrentMonth();
        [[nodiscard]] static int CurrentYear();
        [[nodiscard]] static int CurrentYearDay();
        [[nodiscard]] static int CurrentWeekday();

        friend Date operator+(const Date &date, int n);
        friend Date operator-(const Date &date, int n);
        friend int operator-(const Date &date1, const Date &date2) noexcept;

        friend bool operator<(const Date &lhs, const Date &rhs) noexcept;
        friend bool operator==(const Date &lhs, const Date &rhs) noexcept;

        friend std::istream &operator>>(std::istream &is, Date &date);
        friend std::ostream &operator<<(std::ostream &os, const Date &date);
    private:
        int m_day;
        int m_mon;
        int m_year;
    };

    [[nodiscard]] Date operator+(const Date &date, int n);
    [[nodiscard]] Date operator+(int n, const Date &date);
    [[nodiscard]] Date operator-(const Date &date, int n);
    [[nodiscard]] int operator-(const Date &date1, const Date &date2) noexcept;

    [[nodiscard]] bool operator<(const Date &lhs, const Date &rhs) noexcept;
    [[nodiscard]] bool operator<=(const Date &lhs, const Date &rhs) noexcept;
    [[nodiscard]] bool operator>(const Date &lhs, const Date &rhs) noexcept;
    [[nodiscard]] bool operator>=(const Date &lhs, const Date &rhs) noexcept;
    [[nodiscard]] bool operator==(const Date &lhs, const Date &rhs) noexcept;
    [[nodiscard]] bool operator!=(const Date &lhs, const Date &rhs) noexcept;

    std::istream &operator>>(std::istream &is, Date &date);
    std::ostream &operator<<(std::ostream &os, const Date &date);
}

#endif // DATE_H