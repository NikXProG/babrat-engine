cmake_minimum_required(VERSION 3.30)
project(babrat_src_core_wal)

FetchContent_Declare(
        nlohmann_json
        GIT_REPOSITORY https://github.com/nlohmann/json.git
        GIT_TAG v3.11.2
)

FetchContent_Declare(
        lrucache11
        GIT_REPOSITORY https://github.com/mohaps/lrucache11.git
        GIT_TAG master
)

add_subdirectory(tests)

FetchContent_MakeAvailable(nlohmann_json lrucache11)

find_package(Boost REQUIRED)

add_library(
        babrat_src_core_wal
        src/wal_default.cpp
        src/wal_format.cpp
)

target_include_directories(
        babrat_src_core_wal
        PUBLIC
        ./include
        ${lrucache11_SOURCE_DIR}
        ${Boost_INCLUDE_DIRS}
)

target_link_libraries(
        babrat_src_core_wal
        PUBLIC
        babrat_src_core_common
        nlohmann_json::nlohmann_json
)

set_target_properties(
        babrat_src_core_wal PROPERTIES
        LANGUAGES CXX
        CXX_STANDARD 20
        CXX_STANDARD_REQUIRED ON
        CXX_EXTENSIONS OFF
        VERSION 3.5
        DESCRIPTION "wal controller interface library"
)