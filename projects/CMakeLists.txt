enable_language(CXX)

option(Biogears_BUILD_STATIC  "Build Biogears as a static libraries" OFF)
option(Biogears_BUILD_DOCUMENTATION "Create and install the HTML based API documentation (requires Doxygen)" OFF)
option(Biogears_PREPARE_RUNTIME_DIR "Automatically move files from share/ toto runtime/ to create a staging ground for BioGears to run. Takes a resonable amount of disk space not needed for some CI task" ON)

find_path (
  Biogears_Baseline_DIR
  NAME Scenarios
  PATHS ${PROJECT_SOURCE_DIR}/../verification/core  
        ${PROJECT_SOURCE_DIR}/../validation/core 
        ${PROJECT_SOURCE_DIR}/../baselines/core
        ${PROJECT_SOURCE_DIR}/../verification  
        ${PROJECT_SOURCE_DIR}/../validation 
        ${PROJECT_SOURCE_DIR}/../baselines
        ${PROJECT_SOURCE_DIR}/../../../verification/core  
        ${PROJECT_SOURCE_DIR}/../../../validation/core 
        ${PROJECT_SOURCE_DIR}/../../../baselines/core
        ${PROJECT_SOURCE_DIR}/../../../verification  
        ${PROJECT_SOURCE_DIR}/../../../validation 
        ${PROJECT_SOURCE_DIR}/../../../baselines
  DOC "Optional directory path to hiarchial filetree of baseline archives. This will be used by Python Plotters to find Baselines"
)
find_path (
  Biogears_Reference_DIR
  NAME BioGears.bib
  PATHS ${PROJECT_SOURCE_DIR}/../references
  DOC "Optional directory path to a hiarchial references filetree of publication articles for documentation building"
)
###############################################################################
# Step 1: Some packages need to be configured globally to ensure they find the 
#         right library all others should be called in the respective projects
#         to promote some independence
###############################################################################

if(WIN32)
  set(LIBRARY_INSTALL_DIR bin)
else()
  set(LIBRARY_INSTALL_DIR lib)
endif()

set(CMAKE_STATIC_LIBRARY_PREFIX "lib")
set(CMAKE_STATIC_LIBRARY_SUFFIX "_st${CMAKE_STATIC_LIBRARY_SUFFIX}")
set(CMAKE_SHARED_LIBRARY_PREFIX "lib")

setup_unified_output_directory(PREFIX "outputs")
##################################################################################
#STAGE RUNTIME LOGIC
##################################################################################
if(NOT Biogears_RUNTIME_DIR)
  set(Biogears_RUNTIME_DIR ${PROJECT_BINARY_DIR}/runtime)
endif()

add_custom_command(
    OUTPUT  ${Biogears_RUNTIME_DIR} 
    COMMAND ${CMAKE_COMMAND} -E make_directory ${Biogears_RUNTIME_DIR}
)

add_custom_command(
  OUTPUT ${Biogears_RUNTIME_DIR}/UCEDefs.conf
  DEPENDS ${PROJECT_SOURCE_DIR}/share/etc/UCEDefs.conf
  COMMAND ${CMAKE_COMMAND} -E copy ${PROJECT_SOURCE_DIR}/share/etc/UCEDefs.conf  ${Biogears_RUNTIME_DIR}/
)

file(GLOB_RECURSE source_xsd_list LIST_DIRECTORIES false CONFIGURE_DEPENDS  "${PROJECT_SOURCE_DIR}/share/xsd/*.xsd")
foreach(_file IN LISTS  source_xsd_list)
  string(REPLACE "${PROJECT_SOURCE_DIR}/share" "${Biogears_RUNTIME_DIR}" runtime_file "${_file}" )
  list(APPEND runtime_xsd_list ${runtime_file})
  add_custom_command(
    OUTPUT  ${runtime_file}
    DEPENDS ${_file}
                COMMAND ${CMAKE_COMMAND} -E copy ${_file} ${runtime_file}
  )
endforeach()


file(GLOB_RECURSE source_data_list LIST_DIRECTORIES false CONFIGURE_DEPENDS  "${PROJECT_SOURCE_DIR}/share/data/*")
foreach(_file IN LISTS  source_data_list)
  string(REPLACE "${PROJECT_SOURCE_DIR}/share/data" "${Biogears_RUNTIME_DIR}" runtime_file "${_file}" )
  list(APPEND runtime_data_list ${runtime_file})
  add_custom_command(
    OUTPUT  ${runtime_file}
    DEPENDS ${_file}
    COMMAND ${CMAKE_COMMAND} -E copy ${_file} ${runtime_file}
  )
endforeach()

if(Biogears_BUILD_WEBSITE)
file(GLOB_RECURSE source_doc_list LIST_DIRECTORIES false CONFIGURE_DEPENDS  "${PROJECT_SOURCE_DIR}/share/doc/*")
foreach(_file IN LISTS  source_doc_list)
  string(REPLACE "${PROJECT_SOURCE_DIR}/share" "${Biogears_RUNTIME_DIR}" runtime_file "${_file}" )
  list(APPEND runtime_doc_list ${runtime_file})
  add_custom_command(
    OUTPUT  ${runtime_file}
    DEPENDS ${_file}
    COMMAND ${CMAKE_COMMAND} -E copy ${_file} ${runtime_file}
  )
endforeach()
endif()

file(GLOB_RECURSE source_scenario_list   LIST_DIRECTORIES false  CONFIGURE_DEPENDS "${PROJECT_SOURCE_DIR}/share/Scenarios/*" )
foreach(_file IN LISTS  source_scenario_list)
  string(REPLACE "${PROJECT_SOURCE_DIR}/share" "${Biogears_RUNTIME_DIR}" runtime_file "${_file}" )
  list(APPEND runtime_scenario_list ${runtime_file})
  add_custom_command(
    OUTPUT  ${runtime_file}
    DEPENDS ${_file}
    COMMAND ${CMAKE_COMMAND} -E copy ${_file} ${runtime_file}
  )
endforeach()
########################################################################################################################################################
if(Biogears_BUILD_DOCUMENTATION)
  file(GLOB_RECURSE source_website_list   LIST_DIRECTORIES false CONFIGURE_DEPENDS  "${PROJECT_SOURCE_DIR}/share/website/*" )
  foreach(_file IN LISTS  source_website_list)
    string(REPLACE "${PROJECT_SOURCE_DIR}/share/website" "${Biogears_RUNTIME_DIR}" runtime_file "${_file}" )
    list(APPEND runtime_scenario_list ${runtime_file})
    add_custom_command(
      OUTPUT  ${runtime_file}
      DEPENDS ${_file}
      COMMAND ${CMAKE_COMMAND} -E copy ${_file} ${runtime_file}
    )
  endforeach()
endif()
########################################################################################################################################################
list(APPEND runtime_glob_list   "${runtime_xsd_list}"  "${runtime_data_list}"  "${runtime_doc_list}"   "${runtime_scenario_list}")

add_custom_target( prepare_runtime_dir
  DEPENDS 
  ${Biogears_RUNTIME_DIR}/UCEDefs.conf
  ${runtime_glob_list}
  COMMENT "Scanning for need to stage runtime dir"
)
###############################################################################
# Step 2: Project Includes are generally order dependent.  So manually maintain
#         this list
###############################################################################

SET(VS_DEBUGGER_ENV_PATH "PATH=")
foreach ( dir IN LISTS CMAKE_PREFIX_PATH )
  STRING(APPEND VS_DEBUGGER_ENV_PATH "${dir}/bin;${dir}/lib;")
endforeach()
STRING(APPEND VS_DEBUGGER_ENV_PATH "%PATH%")

LIST(APPEND VS_DEBUGGER_ENVIRONMENT_LIST "${VS_DEBUGGER_ENV_PATH}")
message(STATUS "VS_DEBUGGER_ENVIRONMENT_LIST=${VS_DEBUGGER_ENVIRONMENT_LIST}")
add_subdirectory(unit)
add_subdirectory(biogears-common)
add_subdirectory(biogears)
add_subdirectory(test_driver)
add_subdirectory(howto)
add_subdirectory(circuit_profiler)
add_subdirectory(zip)
add_subdirectory(cli)
add_subdirectory(website)

