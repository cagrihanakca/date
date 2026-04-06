cmake_minimum_required(VERSION 3.14)

project(cgr-date VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(date src/date.cpp)
target_include_directories(date PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

include(CMakePackageConfigHelpers)

write_basic_package_version_file(
    ${CMAKE_CURRENT_BINARY_DIR}/cgr-date-config-version.cmake
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion
)

configure_package_config_file(
    cmake/cgr-date-config.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/cgr-date-config.cmake
    INSTALL_DESTINATION lib/cmake/cgr-date
)

install(TARGETS date
    EXPORT date-targets
    ARCHIVE DESTINATION lib
)

install(DIRECTORY include/cgr
    DESTINATION include
)

install(EXPORT date-targets
    FILE cgr-date-targets.cmake
    NAMESPACE cgr::
    DESTINATION lib/cmake/cgr-date
)

install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/cgr-date-config.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/cgr-date-config-version.cmake
    DESTINATION lib/cmake/cgr-date
)

option(DATE_BUILD_TESTS "build test" OFF)
if (DATE_BUILD_TESTS)
    enable_testing()
    add_subdirectory(test)
endif()

option(DATE_BUILD_EXAMPLES "build examples" OFF)
if (DATE_BUILD_EXAMPLES)
    add_subdirectory(example)
endif()