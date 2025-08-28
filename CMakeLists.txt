<<<<<<< HEAD
cmake_minimum_required(VERSION 3.14)

# ---------------------------------------------------------------------------------------
# Start babrat project
# ---------------------------------------------------------------------------------------

project(babrat
        VERSION 1.0.0
        DESCRIPTION "Educational database system exploring core DB architecture"
        LANGUAGES CXX)
=======
cmake_minimum_required(VERSION 3.30)
project(babrat
        VERSION 1.0
        DESCRIPTION "Babrat DBMS Server"
        LANGUAGES CXX
)
>>>>>>> c6974f1aefbdf4da1c1a0abe17222de4b8f57795

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

<<<<<<< HEAD
include(cmake/utils.cmake)

#[[include(cmake/boost.cmake)
add_boost_if_needed()]]

include(cmake/absl.cmake)
add_absl_if_needed()

include(GNUInstallDirs)

string(COMPARE EQUAL "${CMAKE_SOURCE_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}" is_top_level)

# tests option
option(BABRAT_BUILD_TESTS "Build babrat tests" OFF)

# examples option
option(BABRAT_BUILD_EXAMPLES "Build babrat examples" OFF)

# docs option
option(BABRAT_BUILD_DOCS "Build babrat documentation" OFF)

# install option
option(BABRAT_INSTALL "Generate target for installing babrat" ${is_top_level})
set_if_undefined(BABRAT_INSTALL_CMAKEDIR "${CMAKE_INSTALL_LIBDIR}/cmake/babrat" CACHE STRING
        "Install path for babrat package-related CMake files")

# ---------------------------------------------------------------------------------------
# Set Static/Shared
# ---------------------------------------------------------------------------------------

#[[set(BABRAT_SHARED_LIBS ON)]]

if(DEFINED BABRAT_SHARED_LIBS)
    set(BUILD_SHARED_LIBS ${BABRAT_SHARED_LIBS})
endif()

#----------Default to hidden visibility for symbols-------------
set_if_undefined(CMAKE_CXX_VISIBILITY_PRESET hidden)
set_if_undefined(CMAKE_VISIBILITY_INLINES_HIDDEN ON)

add_library(babrat)
add_library(babrat::babrat ALIAS babrat)

# ---------------------------------------------------------------------------------------
# Set default build to release
# ---------------------------------------------------------------------------------------

if(NOT DEFINED CMAKE_BUILD_TYPE AND NOT DEFINED CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
    set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif()

#----------------------------------------------------------------------------------------------------------------------
# Set dependencies
#----------------------------------------------------------------------------------------------------------------------

find_package(
        nlohmann_json
        3.11.2
        REQUIRED)

find_package(
        spdlog
        REQUIRED)

find_package(
        fmt
        REQUIRED
)

#----------------------------------------------------------------------------------------------------------------------
# Set sources
#----------------------------------------------------------------------------------------------------------------------

include(GenerateExportHeader)

set(export_macro_name "BABRAT_API")

set(export_file_name "api_shared.h")

if(NOT BUILD_SHARED_LIBS)
    set(export_file_name "api_static.h")
endif()

generate_export_header(
        babrat
        EXPORT_FILE_NAME include/babrat/${export_file_name}
        EXPORT_MACRO_NAME ${export_macro_name})

set(sources

        #----------Set include headers-------------
        #common
        include/babrat/common/babrat_api.hpp
        include/babrat/common/error.hpp
        include/babrat/common/exception.hpp
        include/babrat/common/throw_base.hpp
        include/babrat/common/error/stream_error.hpp
        include/babrat/common/error/not_implemented_error.hpp
        include/babrat/common/error/string_empty_error.hpp

        #container
        include/babrat/container/associative_container.hpp

        #container/common
        include/babrat/container/common/exception.hpp

        #container/search_tree
        include/babrat/container/search_tree/b_tree.hpp
        include/babrat/container/search_tree/red_black_tree.hpp
        include/babrat/container/search_tree/search_tree.hpp

        #execution
        include/babrat/execution/ddl_command.hpp
        include/babrat/execution/ddl_executor.hpp
        include/babrat/execution/ddl_command_result.hpp

        #execution/command/ddl
        include/babrat/execution/command/ddl/create_table.hpp
    #[[    include/babrat/execution/command/ddl/cr]]

        #interfaces
        include/babrat/serialization/serializable.hpp

        #storage/logger
        include/babrat/logger/logger.hpp
        include/babrat/logger/logger_manager.hpp
        include/babrat/logger/logger_sink.hpp

        #storage/logger/common
        include/babrat/logger/common/logger_config.hpp

        #main
        include/babrat/main/connection.hpp
        include/babrat/main/connection_manager.hpp
        include/babrat/main/database.hpp
        include/babrat/main/client_context.hpp
        include/babrat/main/database_manager.hpp

        #storage
        include/babrat/storage/database_storage.hpp
        include/babrat/storage/db_enums.hpp
        include/babrat/storage/db_options.hpp
        include/babrat/storage/db_wal_serializer.hpp

        #storage/common
        include/babrat/storage/common/handlers.hpp
        include/babrat/storage/common/tree_utils.hpp
        include/babrat/storage/common/convert_timestamp.hpp
        include/babrat/storage/common/convert_variant.hpp
        include/babrat/storage/common/wal_encoder.hpp

        #storage/file_system
        include/babrat/storage/file_system/file_system_storage.hpp

        #storage/in_memory
        include/babrat/storage/in_memory/in_memory_storage.hpp
        include/babrat/storage/in_memory/collection.hpp
        include/babrat/storage/in_memory/pool.hpp
        include/babrat/storage/in_memory/schema.hpp

        #storage/interfaces
        include/babrat/storage/interfaces/persist_storage.hpp
        include/babrat/storage/interfaces/storage.hpp

        #wal
        include/babrat/wal/lrucache11.hpp
        include/babrat/wal/wal.hpp
        include/babrat/wal/wal_format.hpp
        include/babrat/wal/wal_struct.hpp

        #wal/common
        include/babrat/wal/common/boundary_index_exception.hpp
        include/babrat/wal/common/closed_file_exception.hpp
        include/babrat/wal/common/corrupt_exception.hpp
        include/babrat/wal/common/not_found_exception.hpp
        include/babrat/wal/common/out_of_range_exception.hpp

        #wal/default
        include/babrat/wal/default/wal_default.hpp
        include/babrat/wal/default/wal_default_factory.hpp

        #----------Set source files-------------
        #common
        src/common/error.cpp
        src/common/exception.cpp
        src/common/throw_base.cpp
        src/common/stream_error.cpp
        src/common/not_implemented_error.cpp
        src/common/string_empty_error.cpp

        #container/common
        src/container/common/exception.cpp

        #execution
        src/execution/ddl_executor.cpp
        src/execution/ddl_command_result.cpp

        #execution/command/ddl
        src/execution/command/ddl/create_table.cpp
        src/execution/command/ddl/create_schema.cpp

        #logger
        src/logger/logger.cpp
        src/logger/logger_manager.cpp
        src/logger/logger_storage.cpp

        #logger/common
        src/logger/common/logger_config.cpp

        #main
        src/main/connection.cpp
        src/main/database.cpp
        src/main/connection_manager.cpp
        src/main/database_manager.cpp
        src/main/client_context.cpp

        #storage
        src/storage/db_wal_serializer.cpp

        #wal
        src/wal/wal.cpp
        src/wal/wal_format.cpp
        src/wal/wal_struct.cpp

        #wal/common
        src/wal/common/boundary_index_exception.cpp
        src/wal/common/closed_file_exception.cpp
        src/wal/common/corrupt_exception.cpp
        src/wal/common/not_found_exception.cpp
        src/wal/common/out_of_range_exception.cpp

        #wal/default
        src/wal/default/wal_default.cpp
        src/wal/default/wal_default_factory.cpp)

source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" FILES ${sources})

#----------------------------------------------------------------------------------------------------------------------
# Set targets
#----------------------------------------------------------------------------------------------------------------------

include(CMakePackageConfigHelpers)

target_sources(
        babrat
        PRIVATE
        ${sources})

target_compile_definitions(
        babrat
        PUBLIC
        "$<$<NOT:$<BOOL:${BUILD_SHARED_LIBS}>>:BABRAT_STATIC_DEFINE>")

target_include_directories(babrat
        PUBLIC
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include>")

set_target_properties(babrat PROPERTIES
        SOVERSION ${PROJECT_VERSION_MAJOR}
        VERSION ${PROJECT_VERSION})

# Add nlohmann_json libraries
target_link_libraries(
        babrat
        PUBLIC
        nlohmann_json::nlohmann_json)

# Add spdlog logger libraries
target_link_libraries(
        babrat
        PUBLIC
        spdlog::spdlog)

# Add fmt libraries
target_link_libraries(
        babrat
        PUBLIC
        fmt::fmt)

# Add abseil libraries
target_link_libraries(
        babrat
        PUBLIC
        absl::btree)

if(BABRAT_INSTALL AND NOT CMAKE_SKIP_INSTALL_RULES)

    configure_package_config_file(cmake/babrat-config.cmake.in babrat-config.cmake
            INSTALL_DESTINATION "${BABRAT_INSTALL_CMAKEDIR}")

    write_basic_package_version_file(babrat-config-version.cmake
            COMPATIBILITY SameMajorVersion)

    install(TARGETS babrat EXPORT babrat_export
            RUNTIME COMPONENT babrat
            LIBRARY COMPONENT babrat NAMELINK_COMPONENT babrat-dev
            ARCHIVE COMPONENT babrat-dev
            INCLUDES DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}")

    install(DIRECTORY include/
            TYPE INCLUDE
            COMPONENT babrat-dev)

    install(FILES "${CMAKE_CURRENT_BINARY_DIR}/include/babrat/${export_file_name}"
            COMPONENT babrat-dev
            DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/babrat")

    set(targets_file "babrat-shared-targets.cmake")

    if(NOT BUILD_SHARED_LIBS)
        set(targets_file "babrat-static-targets.cmake")
    endif()

    install(EXPORT babrat_export
            COMPONENT babrat-dev
            FILE "${targets_file}"
            DESTINATION "${BABRAT_INSTALL_CMAKEDIR}"
            NAMESPACE babrat::)

    install(FILES
            "${CMAKE_CURRENT_BINARY_DIR}/babrat-config.cmake"
            "${CMAKE_CURRENT_BINARY_DIR}/babrat-config-version.cmake"
            COMPONENT babrat-dev
            DESTINATION "${BABRAT_INSTALL_CMAKEDIR}")

    if(MSVC)
        set(pdb_file "")
        set(pdb_file_destination "")

        if(BUILD_SHARED_LIBS)
            set(pdb_file "$<TARGET_PDB_FILE:babrat>")
            set(pdb_file_destination "${CMAKE_INSTALL_BINDIR}")
        else()
            # TARGET_PDB_FILE does not work for pdb file generated for static library build, determining it manually
            set(pdb_file "$<TARGET_FILE_DIR:babrat>/$<TARGET_FILE_PREFIX:babrat>$<TARGET_FILE_BASE_NAME:babrat>.pdb")
            set(pdb_file_destination "${CMAKE_INSTALL_LIBDIR}")
        endif()

        install(FILES "${pdb_file}"
                COMPONENT babrat-dev
                CONFIGURATIONS Debug RelWithDebInfo
                DESTINATION "${pdb_file_destination}"
                OPTIONAL)
    endif()
endif()

#----------------------------------------------------------------------------------------------------------------------
# Set other targets
#----------------------------------------------------------------------------------------------------------------------

if(BABRAT_BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()

if(BABRAT_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()

if(BABRAT_BUILD_DOCS)
    find_package(Doxygen REQUIRED)
    doxygen_add_docs(docs include)
endif()


=======
add_subdirectory(src)
>>>>>>> c6974f1aefbdf4da1c1a0abe17222de4b8f57795

