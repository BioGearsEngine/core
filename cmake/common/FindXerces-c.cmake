# Find Xerces-c
#
# Find the native Xerces-c headers and libraries.
#
#...Defines an Import Target Xerces::Xerces-c
#...Xerces-c_FOUND          - True if Xerces-c found.
#   Xerces-c_VERSION_STRING - the version of Xerces-c found (since CMake 2.8.8)
if(NOT Xerces-c_FOUND)
# Look for the header file.
find_path(Xerces-c_INCLUDE_DIR 
      NAMES xercesc/dom/DOM.hpp
      PATH_SUFFIXES
       xercesc
       include
       DOC "Xerces Include Path"
)
# Look for the library (sorted from most current/relevant entry to least).

#This is a new attempt to try and help static libraries be detected over dynamics
#CMAKE is really bad at this so lookout for bugs

set(OLD_CMAKE_FIND_LIBRARY_SUFFIXES ${CMAKE_FIND_LIBRARY_SUFFIXES})
if(Xerces-c_USE_STATIC_LIBRARY)
  set(CMAKE_FIND_LIBRARY_SUFFIXES .a .lib)
  find_library(Xerces-c_LIBRARY_RELEASE_STATIC NAMES
        xerces-c_static_3
        libxerces-c_static_3
        xerces-c_static_3_2
        libxerces-c_static_3_2
        xerces-c
        libxerces-c
    DOC "Xerces Release Library. Prefered over DLL"
  )
  # Look for the library (sorted from most current/relevant entry to least).
  find_library(Xerces-c_LIBRARY_DEBUG_STATIC NAMES
        xerces-c_static_3D
        libxerces-c_static_3D
        xerces-c_3_static_d
        libxerces-c_static_3_d
        xerces-c_static_3
        libxerces-c_static_3
        libxerces-c_static_3_2D
        xerces-c_3_2_static_d
        libxerces-c_static_3_2_d
        xerces-c_static_3_2
        libxerces-c_static_3_2
        xerces-c_static_d
        libxerces-c_static_D
        xerces-c
        libxerces-c
    PATH_SUFFIX  debug
                 lib
                 lib/debug
    DOC "Xerces Debug Library. Prefered over DLL"
  )
else()
  set(CMAKE_FIND_LIBRARY_SUFFIXES .so .lib .dylib)
  if(NOT Xerces-c_LIBRARY_RELEASE_STATIC ) 
      # Look for the library (sorted from most current/relevant entry to least).
    find_library(Xerces-c_LIBRARY_RELEASE_DYNAMIC NAMES
        xerces-c_3
        libxerces-c_3
        xerces-c_3_2
        libxerces-c_3_2
        xerces-c
        libxerces-c
      PATH_SUFFIX  release
                   lib
                   lib/release
      DOC "Xerces DLL Based Release Library. Only searched if Static does not exist"
    )
    # Look for the library (sorted from most current/relevant entry to least).
    find_library(Xerces-c_LIBRARY_DEBUG_DYNAMIC NAMES
        xerces-c_3D
        libxerces-c_3D
        xerces-c_3_d
        libxerces-c_3_d
        xerces-c_3
        libxerces-c_3
        xerces-c_d
        libxerces-c_D
        xerces-c
        libxerces-c
        xerces-c_3_2D
        libxerces-c_3_2D
        xerces-c_3_2_d
        libxerces-c_3_2_d
        xerces-c_3_2
        libxerces-c_3_2
      PATH_SUFFIX  debug
                   lib
                   lib/debug
      DOC "Xerces DLL Based Debug Library. Only searched if Static does not exist"
    )
  endif()
endif()
set(CMAKE_FIND_LIBRARY_SUFFIXES ${OLD_CMAKE_FIND_LIBRARY_SUFFIXES})

if( Xerces-c_LIBRARY_RELEASE_STATIC  AND NOT Xerces-c_LIBRARY_RELEASE_DYNAMIC)
  set(Xerces-c_LIBRARY_RELEASE ${Xerces-c_LIBRARY_RELEASE_STATIC} )
  set(Xerces-c_LIBRARY_DEBUG   ${Xerces-c_LIBRARY_DEBUG_STATIC}   )
  set(Xerces-c_CPPFLAGS "-DXERCES_STATIC_LIBRARY")
  set(LIBRARY_TYPE STATIC)
elseif(Xerces-c_LIBRARY_RELEASE_DYNAMIC) 
  set(Xerces-c_LIBRARY_RELEASE ${Xerces-c_LIBRARY_RELEASE_DYNAMIC} )
  set(Xerces-c_LIBRARY_DEBUG   ${Xerces-c_LIBRARY_DEBUG_DYNAMIC}  )
  set(LIBRARY_TYPE SHARED)
else()
  set(Xerces-c_LIBRARY_RELEASE Xerces-c_LIBRARY_RELEASE-NOTFOUND )
  set(Xerces-c_LIBRARY_DEBUG   Xerces-c_LIBRARY_DEBUG-NOTFOUND   )
endif()


# handle the QUIETLY and REQUIRED arguments and set Xerces-c_FOUND to TRUE if
# all listed variables are TRUE
include(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(Xerces-c
                                  REQUIRED_VARS Xerces-c_INCLUDE_DIR 
                                                Xerces-c_LIBRARY_RELEASE
                                                Xerces-c_LIBRARY_DEBUG
                                  VERSION_VAR Xerces-c_VERSION_STRING)

if(Xerces-c_FOUND)
  add_library(Xerces::xerces ${LIBRARY_TYPE} IMPORTED GLOBAL)
  set_target_properties(Xerces::xerces
    PROPERTIES
    IMPORTED_LOCATION       ${Xerces-c_LIBRARY_RELEASE}   
    IMPORTED_LOCATION_DEBUG ${Xerces-c_LIBRARY_DEBUG}
    IMPORTED_IMPLIB_DEBUG   ${Xerces-c_LIBRARY_DEBUG}
    IMPORTED_IMPLIB ${Xerces-c_LIBRARY_RELEASE}
    INTERFACE_INCLUDE_DIRECTORIES
    ${Xerces-c_INCLUDE_DIR}
    )
   target_compile_definitions(Xerces::xerces INTERFACE ${Xerces-c_CPPFLAGS})
  


  mark_as_advanced(Xerces-c_LIBRARY_DEBUG_STATIC)
  mark_as_advanced(Xerces-c_LIBRARY_RELEASE_STATIC)
  mark_as_advanced(Xerces-c_LIBRARY_DEBUG_DYNAMIC) 
  mark_as_advanced(Xerces-c_LIBRARY_RELEASE_DYNAMIC)
  mark_as_advanced(Xerces-c_INCLUDE_DIR)
  mark_as_advanced(Xerces-c_DIR)
endif()
endif()
