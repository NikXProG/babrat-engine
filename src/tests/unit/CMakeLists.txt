cmake_minimum_required(VERSION 3.30)
project(babrat_src_tests_unit
        VERSION 1.0
        DESCRIPTION "Babrat Server Test Suite unit"
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
        babrat_src_tests_unit
      #[[  common/statements.cc]]
        commands/commands.cc
      #[[  storage/storage.cc]]
      #[[  container/indexing_tree/indexing_containers.cc]]
      #[[  wal/wal.cc]]
       #[[ commands/commands.cc

        container/index_tree_test.cc]])

target_link_libraries(
        babrat_src_tests_unit
        PRIVATE
        gtest_main)

add_test(
        NAME babrat_src_tests_unit
        COMMAND babrat_tests_unit
)

target_link_libraries(
        babrat_src_tests_unit
        PRIVATE
        babrat_src_core_container_search_tree)

target_link_libraries(
        babrat_src_tests_unit
        PRIVATE
        babrat_src_core_main
        babrat_src_core_wal
        babrat_src_core_statements
        babrat_src_core_container_search_tree_index_b_pLus)

target_link_libraries(
        babrat_src_tests_unit
        PRIVATE
        babrat_src_core_catalog)

target_link_libraries(
        babrat_src_tests_unit
        PRIVATE
        babrat_src_core_commd_processor)

target_link_libraries(
        babrat_src_tests_unit
        PRIVATE
        babrat_src_core_container_search_tree_index_red_black)
