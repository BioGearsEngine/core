cmake_minimum_required(VERSION 3.8.0)

enable_language(CXX)

option(ARA_${ROOT_PROJECT_NAME}_BUILD_STATIC  "Build ${ROOT_PROJECT_NAME} as a static libraries" OFF)
option(ARA_${ROOT_PROJECT_NAME}_BUILD_DOCUMENTATION "Create and install the HTML based API documentation (requires Doxygen)" OFF)
find_path (
  ARA_${ROOT_PROJECT_NAME}_Verification_DIR
  NAME Scenarios
  PATHS ${PROJECT_SOURCE_DIR}/../verification
  DOC "Optional directory path to a hiarchial verification filetree of baseline archives. This will be used by Java Utilities to find Baselines"
)
###############################################################################
# Step 1: Some packages need to be configured globally to ensure they find the 
#         right library all others should be called in the respective projects
#         to promote some independence
###############################################################################
include(ProcessorCount)
ProcessorCount(N)
if(NOT N EQUAL 0)
  math(EXPR PARALLEL_JOBS ${N}/2)
  set(ARA_${ROOT_PROJECT_NAME}_PARALLEL_JOBS ${PARALLEL_JOBS} CACHE STRING "Number of Parallel Jobs for IDEs if supported")
endif()

if(WIN32)
  set(LIBRARY_INSTALL_DIR bin)
  add_compile_options("/MP${ARA_${ROOT_PROJECT_NAME}_PARALLEL_JOBS}")
else()
  set(LIBRARY_INSTALL_DIR lib)
endif()

set(CMAKE_STATIC_LIBRARY_PREFIX "lib")
set(CMAKE_STATIC_LIBRARY_SUFFIX "_st")
set(CMAKE_SHARED_LIBRARY_PREFIX "lib")

  
set(OUTPUT_PREFIX ${CMAKE_BINARY_DIR}/outputs)
if(UNIX)
    foreach(OUTPUTCONFIG Release Debug)
        string(TOUPPER ${OUTPUTCONFIG} OUTPUTCONFIG_UPPER)
        set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${OUTPUTCONFIG_UPPER} "${OUTPUT_PREFIX}/lib")
        set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_${OUTPUTCONFIG_UPPER} "${OUTPUT_PREFIX}/lib")
        set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_${OUTPUTCONFIG_UPPER} "${OUTPUT_PREFIX}/bin")
    endforeach(OUTPUTCONFIG)
else()
    foreach(OUTPUTCONFIG ${CMAKE_CONFIGURATION_TYPES})
        string(TOUPPER ${OUTPUTCONFIG} OUTPUTCONFIG_UPPER)
        set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${OUTPUTCONFIG_UPPER} "${OUTPUT_PREFIX}/${OUTPUTCONFIG}/lib")
        set(CMAKE_PDB_OUTPUT_DIRECTORY_${OUTPUTCONFIG_UPPER} "${OUTPUT_PREFIX}/${OUTPUTCONFIG}/lib")
        set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_${OUTPUTCONFIG_UPPER} "${OUTPUT_PREFIX}/${OUTPUTCONFIG}/bin")
        set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_${OUTPUTCONFIG_UPPER} "${OUTPUT_PREFIX}/${OUTPUTCONFIG}/bin")
    endforeach(OUTPUTCONFIG)
endif()
###############################################################################
# Step 2: Project Includes are generally order dependent.  So manually maintain
#         this list
###############################################################################

find_package(Java)
option(ARA_${ROOT_PROJECT_NAME}_BUILD_JAVATOOLS     "Build Java based development tools (requires Doxygen)" ${Java_FOUND})

add_subdirectory(biogears)
add_subdirectory(scenario_driver)
add_subdirectory(test_driver)
add_subdirectory(howto)
add_subdirectory(unit)


if(Java_FOUND AND ARA_${ROOT_PROJECT_NAME}_BUILD_JAVATOOLS)
  add_subdirectory(java)
endif()

###############################################################################
# Step 3: Global Doxygen configuration 
#         you might need to tweak this if you have multiple doxy files
###############################################################################
if(ARA_${ROOT_PROJECT_NAME}_BUILD_DOCUMENTATION)
    find_package(Doxygen REQUIRED)
    if(NOT DOXYGEN_FOUND)
        message(FATAL_ERROR "Doxygen is needed to build the documentation.")
    endif()

    set(doxyfile_in ${CMAKE_SOURCE_DIR}/share/doc/cmake-common.doxy)
    set(doxyfile ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)

    configure_file(${doxyfile_in} ${doxyfile} @ONLY)

    add_custom_target(doc
        COMMAND ${DOXYGEN_EXECUTABLE} ${doxyfile}
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/share/doc/
        COMMENT "Generating API documentation with Doxygen"
        VERBATIM)

   set_target_properties( doc PROPERTIES
                          DEBUG_POSTFIX ${CMAKE_DEBUG_POSTFIX}
                          CXX_STANDARD 11
                          FOLDER "Other"
                          PROJECT_LABEL "Doxygen")

install(DIRECTORY ${CMAKE_SOURCE_DIR}/share/doc/html DESTINATION share/doc/kaleidoscppe/)
endif()

add_custom_command(
    OUTPUT  ${PROJECT_BINARY_DIR}/runtime
    COMMAND ${CMAKE_COMMAND} -E make_directory ${PROJECT_BINARY_DIR}/runtime
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${PROJECT_SOURCE_DIR}/share/verification ${PROJECT_BINARY_DIR}/runtime/verification
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${PROJECT_SOURCE_DIR}/share/xsd  ${PROJECT_BINARY_DIR}/runtime/xsd
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${PROJECT_SOURCE_DIR}/share/jar  ${PROJECT_BINARY_DIR}/runtime/jar
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${PROJECT_SOURCE_DIR}/share/data ${PROJECT_BINARY_DIR}/runtime
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${PROJECT_SOURCE_DIR}/share/etc  ${PROJECT_BINARY_DIR}/runtime
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${PROJECT_SOURCE_DIR}/share/doc  ${PROJECT_BINARY_DIR}/runtime/doc
    COMMENT "Generating runtime folder"
)

add_custom_target( stage_runtime ALL
    DEPENDS "${PROJECT_BINARY_DIR}/runtime"
    COMMENT "Scanning for need to stage runtime dir"
)