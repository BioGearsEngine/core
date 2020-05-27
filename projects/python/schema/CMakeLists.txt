###############################################################################
# Policy adjustments
###############################################################################
cmake_minimum_required(VERSION 3.8.0)
cmake_policy(VERSION 3.8.0)
###############################################################################
# Options
###############################################################################

###############################################################################
# Base Variables
###############################################################################
set(PROJECT_NAME Python_DataModelBindings)

set(PREFIX schema_py)

set(${PREFIX}_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include" PARENT_SCOPE)
set(${PREFIX}_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include" )
set(${PREFIX}_PRIVATE_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src" PARENT_SCOPE)
set(${PREFIX}_PRIVATE_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src" )
set(${PREFIX}_GENERATED_INCLUDE_DIR "${CMAKE_CURRENT_BINARY_DIR}" PARENT_SCOPE)
set(${PREFIX}_GENERATED_INCLUDE_DIR "${CMAKE_CURRENT_BINARY_DIR}" )
set(${PREFIX}_UNIT_DIR "${CMAKE_CURRENT_SOURCE_DIR}/unit" PARENT_SCOPE)

message(STATUS "Configuring ${PROJECT_NAME}")
###############################################################################
# Requirments
###############################################################################

###############################################################################
#Code Generation
###############################################################################
file(GLOB_RECURSE xsd_list "${PROJECT_SOURCE_DIR}/share/xsd/*.xsd")

pyxb_generate_python(BioGearsDataModel SRCS
	    NAMESPACE mil.tatrc.physiology.datamodel.bind 
	    INPUT_PATH ${PROJECT_SOURCE_DIR}/share/xsd
    	    DEPENDS ${xsd_list}
    )

###############################################################################
#Sorce and Header Defines
###############################################################################


add_source_files(SRCS ${CMAKE_CURRENT_SOURCE_DIR}/src "*.py"   "Sources\\")

set(${PREFIX}_HEADERS ${HDRS} ${PUBLIC_HDRS} ${GEN_HDRS})
set(${PREFIX}_SOURCES ${SRCS} ${GEN_SRCS})

string(REGEX REPLACE "Python_" "" SHORT_NAME ${PROJECT_NAME})

#add_executable(${PROJECT_NAME}
#	${${PREFIX}_SOURCES}
#        OUTPUT_NAME ${SHORT_NAME}
#)

message(WARNING "Sources :${${PREFIX}_SOURCES} SRCS : ${SRCS}")
###############################################################################
#Define Logic
###############################################################################

###############################################################################
# COMPILATION & LINKAGE MODIFICATIONS
###############################################################################

add_custom_command(OUTPUT ${SHORT_NAME}
	COMMAND ${CMAKE_COMMAND} -E copy ${${PREFIX}_SOURCES} ${SHORT_NAME}
	DEPENDS ${${PREFIX}_SOURCES})

add_custom_target(${PROJECT_NAME} ALL
	DEPENDS ${SHORT_NAME}
	SOURCES ${${PREFIX}_SOURCES})

set_target_properties(${PROJECT_NAME}
    PROPERTIES
    FOLDER "Python/py"
    OUTPUT_NAME ${SHORT_NAME}
    PROJECT_LABEL "${PROJECT_NAME}"
    )
