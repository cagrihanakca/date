#ifndef CGR_DATE_H
#define CGR_DATE_H

#include <compare>
#include <ctime>
#include <iosfwd>
#include <stdexcept>
#include <string>

namespace cgr
{
    class Date {
    public:
        static constexpr int MIN_YEAR{ 1900 };
        static constexpr int MAX_YEAR{ 9999 };

        class InvalidDate : public std::invalid_argument {
        public:
            enum class Reason {
                DAY, MONTH, YEAR, FORMAT, EPOCH, RANGE, STREAM
            };
            InvalidDate(Reason reason, std::string message);
            [[nodiscard]] Reason GetReason() const noexcept;
        private:
            Reason m_reason;
        };

        struct ISOWeek {
            int year;
            int week;
            [[nodiscard]] bool operator==(const ISOWeek &rhs) const noexcept = default;
        };

        [[nodiscard]] static Date Today();
        [[nodiscard]] static Date RandomDate(int minYear, int maxYear);

        Date() noexcept;
        Date(int day, int month, int year);
        explicit Date(const char *str);
        explicit Date(const std::string &str);
        explicit Date(std::time_t timer);
        explicit Date(std::istream &is);

        [[nodiscard]] int Day() const noexcept;
        [[nodiscard]] int Month() const noexcept;
        [[nodiscard]] int Year() const noexcept;
        [[nodiscard]] int DayOfYear() const noexcept;
        [[nodiscard]] int Weekday() const noexcept;
        [[nodiscard]] ISOWeek WeekOfYear() const noexcept;

        Date &Day(int day);
        Date &Month(int month);
        Date &Year(int year);

        Date &operator+=(int days);
        Date &operator-=(int days);

        Date &operator++();
        Date operator++(int);
        Date &operator--();
        Date operator--(int);

        [[nodiscard]] std::strong_ordering operator<=>(const Date &rhs) const noexcept;
        [[nodiscard]] bool operator==(const Date &rhs) const noexcept = default;

        [[nodiscard]] static bool IsLeap(int year);

        friend Date operator+(const Date &d, int days);
        friend Date operator-(const Date &d, int days);
        friend int operator-(const Date &lhs, const Date &rhs) noexcept;

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

    std::istream &operator>>(std::istream &is, Date &d);
    std::ostream &operator<<(std::ostream &os, const Date &d);
}

#endif // CGR_DATE_H