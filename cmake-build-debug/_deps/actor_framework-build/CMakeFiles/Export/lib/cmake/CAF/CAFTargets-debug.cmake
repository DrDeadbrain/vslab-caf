#----------------------------------------------------------------
# Generated CMake target import file for configuration "Debug".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "CAF::core" for configuration "Debug"
set_property(TARGET CAF::core APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(CAF::core PROPERTIES
  IMPORTED_LOCATION_DEBUG "${_IMPORT_PREFIX}/lib/libcaf_core.so.0.18.0"
  IMPORTED_SONAME_DEBUG "libcaf_core.so.0.18.0"
  )

list(APPEND _IMPORT_CHECK_TARGETS CAF::core )
list(APPEND _IMPORT_CHECK_FILES_FOR_CAF::core "${_IMPORT_PREFIX}/lib/libcaf_core.so.0.18.0" )

# Import target "CAF::io" for configuration "Debug"
set_property(TARGET CAF::io APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(CAF::io PROPERTIES
  IMPORTED_LOCATION_DEBUG "${_IMPORT_PREFIX}/lib/libcaf_io.so.0.18.0"
  IMPORTED_SONAME_DEBUG "libcaf_io.so.0.18.0"
  )

list(APPEND _IMPORT_CHECK_TARGETS CAF::io )
list(APPEND _IMPORT_CHECK_FILES_FOR_CAF::io "${_IMPORT_PREFIX}/lib/libcaf_io.so.0.18.0" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
