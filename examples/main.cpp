// MIT License

// Copyright (c) 2026 Cagrihan Akca

// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:

// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

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