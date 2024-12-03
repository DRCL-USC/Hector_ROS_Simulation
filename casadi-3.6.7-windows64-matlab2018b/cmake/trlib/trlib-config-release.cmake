#----------------------------------------------------------------
# Generated CMake target import file for configuration "Release".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "trlib::trlib" for configuration "Release"
set_property(TARGET trlib::trlib APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(trlib::trlib PROPERTIES
  IMPORTED_IMPLIB_RELEASE "${_IMPORT_PREFIX}/lib/libtrlib.dll.a"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/libtrlib.dll"
  )

list(APPEND _IMPORT_CHECK_TARGETS trlib::trlib )
list(APPEND _IMPORT_CHECK_FILES_FOR_trlib::trlib "${_IMPORT_PREFIX}/lib/libtrlib.dll.a" "${_IMPORT_PREFIX}/lib/libtrlib.dll" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
