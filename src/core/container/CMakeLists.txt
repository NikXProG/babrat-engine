cmake_minimum_required(VERSION 3.30)
project(babrat_src_core_container)

add_subdirectory(search_tree)


add_library(
        babrat_src_core_container
        INTERFACE
)

target_include_directories(
        babrat_src_core_container
        INTERFACE
        ./include)

set_target_properties(
        babrat_src_core_container PROPERTIES
        VERSION 1.0
        DESCRIPTION "associative container interface library"
        LANGUAGES CXX
        LINKER_LANGUAGE CXX
        CXX_STANDARD 20
        CXX_STANDARD_REQUIRED ON
        CXX_EXTENSIONS OFF)

