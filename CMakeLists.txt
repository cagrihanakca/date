cmake_minimum_required(VERSION 3.14)

project(date)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(date src/date.cpp)
target_include_directories(date PUBLIC include/)

add_subdirectory(test)
add_subdirectory(examples)