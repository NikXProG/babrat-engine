cmake_minimum_required(VERSION 3.30)
project(babrat_src_core_server)
set(CACHELIB_GENERATE_THRIFT ON CACHE BOOL "Enable Thrift code generation")

find_package(Boost REQUIRED)

FetchContent_Declare(
        nlohmann_json
        GIT_REPOSITORY https://github.com/nlohmann/json.git
        GIT_TAG v3.11.2
)
FetchContent_MakeAvailable(nlohmann_json)

add_library(
        babrat_src_core_server
        include/wal_mask.cpp
)

target_include_directories(
        babrat_src_core_server
        PUBLIC
        ./include
        ${boost_SOURCE_DIR}
)


target_link_libraries(
        babrat_src_core_server
        PUBLIC
        babrat_src_core_common
        nlohmann_json::nlohmann_json
        babrat_src_core_catalog
        babrat_src_core_container_search_tree
        babrat_src_core_wal
        babrat_src_core_container_search_tree_index_red_black
        babrat_src_core_container_search_tree_index_b_pLus)

set_target_properties(
        babrat_src_core_server PROPERTIES
        LANGUAGES CXX
        CXX_STANDARD 20
        CXX_STANDARD_REQUIRED ON
        CXX_EXTENSIONS OFF
        VERSION 1.0
        DESCRIPTION "server interface library")