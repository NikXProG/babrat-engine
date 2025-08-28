function(add_boost_if_needed)
    # Try to find Homebrew's Boost on macOS
    if(CMAKE_HOST_APPLE)
        find_program(HOMEBREW_EXECUTABLE brew)
        if(HOMEBREW_EXECUTABLE)
            execute_process(
                    COMMAND ${HOMEBREW_EXECUTABLE} --prefix boost
                    OUTPUT_VARIABLE Boost_ROOT
                    ERROR_QUIET
                    OUTPUT_STRIP_TRAILING_WHITESPACE
            )
            message(STATUS "Found Homebrew Boost at: ${Boost_ROOT}")
        endif()
    endif()

    cmake_policy(SET CMP0167 NEW)

    # Find Boost package - we only need headers for flyweight
    find_package(Boost 1.88.0 REQUIRED COMPONENTS headers)

    if(NOT Boost_FOUND)
        message(FATAL_ERROR "Boost not found!")
    endif()

    message(STATUS "Boost version: ${Boost_VERSION}")
    message(STATUS "Boost include dirs: ${Boost_INCLUDE_DIRS}")

endfunction()