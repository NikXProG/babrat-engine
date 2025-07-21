cmake_minimum_required(VERSION 3.15)
project(babrat_src_core_catalog)

# Пути для Apple Silicon (M1/M2)
set(HOMEBREW_PREFIX "/opt/homebrew")

find_library(SODIUM_LIB
        NAMES sodium
        PATHS "${HOMEBREW_PREFIX}/lib"
        NO_DEFAULT_PATH
)

find_path(SODIUM_INCLUDE_DIR
        NAMES sodium.h
        PATHS "${HOMEBREW_PREFIX}/include"
        NO_DEFAULT_PATH
)

if(NOT SODIUM_LIB OR NOT SODIUM_INCLUDE_DIR)
    message(FATAL_ERROR "libsodium not found! Run: brew install libsodium")
endif()

add_library(babrat_src_core_catalog INTERFACE)

target_include_directories(babrat_src_core_catalog INTERFACE
        ./include/
        ${SODIUM_INCLUDE_DIR}
)

target_link_libraries(babrat_src_core_catalog INTERFACE
        babrat_src_core_common
        babrat_src_core_server
        ${SODIUM_LIB}
)

set_target_properties(babrat_src_core_catalog PROPERTIES
        CXX_STANDARD 20
        CXX_STANDARD_REQUIRED ON
        CXX_EXTENSIONS OFF
)

set_target_properties(
        babrat_src_core_catalog PROPERTIES
        LANGUAGES CXX
        LINKER_LANGUAGE CXX
        CXX_STANDARD 20
        CXX_STANDARD_REQUIRED ON
        CXX_EXTENSIONS OFF
        VERSION 1.0
        DESCRIPTION "server interface library")
