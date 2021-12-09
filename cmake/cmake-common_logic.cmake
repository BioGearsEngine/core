
if( NOT CMAKE_DEBUG_POSTFIX AND NOT Biogears_NO_DEBUG_POSTIFX)
  set( CMAKE_DEBUG_POSTFIX "_d" CACHE STRING "This string is appended to target names in debug mode." FORCE )
endif()
if( NOT CMAKE_BUILD_TYPE )
  set(CMAKE_BUILD_TYPE Debug FORCE )
  set(BUILD_OPTIONS_STRINGS
    "Debug"
    "Release"
  ) 
  set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Release;Debug" FORCE)
  set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS ${BUILD_OPTIONS_STRINGS})
endif()

if (WIN32 AND CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    set (CMAKE_INSTALL_PREFIX "install" CACHE PATH "default install path" FORCE )
endif()

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

set(CMAKE_CXX_USE_RESPONSE_FILE_FOR_OBJECTS 1)
set(CMAKE_CXX_USE_RESPONSE_FILE_FOR_INCLUDES 1)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set_property(GLOBAL PROPERTY AUTOGEN_TARGETS_FOLDER "Code Generators" )
set_property(GLOBAL PROPERTY AUTOGEN_SOURCE_GROUP  "Generated")

####
#
#  A simple macro to check for PACKAGE_FOUND and warn the user to 
#
####
function(verify_package package)
  cmake_parse_arguments(_l
    REQUIRED 
    ""
    ""
     ${ARGN})
  find_package(${package} CONFIG ${_l_UNPARSED_ARGUMENTS} QUIET)
  if(NOT ${package}_FOUND)
    find_package(${package} ${_l_UNPARSED_ARGUMENTS} QUIET)
    if(NOT ${package}_FOUND)
      message(WARNING "The following packages ${package} were not found."
        "For native compilations setting CMAKE_PREFIX_PATH can solve this problem"
        "For cross complilation try expanding your CMAKE_FIND_ROOT_PATH"
        "")
      if(_l_REQUIRED)
        message(FATAL_ERROR "Unable to find ${package} which is marked REQUIRED")
      endif()
    endif()
  endif()
  set(${package}_FOUND ${${package}_FOUND} PARENT_SCOPE)
endfunction(verify_package)
####
#
#  Simple macro for taking a list of header files and asking them to be included
#
####
function(install_headers header_list out_dir)

    FOREACH(HEADER ${${header_list}})
        STRING(REGEX MATCH "(.\\\*)\\\[/\\\]" DIR ${HEADER})
        INSTALL(FILES ${HEADER} DESTINATION ${out_dir}/${DIR})
    ENDFOREACH(HEADER)

endfunction()
########################################################################################################
# 
# Source File Managment Macros
# 
########################################################################################################
function(CHILDLIST result curdir)
  file(GLOB children RELATIVE ${curdir} ${curdir}/*)
  set(dirlist "")
  foreach(child ${children})
    if(IS_DIRECTORY ${curdir}/${child})
      list(APPEND dirlist ${child})
    endif()
  endforeach()
  set(${result} ${dirlist} PARENT_SCOPE)
endfunction()


function(add_source_files var)
  set(__prefix "_l")
  set(__options "DEBUG")
  set(__one_value "LOCATION;SOURCE_GROUP")
  set(__multi_value "REGEX")
  cmake_parse_arguments( "_l" "DEBUG" "LOCATION;SOURCE_GROUP" "REGEX" ${ARGN})
    if(_l_DEBUG)
      message(STATUS "add_source_files( ${var} LOCATION \"${_l_LOCATION}\" REGEX \"${_l_REGEX}\" SOURCE_GROUP \"${_l_SOURCE_GROUP}\")")
    endif()
    set(TEMP "")
    set(__new_entries "")
    foreach(regex IN LISTS _l_REGEX)
      file(GLOB TEMP "${_l_LOCATION}/${regex}")
      list(APPEND __new_entries ${TEMP})
    endforeach()
    if(_l_SOURCE_GROUP)
      source_group("${_l_SOURCE_GROUP}" FILES ${__new_entries})
    endif()
    if(_l_DEBUG)
      message(STATUS "CHILDLIST( result ${_l_LOCATION})")
    endif()
    CHILDLIST( result ${_l_LOCATION})
    

    foreach( dir IN LISTS result)
      if( _l_DEBUG)
        add_source_files( ${var} LOCATION "${_l_LOCATION}/${dir}" REGEX ${_l_REGEX} SOURCE_GROUP "${_l_SOURCE_GROUP}\\${dir}" DEBUG)
      else()
        add_source_files( ${var} LOCATION "${_l_LOCATION}/${dir}" REGEX ${_l_REGEX} SOURCE_GROUP "${_l_SOURCE_GROUP}\\${dir}")
      endif()
    endforeach()

    set(${var} ${${var}} ${__new_entries} PARENT_SCOPE)
endfunction()


########################################################################################################
#
# Remove Duplicate LIbraries
#
########################################################################################################
function(remove_duplicate_libraries libraries)
  list(LENGTH ${libraries} LIST_LENGTH)
  while( ${libraries} )
     list(GET ${libraries} 0 item )
     if( item STREQUAL debug)
        list(GET ${libraries} 1 item )
        list(REMOVE_AT ${libraries} 0 1 )
        list(APPEND debug_libraries  ${item})
     elseif( item STREQUAL optimized)
        list(GET ${libraries} 1 item )
        list(REMOVE_AT ${libraries} 0 1 )
        list(APPEND release_libraries  ${item})
     else()
        list(REMOVE_AT ${libraries} 0 )
        list(APPEND common_libraries ${item})
     endif()  
  endwhile()
  if(common_libraries)
    list(REMOVE_DUPLICATES common_libraries)
  endif()
  if(release_libraries)
    list(REMOVE_DUPLICATES release_libraries)
   endif()
  if(debug_libraries)
    list(REMOVE_DUPLICATES debug_libraries)
   endif()
  set( results )
  foreach( item IN LISTS release_libraries)
    list(APPEND results "optimized" ${item})
  endforeach()
  
  foreach( item IN LISTS debug_libraries)
    list(APPEND results "debug" ${item})
  endforeach()
  
  set( ${libraries} ${common_libraries} ${results} PARENT_SCOPE)
endfunction()
########################################################################################################
# 
# Stage Macros
# 
########################################################################################################
function(create_cache_file)
  set(CacheForScript ${CMAKE_BINARY_DIR}/cmake-common_cache.cmake)
  set(OUTPUT_PREFIX ${CMAKE_BINARY_DIR}/outputs)
  file(WRITE ${CacheForScript} "")
  file(APPEND ${CacheForScript} "set(ROOT_PROJECT_NAME ${ROOT_PROJECT_NAME})\n")
  file(APPEND ${CacheForScript} "set(PROJECT_SOURCE_DIR \"${PROJECT_SOURCE_DIR}\")\n")
  file(APPEND ${CacheForScript} "set(CMAKE_BINARY_DIR \"${CMAKE_BINARY_DIR}\")\n")
  file(APPEND ${CacheForScript} "set(CMAKE_PREFIX_PATH \"${CMAKE_PREFIX_PATH}\")\n")
  file(APPEND ${CacheForScript} "set(CMAKE_EXECUTABLE_SUFFIX ${CMAKE_EXECUTABLE_SUFFIX})\n")
  file(APPEND ${CacheForScript} "set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${OUTPUT_PREFIX}/lib)\n")
  file(APPEND ${CacheForScript} "set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${OUTPUT_PREFIX}/lib)\n")
  file(APPEND ${CacheForScript} "set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${OUTPUT_PREFIX}/bin)\n")
  foreach(OUTPUTCONFIG ${CMAKE_CONFIGURATION_TYPES})
        string(TOUPPER _${OUTPUTCONFIG} OUTPUTCONFIG_UPPER)
        file(APPEND ${CacheForScript} "set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY${OUTPUTCONFIG_UPPER} ${OUTPUT_PREFIX}/${OUTPUTCONFIG}/lib)\n")
        file(APPEND ${CacheForScript} "set(CMAKE_LIBRARY_OUTPUT_DIRECTORY${OUTPUTCONFIG_UPPER} ${OUTPUT_PREFIX}/${OUTPUTCONFIG}/lib)\n")
        file(APPEND ${CacheForScript} "set(CMAKE_RUNTIME_OUTPUT_DIRECTORY${OUTPUTCONFIG_UPPER} ${OUTPUT_PREFIX}/${OUTPUTCONFIG}/bin)\n")
  endforeach()
endfunction()

function(create_stage)
  add_custom_target(gather_runtime_dependencies
    ${CMAKE_COMMAND} 
    -DCMAKE_INSTALL_CONFIG_NAME=$<CONFIG> -P "${CMAKE_SOURCE_DIR}/cmake/cmake-common_stage.cmake"
    )
  set_target_properties(gather_runtime_dependencies
      PROPERTIES
      FOLDER ""
      PROJECT_LABEL "gather_runtime_dependencies"
  )
endfunction() 
########################################################################################################
# 
# Git Version Macro
# 
# List Tags in the order they appear assumes the version of the project is the latest version split by '.'
# Creates the following variabels
#
# ${ROOT_PROJECT_NAME}_VERSION_MAJOR  #First group of characters in the split
# ${ROOT_PROJECT_NAME}_VERSION_MINOR  #Second group of characters in the split
# ${ROOT_PROJECT_NAME}_VERSION_PATCH  #Third set of characters in the split
# ${ROOT_PROJECT_NAME}_VERSION_TWEAK  #Forth set of characters in the split
# ${ROOT_PROJECT_NAME}_VERSION_TAG    #A string tag based on how dirty the tag is usually -dirty 
# ${ROOT_PROJECT_NAME}_VERSION_HASH   #Abriviated Git Hash for the specific commit
# ${ROOT_PROJECT_NAME}_LIB_VERSION    #MAJOR.MINOR.PATCH - This really only works if your tags use this format
# ${ROOT_PROJECT_NAME}_CLEAN_BUILD    #True if the number of commits since the last tag is greater then 0
# ${ROOT_PROJECT_NAME}_COMMIT_DATE    #Date of the latest commit in the repo git  log -1 --format=%ai 
#
########################################################################################################

function(configure_version_information _SUCESS_CHECK)
  cmake_parse_arguments( ""  "" "MAJOR;MINOR;PATCH;TWEAK"
                         "" ${ARGN} )

  if(NOT _MAJOR AND NOT _MAJOR EQUAL 0) 
    set(_MAJOR -1)
  endif()
  if(NOT _MINOR AND NOT _MINOR EQUAL 0) 
    set(_MINOR -1)
  endif()
  if(NOT _PATCH AND NOT _PATCH EQUAL 0) 
    set(_PATCH -1)
  endif()
  if(NOT _TWEAK AND NOT _TWEAK EQUAL 0) 
    set(_TWEAK  "source" )
  endif()

  #Set values based on passed inputs as defaults incase GIT fails
  set(${ROOT_PROJECT_NAME}_VERSION_MAJOR ${_MAJOR} PARENT_SCOPE)
  set(${ROOT_PROJECT_NAME}_VERSION_MINOR ${_MINOR} PARENT_SCOPE)
  set(${ROOT_PROJECT_NAME}_VERSION_PATCH ${_PATCH} PARENT_SCOPE)
  set(${ROOT_PROJECT_NAME}_VERSION_TWEAK ${_TWEAK} PARENT_SCOPE)
  set(${ROOT_PROJECT_NAME}_LIB_VERSION "${_MAJOR}.${_MINOR}.${_PATCH}" PARENT_SCOPE)
  set(${ROOT_PROJECT_NAME}_VERSION_TAG "Custom" PARENT_SCOPE)
  set(${ROOT_PROJECT_NAME}_CLEAN_BUILD false PARENT_SCOPE)
  set(${ROOT_PROJECT_NAME}_VERSION_HASH "Unknown" PARENT_SCOPE)
  set(${ROOT_PROJECT_NAME}_COMMIT_DATE "Unknown" PARENT_SCOPE)
 
  if(GIT_FOUND) 
    #Pull the latest GIT TAG
    execute_process(COMMAND ${GIT_EXECUTABLE}  describe --tags
                    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
                    OUTPUT_VARIABLE _GIT_REV
                    RESULT_VARIABLE  _RESULT_VARIABLE
                    OUTPUT_STRIP_TRAILING_WHITESPACE
                    ERROR_QUIET)
    
    if(_RESULT_VARIABLE EQUAL 0)
      message(STATUS "GIT_REV=${_GIT_REV}")
      string(REPLACE "-" ";"  _GIT_REV_LIST "${_GIT_REV}" )
      string(REPLACE "." ";"  _GIT_FULL_REV_LIST "${_GIT_REV_LIST}") 
      list(LENGTH _GIT_FULL_REV_LIST _len)

      list(GET _GIT_REV_LIST 0 _VERSION_TAG)
      if(_len GREATER 0)
        list(GET _GIT_FULL_REV_LIST 0 _VERSION_MAJOR)
      endif()
      if(_len GREATER 1)
        list(GET _GIT_FULL_REV_LIST 1 _VERSION_MINOR)
      endif()
      if(_len GREATER 2)
        list(GET _GIT_FULL_REV_LIST 2 _VERSION_PATCH)
      endif()
      if(_len GREATER 4)
        set(_CLEAN_BUILD false)
        list(GET _GIT_FULL_REV_LIST 3 _VERSION_TWEAK)
        math(EXPR _last "${_len} - 1")
        list(GET _GIT_FULL_REV_LIST ${_last}  _VERSION_HASH )
      else()
        set(_CLEAN_BUILD true)
        set(_VERSION_TWEAK 0)
        if(_len EQUAL 4)
          list(GET _GIT_FULL_REV_LIST 3 _VERSION_HASH )
        else()
           execute_process(COMMAND ${GIT_EXECUTABLE}  rev-parse --short HEAD
                    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
                    OUTPUT_VARIABLE _GIT_REV
                    RESULT_VARIABLE _RESULT_VARIABLE
                    OUTPUT_STRIP_TRAILING_WHITESPACE
                    ERROR_QUIET)
           set(_VERSION_HASH "g${_GIT_REV}")
        endif()
      endif()
      string(STRIP "${_VERSION_HASH}" _VERSION_HASH )
	  string(SUBSTRING "${_VERSION_HASH}" 1 -1 _VERSION_HASH)
      set( ${_SUCESS_CHECK} True PARENT_SCOPE)
    endif() 
    #Pull the commit date of the last GIT TAG 
    execute_process(COMMAND ${GIT_EXECUTABLE}  log -1 --format=%ai 
                    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
                    OUTPUT_VARIABLE _GIT_COMMIT_DATE
                    RESULT_VARIABLE _RESULT_VARIABLE
                                    ERROR_QUIET)
    if(_RESULT_VARIABLE EQUAL 0_)    
      string(STRIP "${_GIT_COMMIT_DATE}" _GIT_COMMIT_DATE)
      set (${ROOT_PROJECT_NAME}_COMMIT_DATE "${_GIT_COMMIT_DATE}" PARENT_SCOPE)
    endif()

    # 
    if( _VERSION_MAJOR MATCHES "[0-9]+")
      set( ${ROOT_PROJECT_NAME}_VERSION_MAJOR ${_VERSION_MAJOR} PARENT_SCOPE)
    endif()
    if( _VERSION_MINOR MATCHES "[0-9]+")
      set( ${ROOT_PROJECT_NAME}_VERSION_MINOR ${_VERSION_MINOR} PARENT_SCOPE)
    endif()
    if( _VERSION_PATCH MATCHES "[0-9]+")
      set( ${ROOT_PROJECT_NAME}_VERSION_PATCH ${_VERSION_PATCH} PARENT_SCOPE)
    endif()
    if( _VERSION_TWEAK MATCHES "[0-9]+")
      set( ${ROOT_PROJECT_NAME}_VERSION_TWEAK ${_VERSION_TWEAK} PARENT_SCOPE)
    endif()
    if( _VERSION_MAJOR AND _VERSION_MINOR )
      set( ${ROOT_PROJECT_NAME}_LIB_VERSION   "${_VERSION_MAJOR}.${_VERSION_MINOR}" PARENT_SCOPE)
    endif()
    if (_VERSION_TAG)
      set( ${ROOT_PROJECT_NAME}_VERSION_TAG   "${_VERSION_TAG}" PARENT_SCOPE)
    endif()
    if (_CLEAN_BUILD)
      set( ${ROOT_PROJECT_NAME}_CLEAN_BUILD   "${_CLEAN_BUILD}" PARENT_SCOPE)
    endif()
    if(_VERSION_HASH)
      set( ${ROOT_PROJECT_NAME}_VERSION_HASH  "${_VERSION_HASH}"  PARENT_SCOPE)
    endif()
 endif() 
  
endfunction(configure_version_information)

########################################################################################################
# 
# Appends a suffix to the project name to make it easier to tell worktree solutions apart
# 
# Will additionally append the MSVC version to the solution if it is of version 14-16
# Creates the following variabels
#
# ${CMAKE_PROJECT_NAME}_SUFFIX        #The value of this variable is appended to the solution name
#                                     #For Cmake Generators who use the project name in the output files
#                                     It is useful for determining which MSVC sln you are loading from 
#                                     the jump list
#
# __PROJECT_SUFFIX_SET                This control variable is set in PARENT_SCOPE to prevent calling 
#                                     of the function twice
#
########################################################################################################
function(generate_project_suffix)
if(MSVC AND NOT __PROJECT_SUFFIX_SET)
  set(__PROJECT_SUFFIX_SET ON PARENT_SCOPE)
  if(${CMAKE_PROJECT_NAME}_PROJECT_SUFFIX)
    project(${CMAKE_PROJECT_NAME}_${${CMAKE_PROJECT_NAME}_PROJECT_SUFFIX})
  else()
    get_filename_component(source_dirname "${PROJECT_SOURCE_DIR}" NAME)
    if ( NOT source_dirname STREQUAL "core")
      project(${CMAKE_PROJECT_NAME}_${source_dirname})
    endif()
  endif()
  if(MSVC_VERSION GREATER_EQUAL 1930)
    project(${CMAKE_PROJECT_NAME}_msvc17)
  elseif(MSVC_VERSION GREATER_EQUAL 1920)
    project(${CMAKE_PROJECT_NAME}_msvc16)
  elseif(MSVC_VERSION GREATER_EQUAL 1910)
    project(${CMAKE_PROJECT_NAME}_msvc15)
  elseif(MSVC_VERSION GREATER_EQUAL 1900)
    project(${CMAKE_PROJECT_NAME}_msvc14)
  endif()
endif()
endfunction()
#######################################################################################################
#
#
#  By default cmake setups up multi configuration directories as {lib,bin}/{debug,release}/<product>
#  This just sets it to {debug,release}/{lib,bin}/<product> which is more natural to me.
# 
#  Optional Cache Value OUTPUT_PREFIX allows you add an additional later to this layout
#######################################################################################################
function(setup_unified_output_directory )
  cmake_parse_arguments( ""  "UNIFIED" "PREFIX"
                         "" ${ARGN} )
if(NOT __UNIFIED_DIR) 
  set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/${_PREFIX}/$<CONFIG>/lib" PARENT_SCOPE)
  set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/${_PREFIX}/$<CONFIG>/$<IF:$<PLATFORM_ID:Windows>,bin,lib>" PARENT_SCOPE)
  set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/${_PREFIX}/$<CONFIG>/bin" PARENT_SCOPE)
  set(__UNIFIED_DIR ON PARENT_SCOPE)
endif()
endfunction()
