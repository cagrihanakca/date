# date

A simple C++20 date library.

![C++20](https://img.shields.io/badge/C%2B%2B-20-blue)
![License: Unlicense](https://img.shields.io/badge/License-Unlicense-blue)

## Features

- Date construction from day/month/year, strings, streams and epoch time
- Getters (day, month, year, day of year, weekday, ISO week)
- Setter chaining
- Arithmetic operations (add, subtract, increment, decrement)
- Comparison operators
- Stream extraction and insertion
- Leap year query
- Random date generation
- Exception-based error handling with reason codes

## Requirements

- C++ standard: `C++20`
- Compiler: `GCC` `Clang` `MSVC`
- Build system: `CMake 3.14+`
- Test dependency: `GoogleTest` *(fetched automatically via CMake)*

## Building

**Clone the repository**

```
git clone https://github.com/cagrihanakca/date.git
cd date
```

**Configure**

```
cmake -B build
```

**Build**

```
cmake --build build
```

**Run tests**

```
ctest --test-dir build
```

## Usage

```cpp
#include <iostream>
#include <cgr/date.h>

int main()
{
    cgr::Date d{ "11/11/2023" };
    std::cout << d.Day() << '/' << d.Month() << '/' << d.Year() << '\n'; // 11/11/2023

    d.Day(12).Month(12).Year(2024);
    std::cout << d << '\n'; // 12 December 2024 Thursday

    std::cout << d + 30 << '\n';
    std::cout << d - 30 << '\n';

    std::cout << cgr::Date::Today() << '\n';
    std::cout << cgr::Date::RandomDate() << '\n';
    std::cout << cgr::Date::IsLeap(2000) << '\n';

    try {
        cgr::Date d{ "invalid" };
    } catch (const cgr::Date::DateError &ex) {
        std::cerr << ex.what() << '\n';
    }
}
```

See `example/main.cpp` for more.

## Documentation

[API Documentation](https://cagrihanakca.github.io/date) — generated with [Doxygen](https://www.doxygen.nl)

## License

This project is released under the [Unlicense](UNLICENSE).
