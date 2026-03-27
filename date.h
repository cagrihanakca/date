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
            InvalidDate(Reason reason, const std::string &message);
            [[nodiscard]] Reason GetReason() const noexcept;
        private:
            Reason m_reason;
        };

        [[nodiscard]] static Date Today();
        [[nodiscard]] static Date RandomDate(int minYear, int maxYear);

        Date();
        Date(int day, int month, int year);
        explicit Date(const char *str);
        explicit Date(const std::string &str);
        explicit Date(std::time_t timer);
        explicit Date(std::istream &is);

        [[nodiscard]] int Day() const noexcept;
        [[nodiscard]] int Month() const noexcept;
        [[nodiscard]] int Year() const noexcept;
        [[nodiscard]] int DayOfYear() const noexcept;
        [[nodiscard]] int WeekOfYear() const noexcept;
        [[nodiscard]] int Weekday() const noexcept;

        Date &Day(int day);
        Date &Month(int month);
        Date &Year(int year);
        Date &Set(int day, int month, int year);

        Date &operator+=(int days);
        Date &operator-=(int days);

        Date &operator++();
        Date operator++(int);
        Date &operator--();
        Date operator--(int);

        [[nodiscard]] static bool IsLeap(int year);

        friend Date operator+(const Date &d, int days);
        friend Date operator-(const Date &d, int days);
        friend int operator-(const Date &lhs, const Date &rhs) noexcept;

        friend bool operator<(const Date &lhs, const Date &rhs) noexcept;
        friend bool operator==(const Date &lhs, const Date &rhs) noexcept;

        friend std::istream &operator>>(std::istream &is, Date &d);
        friend std::ostream &operator<<(std::ostream &os, const Date &d);
    private:
        int m_day;
        int m_month;
        int m_year;
    };

    [[nodiscard]] Date operator+(const Date &d, int days);
    [[nodiscard]] Date operator+(int days, const Date &d);
    [[nodiscard]] Date operator-(const Date &d, int days);
    [[nodiscard]] int operator-(const Date &lhs, const Date &rhs) noexcept;

    [[nodiscard]] bool operator<(const Date &lhs, const Date &rhs) noexcept;
    [[nodiscard]] bool operator<=(const Date &lhs, const Date &rhs) noexcept;
    [[nodiscard]] bool operator>(const Date &lhs, const Date &rhs) noexcept;
    [[nodiscard]] bool operator>=(const Date &lhs, const Date &rhs) noexcept;
    [[nodiscard]] bool operator==(const Date &lhs, const Date &rhs) noexcept;
    [[nodiscard]] bool operator!=(const Date &lhs, const Date &rhs) noexcept;

    std::istream &operator>>(std::istream &is, Date &d);
    std::ostream &operator<<(std::ostream &os, const Date &d);
}

#endif // DATE_H