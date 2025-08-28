cmake_minimum_required(VERSION 3.14)
project(babrat_tests)

cmake_policy(SET CMP0135 NEW)

include("../cmake/utils.cmake")
string(COMPARE EQUAL "${CMAKE_SOURCE_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}" is_top_level)

if(is_top_level)
    enable_testing()
endif()

include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/refs/tags/release-1.12.1.tar.gz)


set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
set(INSTALL_GTEST OFF)
set(BUILD_SHARED_LIBS OFF)

FetchContent_MakeAvailable(googletest)

if(is_top_level)
    find_package(babrat REQUIRED)
endif()

set(sources
        container/search_trees.cpp)
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" FILES ${sources})

add_executable(babrat_tests)
target_sources(babrat_tests PRIVATE ${sources})

target_link_libraries(babrat_tests
        PRIVATE
        babrat::babrat
        gtest_main)

if(NOT is_top_level)
    win_copy_deps_to_target_dir(babrat_tests babrat::babrat)
endif()

include(GoogleTest)
gtest_discover_tests(babrat_tests)