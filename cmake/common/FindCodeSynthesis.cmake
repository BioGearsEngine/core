#.rst:
# FindCodeSynthesis
# --------
#
# Find CodeSynthesis
#
# Find the native CodeSynthesis headers and libraries.
#
# ::
#
#   CodeSynthesis_INCLUDE_DIRS   - where to find CodeSynthesis/CodeSynthesis.h, etc.
#   CodeSynthesis_LIBRARIES      - List of libraries when using CodeSynthesis.
#   CodeSynthesis_CPP_FLAGS      - List of CPP Flags needed ot link against these libraries
#   CodeSynthesis_FOUND          - True if CodeSynthesis found.
#   CodeSynthesis_VERSION_STRING - the version of CodeSynthesis found (since CMake 2.8.8)

#=============================================================================
# Distributed under the OSI-approved BSD License (the "License");
# see accompanying file Copyright.txt for details.
#
# This software is distributed WITHOUT ANY WARRANTY; without even the
# implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
# See the License for more information.
#=============================================================================



if(NOT CodeSynthesis_FOUND)
#######################################################################################################################
# list_directory
#
# Utility function returns a list of all children in a directory for iteration
function(list_directory _result _curdir)
  cmake_parse_arguments( "" 
                          "DEBUG"
                          ""
                          ""
                          ${ARGN}
                          )
   if (_DEBUG)
       message ( "list_directory _result=${_result} _curdir=${_curdir} DEBUG=${_DEBUG}")
   endif()

  file(GLOB _children RELATIVE ${_curdir} ${_curdir}/*)
  set(_list "")
  foreach(_child IN LISTS _children)
     if (_DEBUG)
       message ( "Found ${_child}")
     endif()
     list(APPEND _list ${_child})
  endforeach()
  set(${_result} ${_list} PARENT_SCOPE)
endfunction()
#######################################################################################################################

# Register All files ending XSD in a directory and its subdirectories
#
# OPTIONS
#  -- STAGE  -- When TRUE STAGE will create targets that copy each XSD file to the RESOURCE_FOLDER
#  -- INSTALL -- When TRUE install logic will be built or installing each file in to INSTALL_PREFIX/share/PROJECT_NAME/RESOURCE_FOLDER
#
# ARGUMENTS
#
#  -- PROJECT  
#               A Path Relative to CMAKE_CURRENT_BINARY_DIR where the generated files will be started according to
#               ${CMAKE_CURRENT_BINARY_DIR}/${project}/${_component}/  This directory is intended to model your public include INTERFACE
#               ROOT = biogears COMPONENT=scehma example #include <biogears/_schema/result.hxx>
#               DEFAULT to ""   
#  -- COMPONENT  
#               Used when namming targets and in determing the location files will be placed. 
#               All targets are xsd_gen_${_component}/${_schema}) or xsd_stage_${_component}/${_schema}).
#               Files are placed in  ${CMAKE_CURRENT_BINARY_DIR}/${project}/${_component}/
#               DEfaults to ""
#  -- RESOURCE_FILE
#               Location where files will be placed if STAGE is true. Modifies instllation directory as well
#         
#  -- CONFIG
#               Name of CONFIG file needed to pass to xsd for generating files correctly. See 
#               https://www.codesynthesis.com/projects/xsd/documentation/xsd.xhtml
#  -- TARGETS
#                Variable where generated targets will be stored for used in setting target_dependecies
#                Defaults to _REGISTER_XSD_DIR_TARGETS
#  Multi arguments
#
#  -- DEPENDS
#               List of targets which the generated targets depend on.
function(REGISTER_XSD_DIR _directory)
  cmake_parse_arguments( "_l" 
                          "STAGE;INSTALL"
                          "WORKING_DIR;PROJECT;COMPONENT;RESOURCE_FOLDER;CONFIG;TARGETS"
                          "DEPENDS"
                          ${ARGN}
                          )

  if (NOT _directory)
    set (_directory "${PROJECT_SOURCE_DIR}/share/xsd")
  endif()
  if(_l_STAGE)
    list(APPEND _flags STAGE)
  endif()
  if(_l_INSTALL)
    list(APPEND _flags INSTALL) 
  endif() 
  if (NOT _l_TARGETS )
    set(_l_TARGETS  _REGISTER_XSD_DIR_TARGETS )
  endif()
  if( NOT _l_WORKING_DIR)
     set (_l_WORKING_DIR "${_directory}")
  endif()
  set(_config_file ${_l_CONFIG})
  set(_project ${_l_PROJECT} )
  set(_component ${_l_COMPONENT} )
  set(_resource_path ${_l_RESOURCE_FOLDER})
  set(_working_directory ${_l_WORKING_DIR})

  if ( NOT EXISTS "${_directory}" )
     message(FATAL_ERROR "REGISTER_XSD_DIR called on non existant directory ${_directory}")
  endif()
  list_directory( _files ${_directory} )
  foreach( _item IN LISTS _files)
    if ( IS_DIRECTORY ${_directory}/${_item} )  
        REGISTER_XSD_DIR(  ${_directory}/${_item}  CONFIG ${_config_file} 
              PROJECT ${_project} COMPONENT ${_component}/${_item} 
              RESOURCE_PATH ${_resource_path}/${_item} ${_flags} TARGETS ${_l_TARGETS} ${_verbose_Flag}
              WORKING_DIR ${_working_directory} DEPENDS ${_DEPENDS} )
    elseif( _item MATCHES ".*\\.xsd")
        REGISTER_XSD_FILE(  ${_directory}/${_item} CONFIG ${_config_file} 
          PROJECT ${_project} COMPONENT ${_component}/
          RESOURCE_PATH ${_resource_path}/ ${_flags} TARGETS ${_l_TARGETS}
          WORKING_DIR ${_working_directory} DEPENDS ${_DEPENDS} )
    endif()
  endforeach()
  set(${_l_TARGETS} ${${_l_TARGETS}} PARENT_SCOPE)
endfunction(REGISTER_XSD_DIR)
#######################################################################################################################
#REGISTER_XSD_FILE
#######################################################################################################################
function(REGISTER_XSD_FILE _filepath )
  cmake_parse_arguments( "_l" 
                          "STAGE;INSTALL"
                          "WORKING_DIR;PROJECT;COMPONENT;RESOURCE_FOLDER;CONFIG;TARGETS"
                          "DEPENDS"
                          ${ARGN}
                          )

  set(_config_file ${_l_CONFIG})
  set(_project ${_l_PROJECT} )
  set(_component ${_l_COMPONENT} )
  set(_resource_path ${_l_RESOURCE_FOLDER} )

  if (NOT _l_WORKING_DIR)
    set(_l_WORKING_DIR ${PROJECT_SOURCE_DIR}/share/xsd/)
  endif()
  if (NOT _l_TARGETS )
    set(_l_TARGETS  _REGISTER_XSD_DIR_TARGETS )
  endif()
	
  set(CodeSynthesis_FLAGS --output-dir ${CMAKE_CURRENT_BINARY_DIR}/${_project}/${_component} --options-file ${_config_file} ${_filepath})
  
  get_filename_component(_schema ${_filepath} NAME_WLE  )
  if( NOT EXISTS ${CMAKE_CURRENT_BINARY_DIR}/${_project}/${_component}/${_schema}.hxx OR NOT EXISTS ${CMAKE_CURRENT_BINARY_DIR}/${_project}/${_component}/${_schema}.cxx )
    message(STATUS "Generating ${_project}/${_component}${_schema}{.hxx,.cxx}")
    execute_process( 
                     WORKING_DIRECTORY ${_l_WORKING_DIR}
                     COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_CURRENT_BINARY_DIR}/${_project}/${_component} 
                     COMMAND ${CMAKE_COMMAND} -E env  LD_LIBRARY_PATH=${${ROOT_PROJECT_NAME}_EXTERNAL}/lib ${CodeSynthesis_EXECUTABLE} cxx-tree ${CodeSynthesis_FLAGS} 
                     ERROR_VARIABLE XSD_ERROR
                   )
    if(XSD_ERROR )
      if (XSD_ERROR MATCHES "[Ee][Rr][Rr][Oo][Rr]|failure")
      message(FATAL_ERROR "XSD Generation Failed? Check the value of CodeSynthesis_EXECUTABLE\n"
                          "Error Msg:\n"
                          "${XSD_ERROR}\n"
                          )
      else()
      message(STATUS "XSD Generation Warrning? Check the value of CodeSynthesis_EXECUTABLE\n"
                          "Error Msg:\n"
                          "${XSD_ERROR}\n"
                          )
      endif()        
    endif(XSD_ERROR)
  endif()

  string(REPLACE  "//" "/" _safe_unique_name ${_component}/${_schema}  )
  string(MAKE_C_IDENTIFIER   ${_component}/${_schema} _safe_unique_name)
  add_custom_command( OUTPUT  ${CMAKE_CURRENT_BINARY_DIR}/${_project}/${_component}/${_schema}.hxx ${CMAKE_CURRENT_BINARY_DIR}/${_project}/${_component}/${_schema}.cxx ${_l_OUTPUTS}
                      WORKING_DIRECTORY ${_l_WORKING_DIR}
                      COMMAND ${CMAKE_COMMAND} -E env LD_LIBRARY_PATH=${${ROOT_PROJECT_NAME}_EXTERNAL}/lib ${CodeSynthesis_EXECUTABLE} cxx-tree --show-sloc ${CodeSynthesis_FLAGS} 
                      DEPENDS ${_filepath}
                      DEPENDS ${CodeSynthesis_EXECUTABLE} ${_config_file}
                      DEPENDS ${_l_DEPENDS}
                      COMMENT "Generating source code from XML ${_filepath}" )

  add_custom_target( xsd_gen${_safe_unique_name} DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/${_project}/${_component}/${_schema}.hxx ${CMAKE_CURRENT_BINARY_DIR}/${_project}/${_component}/${_schema}.cxx
                     COMMENT "Checking if re-generation is required" )
  
  set_target_properties(xsd_gen${_safe_unique_name}
                        PROPERTIES
                        FOLDER "Code Generators"
                        PROJECT_LABEL "XSD_Gen_${_safe_unique_name}")

  list(APPEND ${_l_TARGETS} xsd_gen${_safe_unique_name})
  if(_l_STAGE)
    add_custom_target( xsd_stage${_safe_unique_name} DEPENDS  ${CMAKE_BINARY_DIR}/${_resource_path}/${_schema}.xsd
                       COMMENT "Checking if re-generation is required" )

    add_custom_command( OUTPUT  ${CMAKE_CURRENT_BINARY_DIR}/${_schema}.xsd    
                        WORKING_DIRECTORY ${_l_WORKING_DIR}
                        COMMAND ${CMAKE_COMMAND} -E copy ${_filepath} ${CMAKE_BINARY_DIR}/${_resource_path}/   
                        DEPENDS ${_filepath}
                        DEPENDS ${_config_file}
                        COMMENT "Staging XSD Template for runtime" )
    
    set_target_properties(xsd_stage${_safe_unique_name}
                          PROPERTIES
                          FOLDER "Code Generators"
                          PROJECT_LABEL "XSD_Move_${_safe_unique_name}")
	list(APPEND ${_l_TARGETS} xsd_gen${_safe_unique_name})
  endif()

  if(_l_INSTALL)
    install(FILES   ${CMAKE_CURRENT_BINARY_DIR}/${_project}/${_component}/${_schema}.hxx
        DESTINATION ${CMAKE_INSTALL_PREFIX}/include/${_project}/${_component}/
        PERMISSIONS  OWNER_READ GROUP_READ WORLD_READ
       )
  endif()
  set(${_l_TARGETS} ${${_l_TARGETS}} PARENT_SCOPE)
endfunction(REGISTER_XSD_FILE)

#################################################################################################################
#Begin Search of CodeSynthesis
#################################################################################################################

# Look for the header file.
find_path(CodeSynthesis_INCLUDE_DIR 
      NAMES xsd/cxx/version.hxx
      ENV XSD_ROOTDIR 
      PATH_SUFFIXES 
            libxsd/
            xsd/
            include/xsd/
            include/
)
  find_program(CodeSynthesis_EXECUTABLE
            NAMES "xsd"
            ENV XSD_ROOTDIR 
            HINTS "${CodeSynthesis_INCLUDE_DIR}/../bin"
            PATH_SUFFIXES "bin" )

include(CMakeFindDependencyMacro)
find_package(Xerces-c CONFIG QUIET)
find_dependency(Xerces-c )

# handle the QUIETLY and REQUIRED arguments and set CodeSynthesis_FOUND to TRUE if
# all listed variables are TRUE
include(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(CodeSynthesis
                                  REQUIRED_VARS CodeSynthesis_INCLUDE_DIR 
                                                CodeSynthesis_EXECUTABLE
                                                Xerces-c_FOUND
                                  VERSION_VAR CodeSynthesis_VERSION_STRING)

if(CodeSynthesis_FOUND)
  add_library(CodeSynthesis::XSD INTERFACE IMPORTED GLOBAL)
  set_target_properties(CodeSynthesis::XSD
    PROPERTIES
    INTERFACE_INCLUDE_DIRECTORIES
    ${CodeSynthesis_INCLUDE_DIR}
    )
  target_link_libraries(CodeSynthesis::XSD INTERFACE Xerces::xerces)

  mark_as_advanced(CodeSynthesis_INCLUDE_DIR)
  mark_as_advanced(CodeSynthesis_EXECUTABLE)
  mark_as_advanced(CodeSynthesis_DIR)
endif()
endif()
