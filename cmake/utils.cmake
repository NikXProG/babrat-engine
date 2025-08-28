
# Set variable if it is not defined.
macro(set_if_undefined variable)
    if(NOT DEFINED "${variable}")
        set("${variable}" ${ARGN})
    endif()
endmacro()

# Downloads the external dependency repo at the specified commit and calls register_dependency
macro(register_dependency
        NAME
        URL
        COMMIT)

    include(FetchContent)

    set(DEP_NAME ${NAME}_dependency_fc)

    FetchContent_Declare(
            ${DEP_NAME}
            GIT_REPOSITORY ${URL}
            GIT_TAG ${COMMIT}
            GIT_SHALLOW true
    )

    FetchContent_MakeAvailable(${DEP_NAME})

    message(STATUS "Load extension '${NAME}' from ${URL} with tag ${COMMIT}")
    message(STATUS "Dependency ${NAME} downloaded to: ${${DEP_NAME}_SOURCE_DIR}")
    message(STATUS "Binary dir for ${NAME}: ${${DEP_NAME}_BINARY_DIR}")

endmacro()


function(win_copy_deps_to_target_dir target)

    if(NOT WIN32)
        return()
    endif()

    set(has_runtime_dll_genex NO)

    if(CMAKE_MAJOR_VERSION GREATER 3 OR CMAKE_MINOR_VERSION GREATER_EQUAL 21)
        set(has_runtime_dll_genex YES)

        add_custom_command(TARGET ${target} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -P "${babrat_SOURCE_DIR}/cmake/silent_copy.cmake"
                "$<TARGET_RUNTIME_DLLS:${target}>" "$<TARGET_FILE_DIR:${target}>"
                COMMAND_EXPAND_LISTS)
    endif()

    foreach(dep ${ARGN})
        get_target_property(dep_type ${dep} TYPE)

        if(dep_type STREQUAL "SHARED_LIBRARY")
            if(NOT has_runtime_dll_genex)
                add_custom_command(TARGET ${target} POST_BUILD
                        COMMAND ${CMAKE_COMMAND} -P "${babrat_SOURCE_DIR}/cmake/silent_copy.cmake"
                        "$<TARGET_FILE:${dep}>" "$<TARGET_PDB_FILE:${dep}>" "$<TARGET_FILE_DIR:${target}>"
                        COMMAND_EXPAND_LISTS)
            else()
                add_custom_command(TARGET ${target} POST_BUILD
                        COMMAND ${CMAKE_COMMAND} -P "${babrat_SOURCE_DIR}/cmake/silent_copy.cmake"
                        "$<TARGET_PDB_FILE:${dep}>" "$<TARGET_FILE_DIR:${target}>"
                        COMMAND_EXPAND_LISTS)
            endif()
        endif()
    endforeach()

endfunction()