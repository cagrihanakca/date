#ifndef CGR_DATE_H
#define CGR_DATE_H

/**
 * @file date.h
 * @brief Date class declaration.
 */

#include <compare>
#include <ctime>
#include <exception>
#include <iosfwd>
#include <string>

namespace cgr
{
    /// Represents a calendar date.
    class Date {
    public:
        static constexpr int MIN_YEAR{ 1900 }; ///< Minimum supported year.
        static constexpr int MAX_YEAR{ 9999 }; ///< Maximum supported year.

        /// Exception thrown by the Date class.
        class DateError : public std::exception {
        public:
            /// Indicates the reason for a DateError exception.
            enum class Reason {
                DAY,      ///< Day is out of valid range.
                MONTH,    ///< Month is out of valid range.
                YEAR,     ///< Year is out of valid range.
                RANGE,    ///< Resulting date is out of [MIN_YEAR, MAX_YEAR].
                FORMAT,   ///< Invalid date string format.
                EPOCH,    ///< Failed time_t conversion.
                STREAM,   ///< Failed stream extraction.
                ARGUMENT, ///< Invalid function argument.
                SYSTEM    ///< Failed system operation.
            };

            /**
             * @brief Constructs a DateError from a reason and an error message.
             * @param reason The exception reason.
             * @param message The error message.
             */
            DateError(Reason reason, std::string message);

            /// Returns the exception message.
            [[nodiscard]] const char *what() const noexcept override;

            /// Returns the exception reason.
            [[nodiscard]] Reason GetReason() const noexcept;
        private:
            Reason m_reason;
            std::string m_message;
        };

        /// ISO 8601 week date.
        struct ISOWeek {
            int year; ///< ISO week-numbering year.
            int week; ///< ISO week number [1, 53].
            [[nodiscard]] bool operator==(const ISOWeek &rhs) const noexcept = default;
        };

        /**
         * @brief Returns today's date.
         * @throws DateError If today's date cannot be obtained (Reason::SYSTEM).
         */
        [[nodiscard]] static Date Today();

        /**
         * @brief Returns a random date within the given year range.
         * @param minYear The minimum year.
         * @param maxYear The maximum year.
         * @throws DateError If minYear or maxYear is out of [MIN_YEAR, MAX_YEAR] (Reason::ARGUMENT).
         * @throws DateError If minYear > maxYear (Reason::ARGUMENT).
         */
        [[nodiscard]] static Date RandomDate(int minYear = MIN_YEAR, int maxYear = Today().Year());

        /// Constructs a date set to 01/01/1900.
        Date() = default;

        /**
         * @brief Constructs a date from a day, a month and a year.
         * @param day The day.
         * @param month The month.
         * @param year The year.
         * @throws DateError If day is out of valid range (Reason::DAY).
         * @throws DateError If month is out of valid range (Reason::MONTH).
         * @throws DateError If year is out of valid range (Reason::YEAR).
         */
        Date(int day, int month, int year);

        /**
         * @brief Constructs a date by parsing a C-string in dd/mm/yyyy format.
         * @param str The date C-string.
         * @throws DateError If str is nullptr (Reason::ARGUMENT).
         * @throws DateError If str does not match the dd/mm/yyyy format (Reason::FORMAT).
         * @throws DateError If day is out of valid range (Reason::DAY).
         * @throws DateError If month is out of valid range (Reason::MONTH).
         * @throws DateError If year is out of valid range (Reason::YEAR).
         */
        explicit Date(const char *str);

        /**
         * @brief Constructs a date by parsing a string in dd/mm/yyyy format.
         * @param str The date string.
         * @throws DateError If str does not match the dd/mm/yyyy format (Reason::FORMAT).
         * @throws DateError If day is out of valid range (Reason::DAY).
         * @throws DateError If month is out of valid range (Reason::MONTH).
         * @throws DateError If year is out of valid range (Reason::YEAR).
         */
        explicit Date(const std::string &str);

        /**
         * @brief Constructs a date from a time since epoch.
         * @param timer The time since epoch.
         * @throws DateError If conversion from the time since epoch to a date fails (Reason::EPOCH).
         * @throws DateError If the resulting date is out of [MIN_YEAR, MAX_YEAR] (Reason::RANGE).
         */
        explicit Date(std::time_t timer);

        /**
         * @brief Constructs a date by extracting a string in dd/mm/yyyy format from an input stream.
         * @param is The input stream.
         * @throws DateError If the extracted string does not match the dd/mm/yyyy format (Reason::FORMAT).
         * @throws DateError If the input stream is not in good state (Reason::STREAM).
         * @throws DateError If day is out of valid range (Reason::DAY).
         * @throws DateError If month is out of valid range (Reason::MONTH).
         * @throws DateError If year is out of valid range (Reason::YEAR).
         */
        explicit Date(std::istream &is);

        /// Returns the day.
        [[nodiscard]] int Day() const noexcept;

        /// Returns the month.
        [[nodiscard]] int Month() const noexcept;

        /// Returns the year.
        [[nodiscard]] int Year() const noexcept;

        /// Returns the day of year.
        [[nodiscard]] int DayOfYear() const noexcept;

        /// Returns the weekday.
        [[nodiscard]] int Weekday() const noexcept;

        /// Returns the week of year according to ISO 8601.
        [[nodiscard]] ISOWeek WeekOfYear() const noexcept;

        /**
         * @brief Sets the day.
         * @param day The day.
         * @throws DateError If day is out of valid range (Reason::DAY).
         */
        Date &Day(int day) &;

        /**
         * @brief Sets the month.
         * @param month The month.
         * @throws DateError If month is out of valid range (Reason::MONTH).
         */
        Date &Month(int month) &;

        /**
         * @brief Sets the year.
         * @param year The year.
         * @throws DateError If year is out of valid range (Reason::YEAR).
         */
        Date &Year(int year) &;

        /**
         * @brief Moves the date forward by the given number of days.
         * @param days The number of days.
         * @throws DateError If days is negative (Reason::ARGUMENT).
         * @throws DateError If the resulting date exceeds MAX_YEAR (Reason::RANGE).
         */
        Date &operator+=(int days) &;

        /**
         * @brief Moves the date back by the given number of days.
         * @param days The number of days.
         * @throws DateError If days is negative (Reason::ARGUMENT).
         * @throws DateError If the resulting date falls below MIN_YEAR (Reason::RANGE).
         */
        Date &operator-=(int days) &;

        /**
         * @brief Increments the date by one day.
         * @throws DateError If the resulting date exceeds MAX_YEAR (Reason::RANGE).
         */
        Date &operator++() &;

        /**
         * @brief Increments the date by one day.
         * @return The date before the increment.
         * @throws DateError If the resulting date exceeds MAX_YEAR (Reason::RANGE).
         */
        Date operator++(int) &;

        /**
         * @brief Decrements the date by one day.
         * @throws DateError If the resulting date falls below MIN_YEAR (Reason::RANGE).
         */
        Date &operator--() &;

        /**
         * @brief Decrements the date by one day.
         * @return The date before the decrement.
         * @throws DateError If the resulting date falls below MIN_YEAR (Reason::RANGE).
         */
        Date operator--(int) &;

        [[nodiscard]] std::strong_ordering operator<=>(const Date &rhs) const noexcept;
        [[nodiscard]] bool operator==(const Date &rhs) const noexcept = default;

        /**
         * @brief Queries whether a year is leap.
         * @param year The year.
         * @throws DateError If the year is out of range [MIN_YEAR, MAX_YEAR] (Reason::ARGUMENT).
         */
        [[nodiscard]] static bool IsLeap(int year);
    private:
        int m_day{ 1 };
        int m_month{ 1 };
        int m_year{ MIN_YEAR };
    };

    /**
     * @brief Returns the result of moving a date forward by the given number of days.
     * @param d The date.
     * @param days The number of days.
     * @throws DateError If days is negative (Reason::ARGUMENT).
     * @throws DateError If the resulting date exceeds MAX_YEAR (Reason::RANGE).
     */
    [[nodiscard]] Date operator+(const Date &d, int days);

    /**
     * @brief Returns the result of moving a date forward by the given number of days.
     * @param days The number of days.
     * @param d The date.
     * @throws DateError If days is negative (Reason::ARGUMENT).
     * @throws DateError If the resulting date exceeds MAX_YEAR (Reason::RANGE).
     */
    [[nodiscard]] Date operator+(int days, const Date &d);

    /**
     * @brief Returns the result of moving a date back by the given number of days.
     * @param d The date.
     * @param days The number of days.
     * @throws DateError If days is negative (Reason::ARGUMENT).
     * @throws DateError If the resulting date falls below MIN_YEAR (Reason::RANGE).
     */
    [[nodiscard]] Date operator-(const Date &d, int days);

    /// Returns the number of days between two dates.
    [[nodiscard]] int operator-(const Date &lhs, const Date &rhs) noexcept;

    /// Extracts a date from an input stream.
    std::istream &operator>>(std::istream &is, Date &d);

    /// Inserts a date into an output stream.
    std::ostream &operator<<(std::ostream &os, const Date &d);
}

#endif // CGR_DATE_H