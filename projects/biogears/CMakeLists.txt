###############################################################################
# Policy adjustments
###############################################################################

###############################################################################
# Options
###############################################################################

option (${ROOT_PROJECT_NAME}_BUILD_IO_LIBRARY "When enabled an extra IO Library will be built allowing BioGears to fall back to embeded versions of resource files " ON)

option (${ROOT_PROJECT_NAME}_THROW_READONLY_EXCEPTIONS "Toggle the throwing CommonDataModelExceptions when a ReadOnly SEScalar is Modified." ON)
option (${ROOT_PROJECT_NAME}_THROW_NAN_EXCEPTIONS "Toggle the throwing CommonDataModelExceptions when an  SEScalar is NaNExceptions." ON)
option (${ROOT_PROJECT_NAME}_USE_OVERRIDE_CONTROL       "When enabled Biogears will control the ranges of override"  OFF)
option (${ROOT_PROJECT_NAME}_BUILD_CIRCUIT_TESTS       "Toggle on the Circuit Test Library which is a dependeny of libbiogears_jni"  OFF)

option (${ROOT_PROJECT_NAME}_BUILD_SWIG_BINDINGS       "Toggle the Genearting of languge bindings using SWIG (Experimental)"  OFF)

###############################################################################
# Base Variables
###############################################################################
set(PROJECT_NAME biogears)
set(PREFIX bio)

set(${PREFIX}_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/libBiogears/include" )
set(${PREFIX}_PRIVATE_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/libBiogears/src" )
set(${PREFIX}_GENERATED_INCLUDE_DIR "${CMAKE_CURRENT_BINARY_DIR}/generated/" )
set(${PREFIX}_UNIT_DIR "${CMAKE_CURRENT_SOURCE_DIR}/unit" PARENT_SCOPE)

###############################################################################
# Requirments
###############################################################################
set(CMAKE_THREAD_PREFER_PTHREAD TRUE)
if(WIN32)
  set(Boost_USE_STATIC_LIBS ON) 
endif()
verify_package(Threads REQUIRED)
verify_package(Eigen3 REQUIRED)
verify_package(CodeSynthesis REQUIRED)
if(${ROOT_PROJECT_NAME}_BUILD_PYTHON_BINDINGS)
  verify_package(pybind11 REQUIRED)
endif()
###############################################################################
#Code Generation
###############################################################################
file(GLOB_RECURSE biogears_xsd_list "${PROJECT_SOURCE_DIR}/share/xsd/biogears/*.xsd")
file(GLOB_RECURSE cdm_xsd_list "${PROJECT_SOURCE_DIR}/share/xsd/cdm/*.xsd")


set (BIOGEARS_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set (BIOGEARS_BINARY_DIR ${CMAKE_CURRENT_BINARY_DIR})


REGISTER_XSD_DIR(  "${PROJECT_SOURCE_DIR}/share/xsd/"
                    CONFIG "${PROJECT_SOURCE_DIR}/share/xsd/BioGearsDataModel.cfg"
                    PROJECT "generated/biogears" COMPONENT "schema" RESOURCE_FOLDER "resources/schema"
                    NO_TARGETS SOURCES_VAR XSD_SRCS HEADERS_VAR XSD_HDRS VERBOSITY 1)

add_custom_target(xsd_bindings 
  COMMENT "Generates XML Bindings from XSD Files" 
  DEPENDS ${XSD_SRCS} ${XSD_HDRS}
)
set_target_properties(xsd_bindings PROPERTIES FOLDER "Code Generators")
configure_file( ${CMAKE_CURRENT_SOURCE_DIR}/libBiogears/src/version.cpp.in  ${CMAKE_CURRENT_BINARY_DIR}/libBiogears/src/version.cpp NEWLINE_STYLE LF)
###############################################################################
#Sorce and Header Defines
###############################################################################
message(STATUS "Configuring ${PROJECT_NAME}")

set(LIB${PREFIX}_PY_HEADERS ${PY_HDRS} ${PUBLIC_PY_HDRS} ${GEN_PY_HDRS})
set(LIB${PREFIX}_PY_SOURCES ${PY_SRCS} ${GEN_PY_SRCS})

###############################################################################
#Define Logic
###############################################################################
if(${PREFIX}_BUILD_STATIC)
  ###############################################################################
  # libBiogears_st
  ###############################################################################
  add_subdirectory(libBiogears)
else()
  ###############################################################################
  # libBiogears_io
  ###############################################################################
  if(${ROOT_PROJECT_NAME}_BUILD_IO_LIBRARY)
    add_subdirectory(libIO)
  endif()
  ###############################################################################
  # libBiogears_cdm
  ###############################################################################
  add_subdirectory(libCDM)

  ###############################################################################
  # libBiogears
  ###############################################################################
  add_subdirectory(libBiogears)
  
  ###############################################################################
  # libBiogears_circuit_test
  ###############################################################################
  if( ${ROOT_PROJECT_NAME}_BUILD_CIRCUIT_TESTS)
    add_subdirectory(libCircuitTest)
  endif()
  ###############################################################################
  # libBiogears_circuit_test
  ###############################################################################
  add_subdirectory(swig_bindings)
endif() #${PREFIX}_BUILD_STATIC
######################################################################################################
# INSTALL LOGIC
######################################################################################################
if(NOT  ${ROOT_PROJECT_NAME}_BUILD_IO_LIBRARY)
  install(DIRECTORY 
      ${PROJECT_SOURCE_DIR}/share/xsd 
      DESTINATION share/biogears/${${ROOT_PROJECT_NAME}_VERSION_TAG}
      FILE_PERMISSIONS OWNER_READ GROUP_READ WORLD_READ
  )
  
  install(DIRECTORY 
      ${PROJECT_SOURCE_DIR}/share/data
      DESTINATION share/biogears/${${ROOT_PROJECT_NAME}_VERSION_TAG}
      FILE_PERMISSIONS OWNER_READ GROUP_READ WORLD_READ
      FILES_MATCHING 
      PATTERN "*.xml"
      PATTERN "*.csv"
      PATTERN "*.xlsx"
  )
  
  install(DIRECTORY 
      ${PROJECT_SOURCE_DIR}/share/Scenarios
      DESTINATION share/biogears/${${ROOT_PROJECT_NAME}_VERSION_TAG}/data/
      FILE_PERMISSIONS OWNER_READ GROUP_READ WORLD_READ
      FILES_MATCHING 
      PATTERN "*.xml"
  )
endif()
message(STATUS "")
