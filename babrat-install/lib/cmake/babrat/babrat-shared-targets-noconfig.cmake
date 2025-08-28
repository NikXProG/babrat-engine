#----------------------------------------------------------------
# Generated CMake target import file.
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "babrat::babrat" for configuration ""
set_property(TARGET babrat::babrat APPEND PROPERTY IMPORTED_CONFIGURATIONS NOCONFIG)
set_target_properties(babrat::babrat PROPERTIES
  IMPORTED_LOCATION_NOCONFIG "${_IMPORT_PREFIX}/lib/libbabrat.1.0.0.dylib"
  IMPORTED_SONAME_NOCONFIG "@rpath/libbabrat.1.dylib"
  )

list(APPEND _cmake_import_check_targets babrat::babrat )
list(APPEND _cmake_import_check_files_for_babrat::babrat "${_IMPORT_PREFIX}/lib/libbabrat.1.0.0.dylib" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
