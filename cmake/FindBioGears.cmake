#.rst:
# FindBioGears
# --------
#
# Find BioGears
#
# Find the native BioGears headers and libraries.
#
# ::
#
#   BioGears_INCLUDE_DIRS   - where to find BioGears/BioGears.h, etc.
#   BioGears_LIBRARIES      - List of libraries when using BioGears.
#   BioGears_FOUND          - True if BioGears found.
#   BioGears_VERSION_STRING - the version of BioGears found (since CMake 2.8.8)


# Look for the header file.
find_path(BioGears_INCLUDE_DIR 
      NAMES biogears/exports.h
      PATH_SUFFIXES
       biogears
       include/biogears
       DOC "BioGears Include Path"
)

if(WIN32)
# Look for the library (sorted from most current/relevant entry to least).
find_library(BioGears_String_LIBRARY_RELEASE NAMES
    biogears_string
    libbiogears_string
  HINTS ${BioGears_INCLUDE_DIR}/../lib/
  PATH_SUFFIX  release 
  DOC "BioGears Release Library. Prefered over DLL"
)
# Look for the library (sorted from most current/relevant entry to least).
find_library(BioGears_String_LIBRARY_DEBUG NAMES
    biogears_string_d
    libbiogears_string_d
    biogears_string
    libbiogears_string
  HINTS ${BioGears_INCLUDE_DIR}/../lib/
  PATH_SUFFIX  debug
  DOC "BioGears Debug Library. Prefered over DLL"
)
endif()

# Look for the library (sorted from most current/relevant entry to least).
find_library(BioGears_CDM_LIBRARY_RELEASE NAMES
    biogears_cdm
    libbiogears_cdm
  HINTS ${BioGears_INCLUDE_DIR}/../lib/
  PATH_SUFFIX  release
  DOC "BioGears Release Library. Prefered over DLL"
)
# Look for the library (sorted from most current/relevant entry to least).
find_library(BioGears_CDM_LIBRARY_DEBUG NAMES
    biogears_cdm_d
    libbiogears_cdm_d
    biogears_cdm
    libbiogears_cdm
  HINTS ${BioGears_INCLUDE_DIR}/../lib/
  PATH_SUFFIX  debug
  DOC "BioGears Debug Library. Prefered over DLL"
)
# Look for the library (sorted from most current/relevant entry to least).
find_library(BioGears_LIBRARY_RELEASE NAMES
    biogears
    libbiogears
  HINTS ${BioGears_INCLUDE_DIR}/../lib/
  PATH_SUFFIX  release
  DOC "BioGears Release Library. Prefered over DLL"
)
# Look for the library (sorted from most current/relevant entry to least).
find_library(BioGears_LIBRARY_DEBUG NAMES
    biogears_d
    libbiogears_d
    biogears
    libbiogears
  HINTS ${BioGears_INCLUDE_DIR}/../lib/
  PATH_SUFFIX  debug
  DOC "BioGears Debug Library. Prefered over DLL"
)


# handle the QUIETLY and REQUIRED arguments and set BioGears_FOUND to TRUE if
# all listed variables are TRUE
include(FindPackageHandleStandardArgs)

if(WIN32)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(BioGears
                                  REQUIRED_VARS 
                                                BioGears_String_LIBRARY_DEBUG 
                                                BioGears_String_LIBRARY_RELEASE 
                                                BioGears_CDM_LIBRARY_DEBUG 
                                                BioGears_CDM_LIBRARY_RELEASE 
                                                BioGears_LIBRARY_DEBUG 
                                                BioGears_LIBRARY_RELEASE 
                                                BioGears_INCLUDE_DIR
                                  )
else()
FIND_PACKAGE_HANDLE_STANDARD_ARGS(BioGears
                                  REQUIRED_VARS 
                                                BioGears_CDM_LIBRARY_DEBUG 
                                                BioGears_CDM_LIBRARY_RELEASE 
                                                BioGears_LIBRARY_DEBUG 
                                                BioGears_LIBRARY_RELEASE 
                                                BioGears_INCLUDE_DIR
                                 )

endif()

if(BioGears_FOUND)
  if(WIN32)
    set(BioGears_LIBRARIES 
           optimized ${BioGears_String_LIBRARY_RELEASE} debug ${BioGears_String_LIBRARY_DEBUG}
           optimized ${BioGears_CDM_LIBRARY_RELEASE} debug ${BioGears_CDM_LIBRARY_DEBUG}
           optimized ${BioGears_LIBRARY_RELEASE} debug ${BioGears_LIBRARY_DEBUG}
           )
  else()
    set(BioGears_LIBRARIES 
           optimized ${BioGears_CDM_LIBRARY_RELEASE} debug ${BioGears_CDM_LIBRARY_DEBUG}
           optimized ${BioGears_LIBRARY_RELEASE} debug ${BioGears_LIBRARY_DEBUG}
           )
  endif()
  set(BioGears_INCLUDE_DIRS ${BioGears_INCLUDE_DIR})
  mark_as_advanced(BioGears_LIBRARY_DEBUG)
  mark_as_advanced(BioGears_LIBRARY_RELEASE)
  mark_as_advanced(BioGears_INCLUDE_DIR)
endif()
 message(FATAL_ERROR "BioGears was not found. You can use BioGears_INCLUDE_DIR as a HINT to where the Biogears libraries are located") 
