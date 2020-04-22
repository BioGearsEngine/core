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
set(PROJECT_NAME Java_BioGears)
set(PREFIX biogears_jar)

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
if(Biogears_BUILD_WEBSITE)
    find_package(Perl REQUIRED)
	find_package(Doxygen REQUIRED)
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

add_source_files(SRCS ${CMAKE_CURRENT_SOURCE_DIR}/src "*.java"   "Sources\\")

set(${PREFIX}_HEADERS ${HDRS} ${PUBLIC_HDRS} ${GEN_HDRS})
set(${PREFIX}_SOURCES ${SRCS} ${GEN_SRCS})

string(REGEX REPLACE "Java_" "" SHORT_NAME ${PROJECT_NAME})

add_jar(${PROJECT_NAME}
        JARS_AS_SOURCES
        SOURCES
        ${${PREFIX}_SOURCES}
        Java_DataModelBindings
        Java_CommonDataModel
        ${PROJECT_SOURCE_DIR}/share/jar/zip4j-1.3.1.jar
        ${PROJECT_SOURCE_DIR}/share/jar/log4j-1.2.17.jar
        ${PROJECT_SOURCE_DIR}/share/jar/jfreechart-1.0.13.jar
        ${PROJECT_SOURCE_DIR}/share/jar/jcommon-1.0.16.jar
        ${PROJECT_SOURCE_DIR}/share/jar/swtgraphics2d.jar
        ${PROJECT_SOURCE_DIR}/share/jar/jchart2d-3.2.2.jar
        ${PROJECT_SOURCE_DIR}/share/jar/dom4j-1.6.1.jar
        ${PROJECT_SOURCE_DIR}/share/jar/gson-1.4.jar
        ${PROJECT_SOURCE_DIR}/share/jar/guava-11.0.2.jar
        ${PROJECT_SOURCE_DIR}/share/jar/javassist-3.16.1-GA.jar
        ${PROJECT_SOURCE_DIR}/share/jar/jsr305-1.3.9.jar
        ${PROJECT_SOURCE_DIR}/share/jar/reflections-0.9.9-RC1-uberjar.jar
        ${PROJECT_SOURCE_DIR}/share/jar/slf4j-api-1.6.1.jar
        ${PROJECT_SOURCE_DIR}/share/jar/slf4j-simple-1.6.1.jar
        ${PROJECT_SOURCE_DIR}/share/jar/xml-apis-1.0.b2.jar
        ${PROJECT_SOURCE_DIR}/share/jar/xmlbeans-2.6.0.jar
        ${PROJECT_SOURCE_DIR}/share/jar/poi-3.17.jar
        ${PROJECT_SOURCE_DIR}/share/jar/poi-ooxml-3.17.jar
        ${PROJECT_SOURCE_DIR}/share/jar/poi-ooxml-schemas-3.17.jar
        ${PROJECT_SOURCE_DIR}/share/jar/commons-math3-3.6.1.jar
        ${PROJECT_SOURCE_DIR}/share/jar/commons-codec-1.11.jar
        ${PROJECT_SOURCE_DIR}/share/jar/commons-collections4-4.1.jar
        OUTPUT_NAME ${SHORT_NAME}
        MANIFEST ${CMAKE_CURRENT_SOURCE_DIR}/BioGears_manifest.txt
        INCLUDE_JARS 
        )
###############################################################################
#Define Logic
###############################################################################
set_target_properties(${PROJECT_NAME}
    PROPERTIES
    FOLDER "Java"
    OUTPUT_NAME ${SHORT_NAME}
    PROJECT_LABEL "${PROJECT_NAME}"
    )
add_dependencies(${PROJECT_NAME} Biogears::libbiogears_jni
        Java_DataModelBindings
        Java_CommonDataModel
)
###############################################################################
# COMPILATION & LINKAGE MODIFICATIONS
###############################################################################
 install_jar(${PROJECT_NAME}
     DESTINATION java
  )

function(genClasspath _var)
  if (CMAKE_HOST_WIN32 AND NOT CYGWIN AND CMAKE_HOST_SYSTEM_NAME MATCHES "Windows")
    set(CMAKE_JAVA_INCLUDE_FLAG_SEP ";")
  else ()
    message(STATUS "Using Colon Seperator")
    set(CMAKE_JAVA_INCLUDE_FLAG_SEP ":")
  endif()
  string(REPLACE ";" "${CMAKE_JAVA_INCLUDE_FLAG_SEP}" _result "${ARGN}")
  set (${_var} "${_result}" )
  set (${_var} "${_result}" PARENT_SCOPE)
endfunction()


genClasspath(java_classpath
        ${PROJECT_BINARY_DIR}/outputs/jar/BioGears.jar
        ${PROJECT_BINARY_DIR}/outputs/jar/CommonDataModel.jar 
        ${PROJECT_BINARY_DIR}/outputs/jar/DataModelBindings.jar
        ${PROJECT_BINARY_DIR}/runtime/jar/jdom-2.0.2.jar
        ${PROJECT_BINARY_DIR}/runtime/jar/javax.mail-1.6.1.jar
    )
set (allocation_args "-Xms64M" "-Xmx1G" "-Xmn64M" "-Xss4m")
if(Biogears_BUILD_WEBSITE)
  add_custom_target(docs-prepare
        COMMAND ${Java_JAVA_EXECUTABLE} ${allocation_args} -classpath "${java_classpath}"  mil.tatrc.physiology.datamodel.doxygen.CDM2MD #cdm
        COMMAND ${Java_JAVA_EXECUTABLE} ${allocation_args} -classpath "${java_classpath}"  mil.tatrc.physiology.datamodel.doxygen.DoxygenPreprocessor  doc/markdown doc/doxygen/processed_md #cdm
        COMMAND ${Java_JAVA_EXECUTABLE} ${allocation_args} -classpath "${java_classpath}"  mil.tatrc.physiology.datamodel.doxygen.DoxygenPreprocessor  doc/methodology doc/doxygen/processed_md #cdm
        COMMAND ${Java_JAVA_EXECUTABLE} ${allocation_args} -classpath "${java_classpath}"  mil.tatrc.physiology.datamodel.doxygen.XSDToDoxygen xsd/BioGearsDataModel.xsd doc/doxygen/processed_md/xsd.ixx #cdm
        #COMMAND ${Java_JAVA_EXECUTABLE} ${allocation_args} -classpath "${java_classpath}"  mil.tatrc.physiology.utilities.csv.plots.PlotDriver PlotRunJava-1_2.config
        #COMMAND ${Java_JAVA_EXECUTABLE} ${allocation_args} -classpath "${java_classpath}"  mil.tatrc.physiology.utilities.csv.plots.PlotDriver PlotRunJava-2_2.config 
        WORKING_DIRECTORY ${PROJECT_BINARY_DIR}/runtime
        VERBATIM
        USES_TERMINAL
  )
  add_custom_target(docs-doxygen
        COMMAND ${DOXYGEN_EXECUTABLE} Doxyfile
        WORKING_DIRECTORY ${PROJECT_BINARY_DIR}/runtime/doc/doxygen
        VERBATIM
        USES_TERMINAL
  )

  add_custom_target(docs-stage
        COMMAND ${CMAKE_COMMAND} -E  make_directory output/documentation
        COMMAND  ${CMAKE_COMMAND} -E copy_directory doxygen/html/ output/documentation
        COMMAND  ${CMAKE_COMMAND} -E tar cz output/documentation.zip     output/documentation
        WORKING_DIRECTORY ${PROJECT_BINARY_DIR}/runtime/doc/
        VERBATIM
        USES_TERMINAL
  )
  if(Biogears_BUILD_WEBSITE_DEBUG)
    add_custom_target(docs-prepare-cdm2md
        COMMAND ${Java_JAVA_EXECUTABLE} ${allocation_args} -classpath "${java_classpath}"  mil.tatrc.physiology.datamodel.doxygen.CDM2MD #cdm
        WORKING_DIRECTORY ${PROJECT_BINARY_DIR}/runtime
        VERBATIM
        USES_TERMINAL
    )
    add_custom_target(docs-prepare-preprocess-markdown
        COMMAND ${Java_JAVA_EXECUTABLE} ${allocation_args} -classpath "${java_classpath}"  mil.tatrc.physiology.datamodel.doxygen.DoxygenPreprocessor  doc/markdown doc/doxygen/processed_md
        WORKING_DIRECTORY ${PROJECT_BINARY_DIR}/runtime
        VERBATIM
        USES_TERMINAL
    )
    add_custom_target(docs-prepare=preprocess-methodolgy
        COMMAND ${Java_JAVA_EXECUTABLE} ${allocation_args} -classpath "${java_classpath}"  mil.tatrc.physiology.datamodel.doxygen.DoxygenPreprocessor  doc/methodology doc/doxygen/processed_md #cdm
        WORKING_DIRECTORY ${PROJECT_BINARY_DIR}/runtime
        VERBATIM
        USES_TERMINAL
    )
    add_custom_target(docs-prepare-xsd2doxygen
        COMMAND ${Java_JAVA_EXECUTABLE} ${allocation_args} -classpath "${java_classpath}"  mil.tatrc.physiology.datamodel.doxygen.XSDToDoxygen xsd/BioGearsDataModel.xsd doc/doxygen/processed_md/xsd.ixx #cdm

        WORKING_DIRECTORY ${PROJECT_BINARY_DIR}/runtime
        VERBATIM
        USES_TERMINAL
    )
  endif()
endif()



option(Biogears_BUILD_WEBSITE "Use Java to build the website" OFF)
if(Biogears_BUILD_WEBSITE)
  if (NOT Biogears_BUILD_WEBSITE_DEBUG)
       add_dependencies(docs-prepare ${PROJECT_NAME})
       add_dependencies(docs-doxygen ${PROJECT_NAME} docs-prepare)
       add_dependencies(docs-stage   ${PROJECT_NAME} docs-doxygen)
  endif()
  set_target_properties(docs-prepare PROPERTIES FOLDER "Java/website" EXCLUDE_FROM_ALL ON)
  set_target_properties(docs-doxygen PROPERTIES FOLDER "Java/website" EXCLUDE_FROM_ALL ON)
  set_target_properties(docs-stage PROPERTIES FOLDER "Java/website"   EXCLUDE_FROM_ALL ON)
  if(TARGET stage_runtime AND ${ROOT_PROJECT_NAME}_STAGE_RUNTIME)  
   add_dependencies(docs-prepare stage_runtime )
  endif()
endif()
 set_target_properties(Java_BioGears PROPERTIES FOLDER "Java/jar")
 set_target_properties(Java_CommonDataModel PROPERTIES FOLDER "Java/jar")
 set_target_properties(Java_DataModelBindings PROPERTIES FOLDER "Java/jar")
