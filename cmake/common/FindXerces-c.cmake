#.rst:
# FindXerces-c
# --------
#
# Find Xerces-c
#
# Find the native Xerces-c headers and libraries.
#
# ::
#
#   Xerces-c_INCLUDE_DIRS   - where to find Xerces-c/Xerces-c.h, etc.
#   Xerces-c_LIBRARIES      - List of libraries when using Xerces-c.
#   Xerces-c_FOUND          - True if Xerces-c found.
#   Xerces-c_VERSION_STRING - the version of Xerces-c found (since CMake 2.8.8)

#=============================================================================
# Copyright 2006-2009 Kitware, Inc.
# Copyright 2012 Rolf Eike Beer <eike@sf-mail.de>
#
# Distributed under the OSI-approved BSD License (the "License");
# see accompanying file Copyright.txt for details.
#
# This software is distributed WITHOUT ANY WARRANTY; without even the
# implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
# See the License for more information.
#=============================================================================
# (To distribute this file outside of CMake, substitute the full
#  License text for the above reference.)

# Look for the header file.
find_path(Xerces-c_INCLUDE_DIR 
      NAMES dom/DOM.hpp
      PATH_SUFFIXES
       xercesc
       include/xercesc
       DOC "Xerces Include Path"
)
# Look for the library (sorted from most current/relevant entry to least).

#This is a new attempt to try and help static libraries be detected over dynamics
#CMAKE is really bad at this so lookout for bugs

set(OLD_CMAKE_FIND_LIBRARY_SUFFIXES ${CMAKE_FIND_LIBRARY_SUFFIXES})
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
set(CMAKE_FIND_LIBRARY_SUFFIXES ${OLD_CMAKE_FIND_LIBRARY_SUFFIXES})

if( Xerces-c_LIBRARY_RELEASE_STATIC  AND NOT Xerces-c_LIBRARY_RELEASE_DYNAMIC)
  set(Xerces-c_LIBRARY_RELEASE ${Xerces-c_LIBRARY_RELEASE_STATIC} )
  set(Xerces-c_LIBRARY_DEBUG   ${Xerces-c_LIBRARY_DEBUG_STATIC}   )
  set(Xerces-c_CPPFLAGS "-DXERCES_STATIC_LIBRARY")
elseif(Xerces-c_LIBRARY_RELEASE_DYNAMIC) 
  set(Xerces-c_LIBRARY_RELEASE ${Xerces-c_LIBRARY_RELEASE_DYNAMIC} )
  set(Xerces-c_LIBRARY_DEBUG   ${Xerces-c_LIBRARY_DEBUG_DYNAMIC}  )
else()
  set(Xerces-c_LIBRARY_RELEASE Xerces-c_LIBRARY_RELEASE-NOTFOUND )
  set(Xerces-c_LIBRARY_DEBUG   Xerces-c_LIBRARY_DEBUG-NOTFOUND   )
endif()



#if(ANDROID_ABI)
#    find_library(ICU_DATA_LIBRARY_RELEASE NAMES
#      icudata 
#      libicudata  
#      PATH_SUFFIX release
#      
#      DOC "ICU data Library needed on Android and Linux"
#    )
#    find_library(ICU_DATA_LIBRARY_DEBUG NAMES
#      icudata_d 
#      libicudata_d 
#      icudatad 
#      libicudatad  
#      icudata 
#      libicudata  
#      PATH_SUFFIX debug
#      DOC "ICU data Library needed on Android and Linux"
#    )
#    find_library(ICU_LIBRARY_RELEASE NAMES
#      icuuc 
#      libicuuc  
#      PATH_SUFFIX release
#      
#      DOC "LIbrary ICU needed on Android and Linux"
#    )
#    find_library(ICU_LIBRARY_DEBUG NAMES
#      icuuc_d 
#      libicuuc_d 
#      icuucd 
#      libicuucd  
#      icuuc 
#      libicuuc  
#      PATH_SUFFIX debug
#      DOC "ICU Library needed on Android and Linux"
#    )
#endif()

# handle the QUIETLY and REQUIRED arguments and set Xerces-c_FOUND to TRUE if
# all listed variables are TRUE
include(FindPackageHandleStandardArgs)

if(ANDROID_ABI)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(Xerces-c
                                  REQUIRED_VARS Xerces-c_LIBRARY_DEBUG 
                                                Xerces-c_LIBRARY_RELEASE 
#                                                ICU_LIBRARY_RELEASE
#                                                ICU_LIBRARY_DEBUG
#                                                ICU_DATA_LIBRARY_RELEASE
#                                                ICU_DATA_LIBRARY_DEBUG
                                                Xerces-c_INCLUDE_DIR
                                  )
else()
FIND_PACKAGE_HANDLE_STANDARD_ARGS(Xerces-c
                                  REQUIRED_VARS Xerces-c_LIBRARY_DEBUG 
                                                Xerces-c_LIBRARY_RELEASE 
                                                Xerces-c_INCLUDE_DIR
                                  )

endif()


if(Xerces-c_FOUND)
  if(ANDROID_ABI)
    set(Xerces-c_LIBRARIES 
             optimized ${Xerces-c_LIBRARY_RELEASE} debug ${Xerces-c_LIBRARY_DEBUG}
             #optimized ${ICU_LIBRARY_RELEASE}      debug ${ICU_LIBRARY_DEBUG}
             #optimized ${ICU_DATA_LIBRARY_RELEASE} debug ${ICU_DATA_LIBRARY_DEBUG}
       )
  else()
    set(Xerces-c_LIBRARIES optimized ${Xerces-c_LIBRARY_RELEASE} debug ${Xerces-c_LIBRARY_DEBUG})
    message(STATUS "set(Xerces-c_LIBRARIES optimized ${Xerces-c_LIBRARY_RELEASE} debug ${Xerces-c_LIBRARY_DEBUG})")
  endif()
  
  set(Xerces-c_INCLUDE_DIRS ${Xerces-c_INCLUDE_DIR})
  mark_as_advanced(Xerces-c_LIBRARY_DEBUG_STATIC)
  mark_as_advanced(Xerces-c_LIBRARY_RELEASE_STATIC)
  mark_as_advanced(Xerces-c_LIBRARY_DEBUG_DYNAMIC) 
  mark_as_advanced(Xerces-c_LIBRARY_RELEASE_DYNAMIC)
  mark_as_advanced(ICU_LIBRARY_RELEASE)
  mark_as_advanced(ICU_LIBRARY_DEBUG)
  mark_as_advanced(Xerces-c_INCLUDE_DIR)
endif()
