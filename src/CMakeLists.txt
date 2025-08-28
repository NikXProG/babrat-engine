cmake_minimum_required(VERSION 3.30)
project(babrat_src)
set(CMAKE_CXX_STANDARD 20)

add_subdirectory(api)
add_subdirectory(core)

option(BUILD_TESTING "Build tests" ON)
if(BUILD_TESTING)
    enable_testing()
    add_subdirectory(tests)
endif()