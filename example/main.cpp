#include <iostream>
#include <cgr/date.h>

int main()
{
    // construction & getters
    cgr::Date d1{ "12/12/2024" };
    std::cout << "date  : " << d1 << '\n';
    std::cout << "day   : " << d1.Day() << '\n';
    std::cout << "month : " << d1.Month() << '\n';
    std::cout << "year  : " << d1.Year() << "\n\n";

    // setter chaining
    d1 = cgr::Date{ "11/11/2011" };
    std::cout << "date before chaining : " << d1 << '\n';
    d1.Day(25).Month(3).Year(2026);
    std::cout << "date after chaining  : " << d1 << "\n\n";

    // arithmetic
    d1 = cgr::Date{ "11/11/2011" };
    cgr::Date d2{ 7, 10, 2018 };
    std::cout << "date1         : " << d1 << '\n';
    std::cout << "date2         : " << d2 << '\n';
    std::cout << "date1 - date2 : " << d1 - d2 << " days\n";
    std::cout << "date1 + 30    : " << (d1 + 30) << '\n';
    std::cout << "++date1       : " << ++d1 << '\n';
    std::cout << "date2++       : " << d2++ << "\n\n";

    // comparisons
    d1 = cgr::Date{ "16/06/2016" };
    d2 = cgr::Date{ "17/07/2017" };
    std::cout << "date1          : " << d1 << '\n';
    std::cout << "date2          : " << d2 << '\n';
    std::cout << std::boolalpha;
    std::cout << "date1 <  date2 : " << (d1 < d2) << '\n';
    std::cout << "date1 == date2 : " << (d1 == d2) << "\n\n";

    // static functions
    std::cout << "today        : " << cgr::Date::Today() << '\n';
    std::cout << "random date  : " << cgr::Date::RandomDate() << '\n';
    std::cout << "is 1900 leap : " << cgr::Date::IsLeap(1900) << '\n';
    std::cout << "is 2000 leap : " << cgr::Date::IsLeap(2000) << "\n\n";

    // exception
    try {
        d1 = cgr::Date{ "aaa12/04/2024" };
    } catch (const cgr::Date::DateError &ex) {
        std::cerr << ex.what() << "\n\n";
        if (ex.GetReason() == cgr::Date::DateError::Reason::INVALID_FORMAT) {
            d1 = cgr::Date{ 1, 1, cgr::Date::MIN_YEAR };
        }
    }
}