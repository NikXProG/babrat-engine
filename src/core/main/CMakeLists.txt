cmake_minimum_required(VERSION 3.30)
project(babrat_src_core_main)

add_library(
        babrat_src_core_main
        include/db_babrat_controller.cpp
)

target_include_directories(
        babrat_src_core_main
        PUBLIC
        ./include)


target_link_libraries(
        babrat_src_core_main
        PUBLIC
        babrat_src_core_common)

set_target_properties(
        babrat_src_core_main PROPERTIES
        VERSION 1.0
        DESCRIPTION "main database library"
        LANGUAGES CXX
        LINKER_LANGUAGE CXX
        CXX_STANDARD 20
        CXX_STANDARD_REQUIRED ON
        CXX_EXTENSIONS OFF)

