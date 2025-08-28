cmake_minimum_required(VERSION 3.30)
project(babrat_src_core
        VERSION 1.0
        DESCRIPTION "server core realization"
        LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(container)
add_subdirectory(storage)
add_subdirectory(common)
add_subdirectory(statements)
add_subdirectory(catalog)
add_subdirectory(wal)
add_subdirectory(main)
add_subdirectory(commands)