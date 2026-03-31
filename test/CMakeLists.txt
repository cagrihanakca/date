include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/54915d462bb64e9b2bd1a3f0e70c00e6f5d817fb.zip
    DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(date_test date_test.cpp)
target_link_libraries(date_test date GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(date_test)