#----------------------------------------------------------------
# Generated CMake target import file for configuration "Release".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "sleqp::sleqp" for configuration "Release"
set_property(TARGET sleqp::sleqp APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(sleqp::sleqp PROPERTIES
  IMPORTED_IMPLIB_RELEASE "${_IMPORT_PREFIX}/lib/libsleqp.dll.a"
  IMPORTED_LINK_DEPENDENT_LIBRARIES_RELEASE "trlib::trlib"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/bin/libsleqp.dll"
  )

list(APPEND _IMPORT_CHECK_TARGETS sleqp::sleqp )
list(APPEND _IMPORT_CHECK_FILES_FOR_sleqp::sleqp "${_IMPORT_PREFIX}/lib/libsleqp.dll.a" "${_IMPORT_PREFIX}/bin/libsleqp.dll" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
