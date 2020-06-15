# Options
###############################################################################

###############################################################################
# Base Variables
###############################################################################
set(PROJECT_NAME Python_CommonDataModel)
set(PREFIX cdm_py)

set(${PREFIX}_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include" )
set(${PREFIX}_PRIVATE_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src" )
set(${PREFIX}_GENERATED_INCLUDE_DIR "${CMAKE_CURRENT_BINARY_DIR}" )
set(${PREFIX}_UNIT_DIR "${CMAKE_CURRENT_SOURCE_DIR}/unit" PARENT_SCOPE)

message(STATUS "Configuring ${PROJECT_NAME}")
###############################################################################
# Requirments
###############################################################################

###############################################################################
#Code Generation
###############################################################################

add_source_files(SRCS ${CMAKE_CURRENT_SOURCE_DIR}/src "*.py"   "Sources\\")
add_source_files(SRCS ${CMAKE_CURRENT_BINARY_DIR}/src "*.py"   "Sources\\")
set(${PREFIX}_HEADERS ${HDRS} ${PUBLIC_HDRS} ${GEN_HDRS})
set(${PREFIX}_SOURCES ${SRCS} ${GEN_SRCS})

string(REGEX REPLACE "Python_" "" SHORT_NAME ${PROJECT_NAME})

set(CDM2MD_SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/src/mil/tatrc/physiology/datamodel/doxygen/CDM2MD.py PARENT_SCOPE)
set(XSDToDOXYGEN_SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/src/mil/tatrc/physiology/datamodel/doxygen/XSDToDoxygen.py PARENT_SCOPE)

