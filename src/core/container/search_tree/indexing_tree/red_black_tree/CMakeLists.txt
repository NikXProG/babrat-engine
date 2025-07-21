cmake_minimum_required(VERSION 3.30)
project(babrat_src_core_container_search_tree_index_red_black)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(
        babrat_src_core_container_search_tree_index_red_black
        INTERFACE
        include/red_black_tree.h)

target_include_directories(
        babrat_src_core_container_search_tree_index_red_black
        INTERFACE
        ./include)

target_link_libraries(
        babrat_src_core_container_search_tree_index_red_black
        INTERFACE
        babrat_src_core_common
)

target_link_libraries(
        babrat_src_core_container_search_tree_index_red_black
        INTERFACE
        babrat_src_core_container_search_tree
)

set_target_properties(
        babrat_src_core_container_search_tree_index_red_black PROPERTIES
        VERSION 1.0
        DESCRIPTION "b_plus_tree library"
        LANGUAGES CXX
        LINKER_LANGUAGE CXX
        CXX_STANDARD 20
        CXX_STANDARD_REQUIRED ON
        CXX_EXTENSIONS OFF)
