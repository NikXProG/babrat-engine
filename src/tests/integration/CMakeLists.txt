cmake_minimum_required(VERSION 3.30)
project(babrat_src_tests_integration
        VERSION 1.0
        DESCRIPTION "Babrat Server Test Suite integration"
        LANGUAGES CXX
)

enable_testing()

include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip)

FetchContent_MakeAvailable(
        googletest)

add_executable(
        babrat_src_tests_integration
#[[        integration/fast_db_server_tests.cc]]
        catalog_tests.cc)

target_link_libraries(
        babrat_src_tests_integration
        gtest_main)

add_test(
        NAME babrat_src_tests_integration
        COMMAND babrat_tests_integration
)

target_link_libraries(
        babrat_src_tests_integration
        babrat_src_core_container_search_tree_index_b_pLus)


target_link_libraries(
        babrat_src_tests_integration
        babrat_src_core_commd_processor)

target_link_libraries(
        babrat_src_tests_integration
        babrat_src_core_container_search_tree_index_red_black)

target_link_libraries(
        babrat_src_tests_integration
        babrat_src_core_catalog)

target_link_libraries(
        babrat_src_tests_integration
        babrat_src_core_common)

