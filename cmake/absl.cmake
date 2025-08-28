find_package(absl REQUIRED)

function(add_absl_if_needed)


    if(CMAKE_HOST_APPLE)

        find_program(HOMEBREW_EXECUTABLE brew)
        if(HOMEBREW_EXECUTABLE)
            execute_process(
                    COMMAND ${HOMEBREW_EXECUTABLE} --prefix absl
                    OUTPUT_VARIABLE ABSL_ROOT
                    ERROR_QUIET
                    OUTPUT_STRIP_TRAILING_WHITESPACE
            )
            if(ABSL_ROOT)
                list(APPEND CMAKE_PREFIX_PATH "${ABSL_ROOT}")
                message(STATUS "Added Homebrew Abseil path: ${ABSL_ROOT}")
            endif()
        endif()
    endif()


    find_package(absl REQUIRED)

    if(NOT absl_FOUND)
        message(FATAL_ERROR "Abseil (absl) not found!")
    endif()

    if(NOT TARGET absl::strings)
        message(FATAL_ERROR "Abseil strings component not found!")
    endif()
endfunction()