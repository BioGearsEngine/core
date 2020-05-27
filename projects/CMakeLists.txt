enable_language(CXX)

option(${ROOT_PROJECT_NAME}_BUILD_STATIC  "Build ${ROOT_PROJECT_NAME} as a static libraries" OFF)
option(${ROOT_PROJECT_NAME}_BUILD_DOCUMENTATION "Create and install the HTML based API documentation (requires Doxygen)" OFF)
option(${ROOT_PROJECT_NAME}_PREPARE_RUNTIME_DIR "Automatically move files from share/ toto runtime/ to create a staging ground for BioGears to run. Takes a resonable amount of disk space not needed for some CI task" ON)

find_path (
  ${ROOT_PROJECT_NAME}_Baseline_DIR
  NAME Scenarios
  PATHS ${PROJECT_SOURCE_DIR}/../verification/core  
        ${PROJECT_SOURCE_DIR}/../validation/core 
        ${PROJECT_SOURCE_DIR}/../baselines/core
        ${PROJECT_SOURCE_DIR}/../verification  
        ${PROJECT_SOURCE_DIR}/../validation 
        ${PROJECT_SOURCE_DIR}/../baselines
  DOC "Optional directory path to hiarchial filetree of baseline archives. This will be used by Python Plotters to find Baselines"
)
find_path (
  ${ROOT_PROJECT_NAME}_Reference_DIR
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
if(NOT ${ROOT_PROJECT_NAME}_RUNTIME_DIR)
  set(${ROOT_PROJECT_NAME}_RUNTIME_DIR ${PROJECT_BINARY_DIR}/runtime)
endif()

add_custom_command(
    OUTPUT  ${${ROOT_PROJECT_NAME}_RUNTIME_DIR} 
    COMMAND ${CMAKE_COMMAND} -E make_directory ${${ROOT_PROJECT_NAME}_RUNTIME_DIR}
)

add_custom_command(
  OUTPUT ${${ROOT_PROJECT_NAME}_RUNTIME_DIR}/UCEDefs.conf
  DEPENDS ${PROJECT_SOURCE_DIR}/share/etc/UCEDefs.conf
  COMMAND ${CMAKE_COMMAND} -E copy ${PROJECT_SOURCE_DIR}/share/etc/UCEDefs.conf  ${${ROOT_PROJECT_NAME}_RUNTIME_DIR}/
)

file(GLOB_RECURSE source_xsd_list LIST_DIRECTORIES false "${PROJECT_SOURCE_DIR}/share/xsd/*.xsd")
foreach(_file IN LISTS  source_xsd_list)
  string(REPLACE "${PROJECT_SOURCE_DIR}/share" "${${ROOT_PROJECT_NAME}_RUNTIME_DIR}" runtime_file "${_file}" )
  list(APPEND runtime_xsd_list ${runtime_file})
  add_custom_command(
    OUTPUT  ${runtime_file}
    DEPENDS ${_file}
                COMMAND ${CMAKE_COMMAND} -E copy ${_file} ${runtime_file}
  )
endforeach()

file(GLOB_RECURSE source_jar_list LIST_DIRECTORIES false "${PROJECT_SOURCE_DIR}/share/jar/*.jar")
foreach(_file IN LISTS  source_jar_list)
  string(REPLACE "${PROJECT_SOURCE_DIR}/share" "${${ROOT_PROJECT_NAME}_RUNTIME_DIR}" runtime_file "${_file}" )
  list(APPEND runtime_jar_list ${runtime_file})
  add_custom_command(
    OUTPUT  ${runtime_file}
    DEPENDS ${_file}
    COMMAND ${CMAKE_COMMAND} -E copy ${_file} ${runtime_file}
  )
endforeach()

file(GLOB_RECURSE source_data_list LIST_DIRECTORIES false "${PROJECT_SOURCE_DIR}/share/data/*")
foreach(_file IN LISTS  source_data_list)
  string(REPLACE "${PROJECT_SOURCE_DIR}/share/data" "${${ROOT_PROJECT_NAME}_RUNTIME_DIR}" runtime_file "${_file}" )
  list(APPEND runtime_data_list ${runtime_file})
  add_custom_command(
    OUTPUT  ${runtime_file}
    DEPENDS ${_file}
    COMMAND ${CMAKE_COMMAND} -E copy ${_file} ${runtime_file}
  )
endforeach()

file(GLOB_RECURSE source_doc_list LIST_DIRECTORIES false "${PROJECT_SOURCE_DIR}/share/doc/*")
foreach(_file IN LISTS  source_doc_list)
  string(REPLACE "${PROJECT_SOURCE_DIR}/share" "${${ROOT_PROJECT_NAME}_RUNTIME_DIR}" runtime_file "${_file}" )
  list(APPEND runtime_doc_list ${runtime_file})
  add_custom_command(
    OUTPUT  ${runtime_file}
    DEPENDS ${_file}
    COMMAND ${CMAKE_COMMAND} -E copy ${_file} ${runtime_file}
  )
endforeach()

file(GLOB_RECURSE source_scenario_list   LIST_DIRECTORIES false "${PROJECT_SOURCE_DIR}/share/Scenarios/*" )
foreach(_file IN LISTS  source_scenario_list)
  string(REPLACE "${PROJECT_SOURCE_DIR}/share" "${${ROOT_PROJECT_NAME}_RUNTIME_DIR}" runtime_file "${_file}" )
  list(APPEND runtime_scenario_list ${runtime_file})
  add_custom_command(
    OUTPUT  ${runtime_file}
    DEPENDS ${_file}
    COMMAND ${CMAKE_COMMAND} -E copy ${_file} ${runtime_file}
  )
endforeach()
########################################################################################################################################################
if(${ROOT_PROJECT_NAME}_BUILD_DOCUMENTATION)
  file(GLOB_RECURSE source_website_list   LIST_DIRECTORIES false "${PROJECT_SOURCE_DIR}/share/website/*" )
  foreach(_file IN LISTS  source_website_list)
    string(REPLACE "${PROJECT_SOURCE_DIR}/share/website" "${${ROOT_PROJECT_NAME}_RUNTIME_DIR}" runtime_file "${_file}" )
    list(APPEND runtime_scenario_list ${runtime_file})
    add_custom_command(
      OUTPUT  ${runtime_file}
      DEPENDS ${_file}
      COMMAND ${CMAKE_COMMAND} -E copy ${_file} ${runtime_file}
    )
  endforeach()
endif()
########################################################################################################################################################
list(APPEND runtime_glob_list   "${runtime_xsd_list}" "${runtime_jar_list}"   "${runtime_data_list}"  "${runtime_doc_list}"   "${runtime_scenario_list}")

add_custom_target( prepare_runtime_dir
  DEPENDS 
  ${${ROOT_PROJECT_NAME}_RUNTIME_DIR}
  ${${ROOT_PROJECT_NAME}_RUNTIME_DIR}/UCEDefs.conf
  ${runtime_glob_list}
  COMMENT "Scanning for need to stage runtime dir"
)
###############################################################################
# Step 2: Project Includes are generally order dependent.  So manually maintain
#         this list
###############################################################################

verify_package(Log4cpp REQUIRED)
add_subdirectory(biogears-common)
add_subdirectory(biogears)
add_subdirectory(test_driver)
add_subdirectory(howto)
add_subdirectory(unit)
add_subdirectory(circuit_profiler)
add_subdirectory(zip)
add_subdirectory(cli)
add_subdirectory(website)

##############################################################################
# Step 3: Global Doxygen configuration 
#         you might need to tweak this if you have multiple doxy files
##############################################################################
if(${ROOT_PROJECT_NAME}_BUILD_DOCUMENTATION)
    find_package(Doxygen REQUIRED)
    if(NOT DOXYGEN_FOUND)
        message(FATAL_ERROR "Doxygen is needed to build the documentation.")
    endif()

    set(doxyfile_in ${CMAKE_SOURCE_DIR}/share/doc/cmake-common.doxy)
    set(doxyfile ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)

    configure_file(${doxyfile_in} ${doxyfile} @ONLY)

    add_custom_target(doxygen ALL
        COMMAND ${DOXYGEN_EXECUTABLE} ${doxyfile}
        WORKING_DIRECTORY ${PROJECT_BINARY_DIR}/runtime/doc
        COMMENT "Generating API documentation with Doxygen"
        VERBATIM)

   set_target_properties(doxygen PROPERTIES
                          DEBUG_POSTFIX ${CMAKE_DEBUG_POSTFIX}
                          CXX_STANDARD 11
                          FOLDER "Other"
                          PROJECT_LABEL "Doxygen")
   install(DIRECTORY ${PROJECT_BINARY_DIR}/outputs/docs/html DESTINATION share/doc/biogears/)
endif()
