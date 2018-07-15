###############################################################################
# Policy adjustments
###############################################################################
cmake_policy(SET CMP0070 NEW)
###############################################################################
# Options
###############################################################################

###############################################################################
# Base Variables
###############################################################################
set(PROJECT_NAME Java_CommonDataModel)
set(PREFIX cdm_jar)

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
configure_file(src/mil/tatrc/physiology/utilities/Resources.java.in src/Resources.java.in )
file(GENERATE OUTPUT $<CONFIG>/src/mil/tatrc/physiology/utilities/Resources.java INPUT ${CMAKE_CURRENT_BINARY_DIR}/src/Resources.java.in)
file(GENERATE OUTPUT $<CONFIG>/src/mil/tatrc/physiology/utilities/testing/TestDriver.java INPUT src/mil/tatrc/physiology/utilities/testing/TestDriver.java.in )
add_source_files(SRCS ${CMAKE_CURRENT_SOURCE_DIR}/src "*.java"   "Sources\\")
add_source_files(SRCS ${CMAKE_CURRENT_BINARY_DIR}/src "*.java"   "Sources\\")

set(${PREFIX}_HEADERS ${HDRS} ${PUBLIC_HDRS} ${GEN_HDRS})
set(${PREFIX}_SOURCES ${SRCS} ${GEN_SRCS} ${CMAKE_CURRENT_BINARY_DIR}/$<CONFIG>/src/mil/tatrc/physiology/utilities/Resources.java $<CONFIG>/ ${CMAKE_CURRENT_BINARY_DIR}/$<CONFIG>/src/mil/tatrc/physiology/utilities/testing/TestDriver.java)
string(REGEX REPLACE "Java_" "" SHORT_NAME ${PROJECT_NAME})

add_jar(${PROJECT_NAME}
        ${${PREFIX}_SOURCES}
        OUTPUT_NAME ${SHORT_NAME}
        INCLUDE_JARS Java_DataModelBindings
        ${PROJECT_SOURCE_DIR}/share/jar/jcommon-1.0.16.jar
        ${PROJECT_SOURCE_DIR}/share/jar/jfreechart-1.0.13.jar
        ${PROJECT_SOURCE_DIR}/share/jar/swtgraphics2d.jar
        ${PROJECT_SOURCE_DIR}/share/jar/zip4j-1.3.1.jar
        ${PROJECT_SOURCE_DIR}/share/jar/javax.mail-1.6.1.jar
        ${PROJECT_SOURCE_DIR}/share/jar/log4j-1.2.17.jar
        ${PROJECT_SOURCE_DIR}/share/jar/jdom-2.0.2.jar
        ${PROJECT_SOURCE_DIR}/share/jar/reflections-0.9.9-RC1-uberjar.jar
        ${PROJECT_SOURCE_DIR}/share/jar/gson-1.4.jar
        ${PROJECT_SOURCE_DIR}/share/jar/guava-11.0.2.jar
        ${PROJECT_SOURCE_DIR}/share/jar/javassist-3.16.1-GA.jar
        ${PROJECT_SOURCE_DIR}/share/jar/jsr305-1.3.9.jar
        ${PROJECT_SOURCE_DIR}/share/jar/slf4j-api-1.6.1.jar
        ${PROJECT_SOURCE_DIR}/share/jar/slf4j-simple-1.6.1.jar
        ${PROJECT_SOURCE_DIR}/share/jar/xmlbeans-2.6.0.jar
        ${PROJECT_SOURCE_DIR}/share/jar/xml-apis-1.0.b2.jar
        ${PROJECT_SOURCE_DIR}/share/jar/poi-3.17.jar
        ${PROJECT_SOURCE_DIR}/share/jar/poi-ooxml-3.17.jar
        ${PROJECT_SOURCE_DIR}/share/jar/poi-ooxml-schemas-3.17.jar
        ${PROJECT_SOURCE_DIR}/share/jar/dom4j-1.6.1.jar
        ${PROJECT_SOURCE_DIR}/share/jar/commons-collections4-4.1.jar
        ${PROJECT_SOURCE_DIR}/share/jar/commons-logging-1.2.jar
        ${PROJECT_SOURCE_DIR}/share/jar/commons-math3-3.6.1.jar
        ${PROJECT_SOURCE_DIR}/share/jar/commons-codec-1.11.jar
        ${PROJECT_SOURCE_DIR}/share/jar/pdfbox-2.0.0-RC3.jar
        ${PROJECT_SOURCE_DIR}/share/jar/fontbox-2.0.0-RC3.jar
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
add_dependencies(${PROJECT_NAME} Java_DataModelBindings libbiogears_jni)
###############################################################################
# COMPILATION & LINKAGE MODIFICATIONS
###############################################################################
 install_jar(${PROJECT_NAME}
     DESTINATION java
  )
