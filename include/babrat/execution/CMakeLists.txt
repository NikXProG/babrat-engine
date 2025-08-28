cmake_minimum_required(VERSION 3.30)
project(babrat_src_core_commd_processor)

include(FetchContent)
FetchContent_Declare(
        magic_enum
        URL https://github.com/Neargye/magic_enum/archive/refs/tags/v0.9.7.zip)

FetchContent_MakeAvailable(
        magic_enum)


add_library(
        babrat_src_core_commd_processor
        INTERFACE)


target_include_directories(
        babrat_src_core_commd_processor
        INTERFACE
        ./include
        ddl
        dml
        sql_commands/
        factories/ddl
)


target_link_libraries(
        babrat_src_core_commd_processor
        INTERFACE
        magic_enum::magic_enum
)


target_link_libraries(
        babrat_src_core_commd_processor
        INTERFACE
        babrat_src_core_server
)

target_link_libraries(
        babrat_src_core_commd_processor
        INTERFACE
        babrat_src_core_common
        babrat_src_core_statements
)

set_target_properties(
        babrat_src_core_commd_processor PROPERTIES
        LANGUAGES CXX
        LINKER_LANGUAGE CXX
        CXX_STANDARD 20
        CXX_STANDARD_REQUIRED ON
        CXX_EXTENSIONS OFF
        VERSION 1.0
        DESCRIPTION "command processor classes library")