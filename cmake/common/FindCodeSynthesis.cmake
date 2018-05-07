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

#Registers a single XSD File to you cmake PROJECT_LABEL
#
# This function assumes that in your project is laid out with a share/xsd folder in PROJECT_SOURCE_DIR
# schema : filename with out extension to your XSD file to be registered. 
#          the function expects three functions schema.xsd schema.cfg and schema.xml
# root_dir : install dir of your public include fodlers example: boost opencv2 
# component :path to the generated header with respect to root_dir. This will be the location of the generated file in your build_dir
# resource_path : place where the xml and xsd should be copied to for use in runtime.

function(REGISTER_XSD schema root_dir component resource_path)
  set(CodeSynthesis_FLAGS --output-dir ${CMAKE_CURRENT_BINARY_DIR}/${root_dir}/${component} --options-file ${PROJECT_SOURCE_DIR}/share/xsd/${schema}.cfg ${PROJECT_SOURCE_DIR}/share/xsd/${schema}.xsd)

  if( NOT EXISTS ${CMAKE_CURRENT_BINARY_DIR}/${root_dir}/${component}/${schema}.hxx OR NOT EXISTS ${CMAKE_CURRENT_BINARY_DIR}/${root_dir}/${component}/${schema}.cxx 
      OR NOT EXISTS ${CMAKE_BINARY_DIR}/${resource_path}/${schema}.template.xml      OR NOT EXISTS ${CMAKE_BINARY_DIR}/${resource_path}/${schema}.xsd 
    )
    message(STATUS "Generating ${schema}.hxx and ${schema}.cxx")

    execute_process( WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
                     COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_CURRENT_BINARY_DIR}/${root_dir}/${component} 
                     COMMAND ${CMAKE_COMMAND} -E env  LD_LIBRARY_PATH=${ARA_${ROOT_PROJECT_NAME}_EXTERNAL}/lib ${CodeSynthesis_EXECUTABLE} cxx-tree ${CodeSynthesis_FLAGS} 
                     COMMAND ${CMAKE_COMMAND} -E copy ${PROJECT_SOURCE_DIR}/share/xsd/${schema}.xml ${CMAKE_BINARY_DIR}/${resource_path}/${schema}.template.xml
                     COMMAND ${CMAKE_COMMAND} -E copy ${PROJECT_SOURCE_DIR}/share/xsd/${schema}.xsd ${CMAKE_BINARY_DIR}/${resource_path}/${schema}.xsd   
                     ERROR_VARIABLE XSD_ERROR
                     OUTPUT_QUIET
                    )
    if(XSD_ERROR)
      message(FATAL_ERROR "XSD Generation Failed? Check the value of CodeSynthesis_EXECUTABLE\n"
                          "Error Msg:\n"
                          "${XSD_ERROR}\n"
             )
    endif(XSD_ERROR)
  endif()

  add_custom_command( OUTPUT  ${CMAKE_CURRENT_BINARY_DIR}/${root_dir}/${component}/${schema}.hxx ${CMAKE_CURRENT_BINARY_DIR}/${root_dir}/${component}/${schema}.cxx
                      WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
                      COMMAND ${CMAKE_COMMAND} -E env LD_LIBRARY_PATH=${ARA_${ROOT_PROJECT_NAME}_EXTERNAL}/lib ${CodeSynthesis_EXECUTABLE} cxx-tree --show-sloc ${CodeSynthesis_FLAGS} 
                      DEPENDS ${CodeSynthesis_EXECUTABLE} ${PROJECT_SOURCE_DIR}/share/xsd/${schema}.xsd
                      DEPENDS ${CodeSynthesis_EXECUTABLE} ${PROJECT_SOURCE_DIR}/share/xsd/${schema}.cfg 
                      COMMENT "Generating source code from XML" )

  add_custom_command( OUTPUT  ${CMAKE_BINARY_DIR}/${CMAKE_BINARY_DIR}/${schema}.xsd ${CMAKE_BINARY_DIR}/${resource_path}/${schema}.template.xml     
                      WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
                      COMMAND ${CMAKE_COMMAND} -E copy ${PROJECT_SOURCE_DIR}/share/xsd/${schema}.xsd ${CMAKE_BINARY_DIR}/${resource_path}/   
                      COMMAND ${CMAKE_COMMAND} -E copy ${PROJECT_SOURCE_DIR}/share/xsd/${schema}.xml ${CMAKE_BINARY_DIR}/${resource_path}/${schema}.template.xml     
                      DEPENDS ${PROJECT_SOURCE_DIR}/share/xsd/${schema}.xml ${PROJECT_SOURCE_DIR}/share/xsd/${schema}.xsd
                      DEPENDS ${PROJECT_SOURCE_DIR}/share/xsd/${schema}.cfg
                      COMMENT "Copying XML Template" )

  add_custom_target( xsd_gen_${component}_${schema} DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/${root_dir}/${component}/${schema}.hxx ${CMAKE_CURRENT_BINARY_DIR}/${root_dir}/${component}/${schema}.cxx 
                     COMMENT "Checking if re-generation is required" )

  add_custom_target( xsd_stage_${component}_${schema} DEPENDS  ${CMAKE_BINARY_DIR}/${resource_path}/${schema}.xsd ${CMAKE_BINARY_DIR}/${resource_path}/${schema}.template.xml     
                     COMMENT "Checking if re-generation is required" )

  install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${root_dir}/${component}/${schema}.hxx
          DESTINATION ${CMAKE_INSTALL_PREFIX}/include/${root_dir}/${component}/
          PERMISSIONS  OWNER_READ GROUP_READ WORLD_READ
         )

  set_target_properties(xsd_gen_${component}_${schema}
      PROPERTIES
      FOLDER "Code Generators"
      PROJECT_LABEL "XSD_Gen_${component}_${schema}")

  set_target_properties(xsd_stage_${component}_${schema}
      PROPERTIES
      FOLDER "Code Generators"
      PROJECT_LABEL "XSD_Move_${component}_${schema}")
endfunction(REGISTER_XSD)

function(list_directory result curdir)
  file(GLOB children RELATIVE ${curdir} ${curdir}/*)
  set(list "")
  foreach(child ${children})
      list(APPEND list ${child})
  endforeach()
  set(${result} ${list} PARENT_SCOPE)
endfunction()


#Registers all xsd found in a given DIR recursivly
#
# This function assumes that in your project is laid out with a share/xsd folder in PROJECT_SOURCE_DIR
# DIR : The directory in share/xsd to be recursed.  A DIR.cfg must also exist
# 
# root_dir : install dir of your public include fodlers example: boost opencv2 
# component :path to the generated header with respect to root_dir. This will be the location of the generated file in your build_dir
# resource_path : place where the xml and xsd should be copied to for use in runtime.
function(REGISTER_XSD_DIR dir root_dir component resource_path)
  list_directory( files ${PROJECT_SOURCE_DIR}/share/xsd/${dir})
  foreach( item IN LISTS files)
    set (extra_macro_args ${ARGN})

    # Did we get any optional args?
    list(LENGTH extra_macro_args num_extra_args)
    if (${num_extra_args} GREATER 0)
        list(GET extra_macro_args 0 cfg_file)
    else()
      set(cfg_file ${dir})
    endif ()

    if ( IS_DIRECTORY ${PROJECT_SOURCE_DIR}/share/xsd/${dir}/${item} )
      # message(STATUS "REGISTER_XSD_DIR( ${dir}/${item} ${root_dir} ${component}/${item} ${resource_path}/${item} ${cfg_file}.xsd)")
      REGISTER_XSD_DIR(  ${dir}/${item} ${root_dir} ${component}/${item} ${resource_path}/${item} ${cfg_file} )
    else()
      # message(STATUS "REGISTER_XSD_FILE( ${dir}/${item} ${cfg_file} ${root_dir} ${component}/ ${resource_path}/ )")
      REGISTER_XSD_FILE(  ${dir}/${item} ${cfg_file}.cfg ${root_dir} ${component}/ ${resource_path}/ )
    endif()
  endforeach()
endfunction(REGISTER_XSD_DIR)

function(REGISTER_XSD_FILE file cfg_file root_dir component resource_path)
  
  set(CodeSynthesis_FLAGS --output-dir ${CMAKE_CURRENT_BINARY_DIR}/${root_dir}/${component} --options-file ${PROJECT_SOURCE_DIR}/share/xsd/${cfg_file} ${PROJECT_SOURCE_DIR}/share/xsd/${file})
  
  get_filename_component(schema ${file} NAME_WE)

  if( NOT EXISTS ${CMAKE_CURRENT_BINARY_DIR}/${root_dir}/${component}/${schema}.hxx OR NOT EXISTS ${CMAKE_CURRENT_BINARY_DIR}/${root_dir}/${component}/${schema}.cxx 
      OR NOT EXISTS ${CMAKE_BINARY_DIR}/${resource_path}/${schema}.xsd 
    )
    message(STATUS "Generating ${root_dir}/${component}${schema}{.hxx,.cxx}")
    execute_process( 
                     WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
                     COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_CURRENT_BINARY_DIR}/${root_dir}/${component} 
                     COMMAND ${CMAKE_COMMAND} -E env  LD_LIBRARY_PATH=${ARA_${ROOT_PROJECT_NAME}_EXTERNAL}/lib ${CodeSynthesis_EXECUTABLE} cxx-tree ${CodeSynthesis_FLAGS} 
                     COMMAND ${CMAKE_COMMAND} -E copy ${PROJECT_SOURCE_DIR}/share/xsd/${file} ${CMAKE_BINARY_DIR}/${resource_path}/${schema}.xsd   
                     ERROR_VARIABLE XSD_ERROR
                    )
    if(XSD_ERROR )
      if (XSD_ERROR MATCHES "[Ee][Rr][Rr][Oo][Rr]")
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

  add_custom_command( OUTPUT  ${CMAKE_CURRENT_BINARY_DIR}/${root_dir}/${component}/${schema}.hxx ${CMAKE_CURRENT_BINARY_DIR}/${root_dir}/${component}/${schema}.cxx
                      WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
                      COMMAND ${CMAKE_COMMAND} -E env LD_LIBRARY_PATH=${ARA_${ROOT_PROJECT_NAME}_EXTERNAL}/lib ${CodeSynthesis_EXECUTABLE} cxx-tree --show-sloc ${CodeSynthesis_FLAGS} 
                      DEPENDS ${CodeSynthesis_EXECUTABLE} ${PROJECT_SOURCE_DIR}/share/xsd/${file}
                      DEPENDS ${CodeSynthesis_EXECUTABLE} ${PROJECT_SOURCE_DIR}/share/xsd/${cfg_file}
                      COMMENT "Generating source code from XML" )

  add_custom_command( OUTPUT  ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CURRENT_BINARY_DIR}/${schema}.xsd    
                      WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
                      COMMAND ${CMAKE_COMMAND} -E copy ${PROJECT_SOURCE_DIR}/share/xsd/${file} ${CMAKE_BINARY_DIR}/${resource_path}/   
                      DEPENDS ${PROJECT_SOURCE_DIR}/share/xsd/${file}
                      DEPENDS ${PROJECT_SOURCE_DIR}/share/xsd/${cfg_file}
                      COMMENT "Copying XML Template" )

  string(REPLACE "/" "_" safe_unique_name ${component}/${schema})
  add_custom_target( xsd_gen_${safe_unique_name} DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/${root_dir}/${component}/${schema}.hxx ${CMAKE_CURRENT_BINARY_DIR}/${root_dir}/${component}/${schema}.cxx
                     COMMENT "Checking if re-generation is required" )

  add_custom_target( xsd_stage_${safe_unique_name} DEPENDS  ${CMAKE_BINARY_DIR}/${resource_path}/${schema}.xsd
                     COMMENT "Checking if re-generation is required" )

  install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${root_dir}/${component}/${schema}.hxx
          DESTINATION ${CMAKE_INSTALL_PREFIX}/include/${root_dir}/${component}/
          PERMISSIONS  OWNER_READ GROUP_READ WORLD_READ
         )

  set_target_properties(xsd_gen_${safe_unique_name}
      PROPERTIES
      FOLDER "Code Generators"
      PROJECT_LABEL "XSD_Gen_${safe_unique_name}")

  set_target_properties(xsd_stage_${safe_unique_name}
      PROPERTIES
      FOLDER "Code Generators"
      PROJECT_LABEL "XSD_Move_${safe_unique_name}")
endfunction(REGISTER_XSD_FILE)
function(GENERATE_XSD_SCHEMA file cfg_file root_dir component resource_path)
  
  set(CodeSynthesis_FLAGS --generate-xml-schema --output-dir ${CMAKE_CURRENT_BINARY_DIR}/${root_dir}/${component} --options-file ${PROJECT_SOURCE_DIR}/share/xsd/${cfg_file} ${PROJECT_SOURCE_DIR}/share/xsd/${file})
 
  
  get_filename_component(schema ${file} NAME_WE)
  if( NOT EXISTS ${CMAKE_CURRENT_BINARY_DIR}/${root_dir}/${component}/${schema}.hxx 
    )
    message(STATUS "Generating Schema ${file}" )

    execute_process( 
                     WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
                     COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_CURRENT_BINARY_DIR}/${root_dir}/${component} 
                     COMMAND ${CMAKE_COMMAND} -E env  LD_LIBRARY_PATH=${ARA_${ROOT_PROJECT_NAME}_EXTERNAL}/lib ${CodeSynthesis_EXECUTABLE} cxx-tree ${CodeSynthesis_FLAGS} 
                     ERROR_VARIABLE XSD_ERROR
                    )
    if(XSD_ERROR )
      if (XSD_ERROR MATCHES "[Ee][Rr][Rr][Oo][Rr]")
      message(FATAL_ERROR "XSD Schema Generation Failed? Check the value of CodeSynthesis_EXECUTABLE\n"
                          "Error Msg:\n"
                          "${XSD_ERROR}\n"
                          )
      else()
      message(STATUS "XSD Schema Generation Warrning? Check the value of CodeSynthesis_EXECUTABLE\n"
                          "Error Msg:\n"
                          "${XSD_ERROR}\n"
                          )
      endif()        
    endif(XSD_ERROR)
  endif()

  add_custom_command( OUTPUT  ${CMAKE_CURRENT_BINARY_DIR}/${root_dir}/${component}/${schema}.hxx
                      WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
                      COMMAND ${CMAKE_COMMAND} -E env LD_LIBRARY_PATH=${ARA_${ROOT_PROJECT_NAME}_EXTERNAL}/lib ${CodeSynthesis_EXECUTABLE} cxx-tree --show-sloc ${CodeSynthesis_FLAGS} 
                      DEPENDS ${CodeSynthesis_EXECUTABLE} ${PROJECT_SOURCE_DIR}/share/xsd/${cfg_file}
                      COMMENT "Generating source code from XML" )

  add_custom_command( OUTPUT  ${CMAKE_BINARY_DIR}/${CMAKE_BINARY_DIR}/${schema}.xsd    
                      WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
                      COMMAND ${CMAKE_COMMAND} -E copy ${PROJECT_SOURCE_DIR}/share/xsd/${file} ${CMAKE_BINARY_DIR}/${resource_path}/   
                      DEPENDS ${PROJECT_SOURCE_DIR}/share/xsd/${cfg_file}
                      COMMENT "Copying XML Template" )

  string(REPLACE "/" "_" safe_unique_name ${component}/${schema})
  add_custom_target( xsd_gen_${safe_unique_name} DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/${root_dir}/${component}/${schema}.hxx
                     COMMENT "Checking if re-generation is required" )

  add_custom_target( xsd_stage_${safe_unique_name} DEPENDS  ${CMAKE_BINARY_DIR}/${resource_path}/${schema}.xsd
                     COMMENT "Checking if re-generation is required" )

  install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${root_dir}/${component}/${schema}.hxx
          DESTINATION ${CMAKE_INSTALL_PREFIX}/include/${root_dir}/${component}/
          PERMISSIONS  OWNER_READ GROUP_READ WORLD_READ
         )

  set_target_properties(xsd_gen_${safe_unique_name}
      PROPERTIES
      FOLDER "Code Generators"
      PROJECT_LABEL "XSD_Gen_${safe_unique_name}")

  set_target_properties(xsd_stage_${safe_unique_name}
      PROPERTIES
      FOLDER "Code Generators"
      PROJECT_LABEL "XSD_Move_${safe_unique_name}")
endfunction(GENERATE_XSD_SCHEMA)
#################################################################################################################
#Begin Search of CodeSynthesis
#################################################################################################################

# Look for the header file.
find_path(  CodeSynthesis_INCLUDE_DIR 
      NAMES xsd/cxx/version.hxx
      ENV XSD_ROOTDIR 
      PATH_SUFFIXES 
            xsd
            include/xsd/
            include/
)

if( CMAKE_HOST_WIN32 )
  find_file(CodeSynthesis_EXECUTABLE
            NAMES "xsd.exe"
            ENV XSD_ROOTDIR 
            HINTS ${ARA_${ROOT_PROJECT_NAME}_EXTERNAL}
            PATH_SUFFIXES "bin" )
else()
  message(STATUS "Looking for bin/xsd in ${ARA_${ROOT_PROJECT_NAME}_EXTERNAL}")
  find_file(CodeSynthesis_EXECUTABLE
            NAMES "bin/xsd"
            HINTS "${ARA_${ROOT_PROJECT_NAME}_EXTERNAL}"
            NO_DEFAULT_PATH
            )
endif()


find_package(Xerces-c REQUIRED)

# handle the QUIETLY and REQUIRED arguments and set CodeSynthesis_FOUND to TRUE if
# all listed variables are TRUE
include(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(CodeSynthesis
                                  REQUIRED_VARS CodeSynthesis_INCLUDE_DIR 
                                                CodeSynthesis_EXECUTABLE
                                                Xerces-c_FOUND
                                  VERSION_VAR CodeSynthesis_VERSION_STRING)

if(CodeSynthesis_FOUND)
  set(CodeSynthesis_LIBRARIES ${Xerces-c_LIBRARIES})
  set(CodeSynthesis_INCLUDE_DIRS ${CodeSynthesis_INCLUDE_DIR} ${Xerces-c_INCLUDE_DIRS})
  set(CodeSynthesis_CPPFLAGS ${Xerces-c_CPPFLAGS})
  mark_as_advanced(CodeSynthesis_INCLUDE_DIR)
  mark_as_advanced(CodeSynthesis_EXECUTABLE)
endif()
