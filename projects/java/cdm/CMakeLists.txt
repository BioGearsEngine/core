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
set(PROJECT_NAME Java_CommonDataModel)
set(PREFIX cdm_jar)

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

###############################################################################
#Sorce and Header Defines
###############################################################################

#We are going to try and figure out the name of the Windows DLL which for reasons unknown to me
#is not required to match the link lib. No right way to do this, so for now I'm assuming the DLL is always
#more specific then the lib (e.g Library-3.lib Library-3.2.dll)
if(Xerces-c_LIBRARY_RELEASE_STATIC)
   get_filename_component(Xerces-c_DLL_PATH ${Xerces-c_LIBRARY_RELEASE_STATIC} DIRECTORY )
   get_filename_component(Xerces-c_RELEASE_DLL ${Xerces-c_LIBRARY_RELEASE_STATIC} NAME_WE)
   get_filename_component(Xerces-c_DEBUG_DLL   ${Xerces-c_LIBRARY_DEBUG_STATIC} NAME_WE)

   set(_xerces_release_dll ${Xerces-c_LIBRARY_RELEASE_STATIC})
   set(_xerces_debug_dll ${Xerces-c_LIBRARY_DEBUG_STATIC})
else()
   get_filename_component(Xerces-c_DLL_PATH ${Xerces-c_LIBRARY_RELEASE_DYNAMIC} DIRECTORY)
   get_filename_component(Xerces-c_RELEASE_DLL ${Xerces-c_LIBRARY_RELEASE_DYNAMIC} NAME_WE)
   get_filename_component(Xerces-c_DEBUG_DLL   ${Xerces-c_LIBRARY_DEBUG_DYNAMIC} NAME_WE)

   set(_xerces_release_dll ${Xerces-c_LIBRARY_RELEASE_DYNAMIC})
   set(_xerces_debug_dll ${Xerces-c_LIBRARY_DEBUG_DYNAMIC})
endif()
   
if(WIN32)
   file(GLOB _release_canidates "${Xerces-c_DLL_PATH}/../bin/*${Xerces-c_RELEASE_DLL}*${CMAKE_SHARED_LIBRARY_SUFFIX}")
   file(GLOB _debug_canidates   "${Xerces-c_DLL_PATH}/../bin/*${Xerces-c_DEBUG_DLL}*${CMAKE_SHARED_LIBRARY_SUFFIX}")

   list(GET _release_canidates 0 _tmp_release)
   list(GET _debug_canidates   0 _tmp_debug)

   get_filename_component(_xerces_release_dll "${_tmp_release}" ABSOLUTE)
   get_filename_component(_xerces_debug_dll    "${_tmp_debug}"   ABSOLUTE)
endif()

if(WIN32)  
    configure_file(src/mil/tatrc/physiology/utilities/Resources.java.win.in src/mil/tatrc/physiology/utilities/Resources.java )
else()
    configure_file(src/mil/tatrc/physiology/utilities/Resources.java.nix.in src/mil/tatrc/physiology/utilities/Resources.java )
endif()
    configure_file(src/mil/tatrc/physiology/utilities/testing/TestDriver.java.in src/mil/tatrc/physiology/utilities/testing/TestDriver.java )
add_source_files(SRCS ${CMAKE_CURRENT_SOURCE_DIR}/src "*.java"   "Sources\\")
add_source_files(SRCS ${CMAKE_CURRENT_BINARY_DIR}/src "*.java"   "Sources\\")

set(${PREFIX}_HEADERS ${HDRS} ${PUBLIC_HDRS} ${GEN_HDRS})
set(${PREFIX}_SOURCES ${SRCS} ${GEN_SRCS})
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
        ${PROJECT_SOURCE_DIR}/share/jar/xmlbeans-2.6.0.jar
        ${PROJECT_SOURCE_DIR}/share/jar/jdom-2.0.2.jar
        ${PROJECT_SOURCE_DIR}/share/jar/reflections-0.9.9-RC1-uberjar.jar
        ${PROJECT_SOURCE_DIR}/share/jar/dom4j-1.6.1.jar
        ${PROJECT_SOURCE_DIR}/share/jar/gson-1.4.jar
        ${PROJECT_SOURCE_DIR}/share/jar/guava-11.0.2.jar
        ${PROJECT_SOURCE_DIR}/share/jar/javassist-3.16.1-GA.jar
        ${PROJECT_SOURCE_DIR}/share/jar/jsr305-1.3.9.jar
        ${PROJECT_SOURCE_DIR}/share/jar/slf4j-api-1.6.1.jar
        ${PROJECT_SOURCE_DIR}/share/jar/slf4j-simple-1.6.1.jar
        ${PROJECT_SOURCE_DIR}/share/jar/xml-apis-1.0.b2.jar
        ${PROJECT_SOURCE_DIR}/share/jar/poi-3.13-20150929.jar
        ${PROJECT_SOURCE_DIR}/share/jar/poi-ooxml-3.13-20150929.jar
        ${PROJECT_SOURCE_DIR}/share/jar/poi-ooxml-schemas-3.13-20150929.jar
        ${PROJECT_SOURCE_DIR}/share/jar/pdfbox-2.0.0-RC3.jar
        ${PROJECT_SOURCE_DIR}/share/jar/commons-logging-1.2.jar
        ${PROJECT_SOURCE_DIR}/share/jar/fontbox-2.0.0-RC3.jar
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
add_dependencies(${PROJECT_NAME} libbiogears_jni)
###############################################################################
# COMPILATION & LINKAGE MODIFICATIONS
###############################################################################
 install_jar(${PROJECT_NAME}
     DESTINATION java
  )
