cmake_minimum_required(VERSION 3.30)
project(babrat_src_tests
        VERSION 1.0
        DESCRIPTION "Babrat Server Test Suite"
        LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(unit)
add_subdirectory(integration)