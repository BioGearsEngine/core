#.rst:
# FindLog4cpp
# --------
#
# Find Log4cpp
#
# Find the native Log4cpp headers and libraries.
#
# ::
#
#   Log4cpp_INCLUDE_DIRS   - where to find Log4cpp/Log4cpp.h, etc.
#   Log4cpp_LIBRARIES      - List of libraries when using Log4cpp.
#   Log4cpp_FOUND          - True if Log4cpp found.
#   Log4cpp_VERSION_STRING - the version of Log4cpp found (since CMake 2.8.8)
if(NOT Log4cpp_FOUND)
  # Look for the header file.
  find_path(Log4cpp_INCLUDE_DIR 
        NAMES log4cpp/Category.hh
        PATH_SUFFIXES
         log4cpp
         include/log4cpp
         DOC "Log4cpp Include Path"
  )
  # Look for the library (sorted from most current/relevant entry to least).
  find_library(Log4cpp_LIBRARY_RELEASE NAMES
      log4cpp
      liblog4cpp
      Log4cpp
      libLog4cpp
  
    PATH_SUFFIX  release
    DOC "Log4cpp Release Library. Prefered over DLL"
  )
  # Look for the library (sorted from most current/relevant entry to least).
  find_library(Log4cpp_LIBRARY_DEBUG NAMES
      log4cpp_d
      liblog4cpp_d
      log4cppD
      liblog4cppD
      log4cpp
      liblog4cpp
      Log4cpp_d
      LibLog4cpp_d
      Log4cppD
      libLog4cppD
      Log4cpp
      libLog4cpp
    PATH_SUFFIX  debug
    DOC "Log4cpp Debug Library. Prefered over DLL"
  )
  
  
  # handle the QUIETLY and REQUIRED arguments and set Log4cpp_FOUND to TRUE if
  # all listed variables are TRUE
  include(FindPackageHandleStandardArgs)
  
  FIND_PACKAGE_HANDLE_STANDARD_ARGS(Log4cpp
                                    REQUIRED_VARS Log4cpp_LIBRARY_DEBUG 
                                                  Log4cpp_LIBRARY_RELEASE 
                                                  Log4cpp_INCLUDE_DIR
                                    )
  
  if(Log4cpp_FOUND)
    add_library(Log4cpp::Log4cpp SHARED IMPORTED GLOBAL)
    set_target_properties(Log4cpp::Log4cpp
          PROPERTIES
          IMPORTED_LOCATION ${Log4cpp_LIBRARY_RELEASE}
          IMPORTED_LOCATION_DEBUG ${Log4cpp_LIBRARY_DEBUG}
          IMPORTED_IMPLIB_DEBUG   ${Log4cpp_LIBRARY_DEBUG}
          IMPORTED_IMPLIB         ${Log4cpp_LIBRARY_RELEASE}
          INTERFACE_INCLUDE_DIRECTORIES
          ${Log4cpp_INCLUDE_DIR}
          )
  
    mark_as_advanced(Log4cpp_LIBRARY_DEBUG)
    mark_as_advanced(Log4cpp_LIBRARY_RELEASE)
    mark_as_advanced(Log4cpp_INCLUDE_DIR)
	mark_as_advanced(Log4cpp_DIR)
  endif()
endif()
