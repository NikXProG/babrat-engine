cmake_minimum_required(VERSION 3.30)
project(babrat_src_core_container_search_tree_index_b_pLus)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
FetchContent_Declare(
        tlx
        URL https://github.com/tlx/tlx/archive/refs/tags/v0.6.1.zip
)
FetchContent_MakeAvailable(tlx)

add_library(
        babrat_src_core_container_search_tree_index_b_pLus
        INTERFACE)

target_include_directories(
        babrat_src_core_container_search_tree_index_b_pLus
        INTERFACE
        ./include/
        ${tlx_SOURCE_DIR})


target_link_libraries(
        babrat_src_core_container_search_tree_index_b_pLus
        INTERFACE
        babrat_src_core_common
)


target_link_libraries(
        babrat_src_core_container_search_tree_index_b_pLus
        INTERFACE
        babrat_src_core_container_search_tree
)

set_target_properties(
        babrat_src_core_container_search_tree_index_b_pLus PROPERTIES
        VERSION 1.0
        DESCRIPTION "b_plus_tree library"
        LANGUAGES CXX
        LINKER_LANGUAGE CXX
        CXX_STANDARD 20
        CXX_STANDARD_REQUIRED ON
        CXX_EXTENSIONS OFF)