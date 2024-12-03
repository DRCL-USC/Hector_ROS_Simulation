#----------------------------------------------------------------
# Generated CMake target import file for configuration "Release".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "highs::highs" for configuration "Release"
set_property(TARGET highs::highs APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(highs::highs PROPERTIES
  IMPORTED_IMPLIB_RELEASE "${_IMPORT_PREFIX}/lib/libhighs.dll.a"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/bin/libhighs.dll"
  )

list(APPEND _IMPORT_CHECK_TARGETS highs::highs )
list(APPEND _IMPORT_CHECK_FILES_FOR_highs::highs "${_IMPORT_PREFIX}/lib/libhighs.dll.a" "${_IMPORT_PREFIX}/bin/libhighs.dll" )

# Import target "highs::highs-bin" for configuration "Release"
set_property(TARGET highs::highs-bin APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(highs::highs-bin PROPERTIES
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/bin/highs.exe"
  )

list(APPEND _IMPORT_CHECK_TARGETS highs::highs-bin )
list(APPEND _IMPORT_CHECK_FILES_FOR_highs::highs-bin "${_IMPORT_PREFIX}/bin/highs.exe" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
