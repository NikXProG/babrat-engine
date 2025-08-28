cmake_minimum_required(VERSION 3.30)
project(babrat_src_core_container_search_tree_index
        VERSION 1.0
        DESCRIPTION "indexing tree realization"
        LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(b_tree)
add_subdirectory(red_black_tree)