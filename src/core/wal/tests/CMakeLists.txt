cmake_minimum_required(VERSION 3.30)
project(babrat_src_core_wal_tests)

enable_testing()

include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip)

FetchContent_MakeAvailable(
        googletest)

add_executable(
        babrat_src_core_wal_tests
        tests.cc)

target_link_libraries(
        babrat_src_core_wal_tests
        PRIVATE
        babrat_src_core_wal
        gtest_main
)

add_test(
    NAME babrat_src_core_wal_tests
    COMMAND babrat_tests_wal_component
)

set_target_properties(
        babrat_src_core_wal_tests PROPERTIES
        LANGUAGES CXX
        CXX_STANDARD 20
        CXX_STANDARD_REQUIRED ON
        CXX_EXTENSIONS OFF
        VERSION 3.5
        DESCRIPTION "Babrat Server Test Wal Suite")
