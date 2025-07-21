cmake_minimum_required(VERSION 3.30)
project(babrat_src_core_statements)



add_library(
        babrat_src_core_statements
        INTERFACE
)

target_include_directories(
        babrat_src_core_statements
        INTERFACE
        ./common
        ./ddl
        ./dml
        ./
)

target_link_libraries(
        babrat_src_core_statements
        INTERFACE
        babrat_src_core_common
)

set_target_properties(
        babrat_src_core_statements PROPERTIES
        LANGUAGES CXX
        CXX_STANDARD 20
        CXX_STANDARD_REQUIRED ON
        CXX_EXTENSIONS OFF
        VERSION 3.5
        DESCRIPTION "wal controller interface library"
)