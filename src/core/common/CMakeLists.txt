project(babrat_src_core_common)

add_library(
        babrat_src_core_common
        not_implemented.cpp
        exceptions/string_empty_error.cpp
        exceptions/babrat_storage_error.cpp
        exceptions/babrat_fatal_error.cpp
        exceptions/babrat_container_error.cpp
        exception_util.cpp
        babrat_id.cpp
        babrat_exception.cpp)

target_include_directories(
        babrat_src_core_common
        PUBLIC
        ./
        exceptions
        ${boost_SOURCE_DIR}
)

set_target_properties(
        babrat_src_core_common PROPERTIES
        LANGUAGES CXX
        LINKER_LANGUAGE CXX
        CXX_STANDARD 20
        CXX_STANDARD_REQUIRED ON
        CXX_EXTENSIONS OFF
        VERSION 1.0
        DESCRIPTION "common utils classes library")