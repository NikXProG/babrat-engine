cmake_minimum_required(VERSION 3.30)
project(babrat_src_core_container_search_tree)

add_subdirectory(indexing_tree)

add_library(
        babrat_src_core_container_search_tree
        INTERFACE)

target_include_directories(
        babrat_src_core_container_search_tree
        INTERFACE
        ./include
)

target_link_libraries(
        babrat_src_core_container_search_tree
        INTERFACE
        babrat_src_core_container
)

target_link_libraries(
        babrat_src_core_container_search_tree
        INTERFACE
        babrat_src_core_common
)



set_target_properties(
        babrat_src_core_container_search_tree PROPERTIES
        VERSION 1.0
        DESCRIPTION "search tree interface library"
        LANGUAGES CXX
        LINKER_LANGUAGE CXX
        CXX_STANDARD 20
        CXX_STANDARD_REQUIRED ON
        CXX_EXTENSIONS OFF)


