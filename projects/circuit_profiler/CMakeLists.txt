###############################################################################
# Policy adjustments
###############################################################################
cmake_minimum_required(VERSION 3.8.0)
cmake_policy(VERSION 3.8.0)
###############################################################################
# Options
###############################################################################
option(${ROOT_PROJECT_NAME}_BUILD_CIRCUIT_PROFILER "Toggle building of circuit_profiler" ${ROOT_PROJECT_NAME}_BUILD_CIRCUIT_TESTS)
if(${ROOT_PROJECT_NAME}_BUILD_CIRCUIT_PROFILER)
if(NOT ${ROOT_PROJECT_NAME}_BUILD_CIRCUIT_TESTS)
  message(FATAL_ERROR "Fatal Error: circuit_profiler requires BUILD_CIRCUIT_TESTS enabled to continue")
endif()
###############################################################################
# Base Variables
###############################################################################
set(PROJECT_NAME circuit_profiler)
set(PREFIX bio)

set(${PREFIX}_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include" PARENT_SCOPE)
set(${PREFIX}_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include" )
set(${PREFIX}_PRIVATE_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src" PARENT_SCOPE)
set(${PREFIX}_PRIVATE_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src" )
set(${PREFIX}_GENERATED_INCLUDE_DIR "${CMAKE_CURRENT_BINARY_DIR}" PARENT_SCOPE)
set(${PREFIX}_GENERATED_INCLUDE_DIR "${CMAKE_CURRENT_BINARY_DIR}" )
set(${PREFIX}_UNIT_DIR "${CMAKE_CURRENT_SOURCE_DIR}/unit" PARENT_SCOPE)

###############################################################################
# Requirments
###############################################################################
if (${PREFIX}_BUILD_STATIC)
  add_definitions("-D${PREFIX}_BUILT_AS_STATIC") #${PREFIX} = bio
  add_definitions("-DARACORE_BUILT_AS_STATIC")
endif()
###############################################################################
#Code Generation
###############################################################################

###############################################################################
#Sorce and Header Defines
###############################################################################
message(STATUS "Configuring ${PROJECT_NAME}") #${PROJECT_NAME} = circuit_test

add_source_files(HDRS LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/src REGEX "*.h"   SOURCE_GROUP "Headers\\Private\\")
add_source_files(HDRS LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/src REGEX "*.hpp" SOURCE_GROUP "Headers\\Private\\")
add_source_files(SRCS LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/src REGEX "*.cpp" SOURCE_GROUP "Sources\\")
add_source_files(SRCS LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/src REGEX "*.c"   SOURCE_GROUP "Sources\\")

set(${PREFIX}_HEADERS ${HDRS} ${PUBLIC_HDRS} ${GEN_HDRS}) #${PREFIX} = bio, ${HDRS} defined above, ${PUBLIC_HDRS} & ${GEN_HDRS} are empty I think
set(${PREFIX}_SOURCES ${SRCS} ${GEN_SRCS})
###############################################################################
#Define Logic
###############################################################################
if(WIN32)
  list(APPEND ${PREFIX}_CPPFLAGS_EXPORT )
  list(APPEND ${PREFIX}_CPPFLAGS "-D_SCL_SECURE_NO_WARNINGS" "-D_CRT_SECURE_NO_WARNINGS"  $ENV{PARALLEL_COMPILE} )
elseif(CMAKE_COMPILER_IS_GNUCXX)
list(APPEND ${PREFIX}_CPPFLAGS_EXPORT )
  list(APPEND ${PREFIX}_CPPFLAGS  ${CodeSynthesis_CPPFLAGS})
  list(APPEND ${PREFIX}_LDFLAGS "-Wl,--no-as-needed" )
endif()

if (${PREFIX}_BUILD_STATIC)
  add_definitions("-D${PREFIX}_BUILT_AS_STATIC")
endif()

add_executable(${PROJECT_NAME} ${${PREFIX}_SOURCES} ${${PREFIX}_HEADERS})

set_target_properties(${PROJECT_NAME}
  PROPERTIES
  DEFINE_SYMBOL ${PROJECT_NAME}_EXPORTS
  FOLDER "Binaries"
  OUTPUT_NAME "${PROJECT_NAME}"
  COMPILE_PDB_NAME "${PROJECT_NAME}"
  PROJECT_LABEL "${PROJECT_NAME}"
  DEBUG_POSTFIX "${CMAKE_DEBUG_POSTFIX}"
  CXX_STANDARD 14
  VS_DEBUGGER_WORKING_DIRECTORY ${PROJECT_BINARY_DIR}/runtime
  VS_DEBUGGER_ENVIRONMENT "${VS_DEBUGGER_ENVIRONMENT_LIST}"

  )
  target_compile_definitions(${PROJECT_NAME} PRIVATE ${${PREFIX}_CPPFLAGS} )
  target_compile_options(${PROJECT_NAME} PRIVATE $<$<PLATFORM_ID:Windows>:/bigobj> PRIVATE $<$<PLATFORM_ID:Windows>:/MP${${ROOT_PROJECT_NAME}_BUILD_PARALLEL_LEVEL}>)
###############################################################################
# COMPILATION & LINKAGE MODIFICATIONS
###############################################################################

list(APPEND ${PREFIX}_INCLUDES
      ${PREFIX}_INCLUDE_DIR
      ${PREFIX}_PRIVATE_INCLUDE_DIR
      ${PREFIX}_GENERATED_INCLUDE_DIR
      bio_INCLUDE_DIR
)
list(REMOVE_DUPLICATES ${PREFIX}_INCLUDES)

set(${PREFIX}_LIBS
      ${CMAKE_THREAD_LIBS_INIT}
      ${CMAKE_DL_LIBS}
      Biogears::libbiogears_cdm
      Biogears::libbiogears
      Biogears::libbiogears_circuit_test
)
set(${PREFIX}_LIBS ${${PREFIX}_LIBS} PARENT_SCOPE)
target_link_libraries(${PROJECT_NAME}  ${${PREFIX}_LIBS} )
 
if(STAGE_RUNTIME) 
add_dependencies(${PROJECT_NAME} stage_runtime)
endif()

if(CMAKE_COMPILER_IS_GNUCXX)
    set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS ${${PREFIX}_LDFLAGS})
endif()

 install(TARGETS ${PROJECT_NAME}
     RUNTIME DESTINATION bin
     LIBRARY DESTINATION ${LIBRARY_INSTALL_DIR}
     ARCHIVE DESTINATION lib
  )

endif()
