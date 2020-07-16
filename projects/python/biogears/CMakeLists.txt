###############################################################################
# Policy adjustments
###############################################################################
cmake_minimum_required(VERSION 3.8.0)
cmake_policy(VERSION 3.8.0)
###############################################################################
# Options
###############################################################################
option(Biogears_Python_BUILD_WEBSITE "Use Python to build the website" OFF)


###############################################################################
# Base Variables
###############################################################################
set(PROJECT_NAME Python_BioGears)
set(PREFIX biogears_py)

set(${PREFIX}_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include" PARENT_SCOPE)
set(${PREFIX}_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include" )
set(${PREFIX}_PRIVATE_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src" PARENT_SCOPE)
set(${PREFIX}_PRIVATE_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src" )
set(${PREFIX}_GENERATED_INCLUDE_DIR "${CMAKE_CURRENT_BINARY_DIR}" PARENT_SCOPE)
set(${PREFIX}_GENERATED_INCLUDE_DIR "${CMAKE_CURRENT_BINARY_DIR}" )
set(${PREFIX}_UNIT_DIR "${CMAKE_CURRENT_SOURCE_DIR}/unit" PARENT_SCOPE)

set(biogears_reference_file "D:/biogears/references/BioGears.bib")
set(doxygen_dot_dir "C:/Program Files/graphviz-2.38/release/bin/")
###############################################################################
# Requirments
###############################################################################
if(Biogears_Python_BUILD_WEBSITE)
    find_package(Perl REQUIRED)
        find_package(Doxygen REQUIRED)
	find_package (Python3 COMPONENTS Interpreter Development REQUIRED )
endif()
###############################################################################
#Code Generation
###############################################################################

###############################################################################
#Sorce and Header Defines
###############################################################################
message(STATUS "Configuring ${PROJECT_NAME}")

SET(MD_MAIN_PAGE "MainPage.md")

configure_file(${PROJECT_SOURCE_DIR}/share/doc/doxygen/doxyfile.in ${PROJECT_BINARY_DIR}/runtime/doc/doxygen/Doxyfile @ONLY)

add_source_files(SRCS ${CMAKE_CURRENT_SOURCE_DIR}/src "*.py"   "Sources\\")

set(${PREFIX}_HEADERS ${HDRS} ${PUBLIC_HDRS} ${GEN_HDRS})
set(${PREFIX}_SOURCES ${SRCS} ${GEN_SRCS})

string(REGEX REPLACE "Python_" "" SHORT_NAME ${PROJECT_NAME})


add_custom_command( OUTPUT ${SHORT_NAME} 
	#	COMMAND ${CMAKE_COMMAND} -E copy ${${PREFIX}_SOURCES} ${SHORT_NAME}
	DEPENDS  Python_DataModelBindings ${${PREFIX}_SOURCES}
              )


add_custom_target(${PROJECT_NAME}
	      DEPENDS ${SHORT_NAME} ${${PREFIX}_SOURCES} Python_DataModelBindings
	      SOURCES ${${PREFIX}_SOURCES}
             )

set_source_files_properties( Python_DataModelBindings PROPERTIES GENERATED TRUE )

###############################################################################
#Define Logic
###############################################################################
set_target_properties(${PROJECT_NAME}
    PROPERTIES
    FOLDER "Python"
    OUTPUT_NAME ${SHORT_NAME}
    PROJECT_LABEL "${PROJECT_NAME}"
    )

add_dependencies(${PROJECT_NAME} 
	Python_DataModelBindings
)

###############################################################################
# COMPILATION & LINKAGE MODIFICATIONS
###############################################################################

function(genClasspath _var)
  if (CMAKE_HOST_WIN32 AND NOT CYGWIN AND CMAKE_HOST_SYSTEM_NAME MATCHES "Windows")
    set(CMAKE_Python_INCLUDE_FLAG_SEP ";")
  else ()
    message(STATUS "Using Colon Seperator")
    set(CMAKE_Python_INCLUDE_FLAG_SEP ":")
  endif()
  string(REPLACE ";" "${CMAKE_Python_INCLUDE_FLAG_SEP}" _result "${ARGN}")
  set (${_var} "${_result}" )
  set (${_var} "${_result}" PARENT_SCOPE)
endfunction()

if(Biogears_Python_BUILD_WEBSITE)

	message(WARNING "Python_ver: ${Python3_EXECUTABLE}")
        add_custom_target(docs-prepare-py
	COMMAND ${Python3_EXECUTABLE}  ${CDM2MD_SOURCE} -p xsd/BioGearsDataModel.xsd -f doc/doxygen/processed_md#cdm
	COMMAND ${Python3_EXECUTABLE}  ${DoxygenPreprocessor_SOURCE} -p doc/markdown -f doc/doxygen/processed_md #cdm
        COMMAND ${Python3_EXECUTABLE}  ${DoxygenPreprocessor_SOURCE}  -p doc/methodology -f doc/doxygen/processed_md #cdm
        COMMAND ${Python3_EXECUTABLE}  ${XSDToDOXYGEN_SOURCE} -p xsd/BioGearsDataModel.xsd -f doc/doxygen/processed_md #cdm 
        WORKING_DIRECTORY ${PROJECT_BINARY_DIR}/runtime
	COMMENT "Running docs-prepare"
	VERBATIM
        USES_TERMINAL
  )
  
  add_custom_target(docs-doxygen-py
        COMMAND ${DOXYGEN_EXECUTABLE} Doxyfile
	COMMENT "Running docs-doxygen"
	WORKING_DIRECTORY ${PROJECT_BINARY_DIR}/runtime/doc/doxygen
        VERBATIM
        USES_TERMINAL

  )

  add_custom_target(docs-stage-py
        COMMAND ${CMAKE_COMMAND} -E  make_directory output/documentation
        COMMAND ${CMAKE_COMMAND} -E copy_directory doxygen/html/ output/documentation
	COMMAND ${CMAKE_COMMAND} -E tar cfv output/documentation.zip  --format=zip  -- output/documentation
        WORKING_DIRECTORY ${PROJECT_BINARY_DIR}/runtime/doc/
        VERBATIM
	COMMENT "Running docs-stage"
        USES_TERMINAL
 )
  if(Biogears_BUILD_WEBSITE_DEBUG)
    add_custom_target(docs-prepare-cdm2md
        COMMAND ${Python3_EXECUTABLE}  ${CDM2MD_SOURCE} -p xsd/BioGearsDataModel.xsd -f doc/doxygen/processed_md #cdm
        WORKING_DIRECTORY ${PROJECT_BINARY_DIR}/runtime
        VERBATIM
        USES_TERMINAL
    )
    add_custom_target(docs-prepare-preprocess-markdown
        COMMAND ${Python3_EXECUTABLE}  ${DoxygenPreprocessor_SOURCE} -p doc/markdown -f doc/doxygen/processed_md
        WORKING_DIRECTORY ${PROJECT_BINARY_DIR}/runtime
        VERBATIM
        USES_TERMINAL
    )
    add_custom_target(docs-prepare-preprocess-methodolgy
        COMMAND ${Python3_EXECUTABLE}  ${DoxygenPreprocessor_SOURCE} -p doc/methodology -f doc/doxygen/processed_md
        WORKING_DIRECTORY ${PROJECT_BINARY_DIR}/runtime
        VERBATIM
        USES_TERMINAL
    )
    add_custom_target(docs-prepare-xsd2doxygen
        COMMAND ${Python3_EXECUTABLE}  ${XSDToDOXYGEN_SOURCE} -p xsd/BioGearsDataModel.xsd -f doc/doxygen/processed_md #cdm
        WORKING_DIRECTORY ${PROJECT_BINARY_DIR}/runtime
        VERBATIM
        USES_TERMINAL
    )
  endif()
endif()


if(Biogears_Python_BUILD_WEBSITE)
  if (NOT Biogears_BUILD_WEBSITE_DEBUG)
       add_dependencies(docs-prepare-py ${PROJECT_NAME})
       add_dependencies(docs-doxygen-py ${PROJECT_NAME} docs-prepare-py)
       add_dependencies(docs-stage-py   ${PROJECT_NAME} docs-doxygen-py)
  endif()
  set_target_properties(docs-prepare-py PROPERTIES FOLDER "Python/website" EXCLUDE_FROM_ALL ON)
  set_target_properties(docs-doxygen-py PROPERTIES FOLDER "Python/website" EXCLUDE_FROM_ALL ON)
  set_target_properties(docs-stage-py PROPERTIES FOLDER "Python/website"   EXCLUDE_FROM_ALL ON)
  if(TARGET stage_runtime AND ${ROOT_PROJECT_NAME}_STAGE_RUNTIME)
   add_dependencies(docs-prepare-py stage_runtime )
  endif()
endif()

 set_target_properties(Python_BioGears PROPERTIES FOLDER "Python/py")
 set_target_properties(Python_DataModelBindings PROPERTIES FOLDER "Python/py")
                                 
