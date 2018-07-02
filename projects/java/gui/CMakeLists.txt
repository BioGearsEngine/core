###############################################################################
# Policy adjustments
###############################################################################

###############################################################################
# Options
###############################################################################

###############################################################################
# Base Variables
###############################################################################
set(PROJECT_NAME Java_BioGearsGUI)
set(PREFIX gui_jar)

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

###############################################################################
#Sorce and Header Defines
###############################################################################

add_source_files(SRCS ${CMAKE_CURRENT_SOURCE_DIR}/src "*.java"   "Sources\\")

set(${PREFIX}_HEADERS ${HDRS} ${PUBLIC_HDRS} ${GEN_HDRS})
set(${PREFIX}_SOURCES ${SRCS} ${GEN_SRCS})

string(REGEX REPLACE "Java_" "" SHORT_NAME ${PROJECT_NAME})

add_jar(${PROJECT_NAME}
        ${${PREFIX}_SOURCES}
        OUTPUT_NAME ${SHORT_NAME}
        INCLUDE_JARS Java_DataModelBindings
        Java_CommonDataModel
        Java_BioGearsEngine
        ${PROJECT_SOURCE_DIR}/share/jar/jide-oss-2.9.7.jar
        ${PROJECT_SOURCE_DIR}/share/jar/jchart2d-3.2.2.jar
        ${PROJECT_SOURCE_DIR}/share/jar/xmlgraphics-commons-1.3.1.jar
        ENTRY_POINT mil/tatrc/physiology/biogears/gui/BioGearsGUI
        )
###############################################################################
#Define Logic
###############################################################################
set_target_properties(${PROJECT_NAME}
    PROPERTIES
    FOLDER "Java/jar"
    OUTPUT_NAME ${SHORT_NAME}
    PROJECT_LABEL "${PROJECT_NAME}"
    )
add_dependencies(${PROJECT_NAME} Java_DataModelBindings        Java_CommonDataModel        Java_BioGearsEngine)
###############################################################################
# COMPILATION & LINKAGE MODIFICATIONS
###############################################################################
 install_jar(${PROJECT_NAME}
     DESTINATION java
  )
