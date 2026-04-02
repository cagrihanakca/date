#include <iostream>
#include <cgr/date.h>

int main()
{
    // construction
    cgr::Date d1{ "12/12/2024" };
    cgr::Date d2{ 7, 10, 2018 };

    // getters
    std::cout << "date1 : " << d1 << '\n';
    std::cout << "day   : " << d1.Day() << '\n';
    std::cout << "month : " << d1.Month() << '\n';
    std::cout << "year  : " << d1.Year() << "\n\n";

    std::cout << "date2 : " << d2 << '\n';
    std::cout << "day   : " << d2.Day() << '\n';
    std::cout << "month : " << d2.Month() << '\n';
    std::cout << "year  : " << d2.Year() << "\n\n";

    // setter chaining
    d1 = cgr::Date{ "11/11/2011" };
    std::cout << "before chaining : " << d1 << '\n';
    d1.Day(25).Month(3).Year(2026);
    std::cout << "after chaining  : "<< d1 << "\n\n";

    // arithmetic
    d1 = cgr::Date{ "11/11/2011" };
    d2 = cgr::Date{ "10/10/2010" };
    std::cout << "date1         : " << d1 << '\n';
    std::cout << "date2         : " << d2 << '\n';
    std::cout << "date1 - date2 : " << d1 - d2 << " days" << "\n";
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
    std::cout << "today        : " << cgr::Date::Today() << "\n\n";
    std::cout << "random date  : " << cgr::Date::RandomDate() << "\n\n";
    std::cout << "is 1900 leap : " << cgr::Date::IsLeap(1900) << '\n';
    std::cout << "is 2000 leap : " << cgr::Date::IsLeap(2000) << "\n\n";
}