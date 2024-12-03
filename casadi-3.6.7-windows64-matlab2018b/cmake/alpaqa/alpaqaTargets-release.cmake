#----------------------------------------------------------------
# Generated CMake target import file for configuration "Release".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "alpaqa::alpaqa" for configuration "Release"
set_property(TARGET alpaqa::alpaqa APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(alpaqa::alpaqa PROPERTIES
  IMPORTED_IMPLIB_RELEASE "${_IMPORT_PREFIX}/lib/libalpaqa.dll.a"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/bin/libalpaqa.dll"
  )

list(APPEND _IMPORT_CHECK_TARGETS alpaqa::alpaqa )
list(APPEND _IMPORT_CHECK_FILES_FOR_alpaqa::alpaqa "${_IMPORT_PREFIX}/lib/libalpaqa.dll.a" "${_IMPORT_PREFIX}/bin/libalpaqa.dll" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
