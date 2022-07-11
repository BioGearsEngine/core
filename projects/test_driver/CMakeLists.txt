###############################################################################
# Policy adjustments
###############################################################################
cmake_minimum_required(VERSION 3.8.0)
cmake_policy(VERSION 3.8.0)
###############################################################################
# Options
###############################################################################
option(Biogears_BUILD_TEST_DRIVER "Toggle building of test driver tools" Biogears_BUILD_CIRCUIT_TESTS)
if(Biogears_BUILD_TEST_DRIVER)
if(NOT Biogears_BUILD_CIRCUIT_TESTS)
  message(FATAL_ERROR "Fatal Error: test_driver requires BUILD_CIRCUIT_TESTS enabled to continue")
endif()
###############################################################################
# Base Variables
###############################################################################
set(CTEST_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include" PARENT_SCOPE)
set(CTEST_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include" )
set(CTEST_PRIVATE_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src" PARENT_SCOPE)
set(CTEST_PRIVATE_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src" )
set(CTEST_GENERATED_INCLUDE_DIR "${CMAKE_CURRENT_BINARY_DIR}" PARENT_SCOPE)
set(CTEST_GENERATED_INCLUDE_DIR "${CMAKE_CURRENT_BINARY_DIR}" )
set(CTEST_UNIT_DIR "${CMAKE_CURRENT_SOURCE_DIR}/unit" PARENT_SCOPE)

###############################################################################
# Requirments
###############################################################################
if (CTEST_BUILD_STATIC)
  add_definitions("-DCTEST_BUILT_AS_STATIC")
  add_definitions("-DARACORE_BUILT_AS_STATIC")
endif()

###############################################################################
#Code Generation
###############################################################################

###############################################################################
#Sorce and Header Defines
###############################################################################
message(STATUS "Configuring test_driver")

#Scenario Driver
add_source_files(HDRS LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/src REGEX "*.h"   SOURCE_GROUP  "Headers\\Private\\")
add_source_files(HDRS LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/src REGEX "*.hpp" SOURCE_GROUP  "Headers\\Private\\")
add_source_files(SRCS LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/src REGEX "*.cpp" SOURCE_GROUP  "Sources\\")
add_source_files(SRCS LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/src REGEX "*.c"   SOURCE_GROUP  "Sources\\")

set(CTEST_HEADERS ${HDRS} ${PUBLIC_HDRS} ${GEN_HDRS})
set(CTEST_SOURCES ${SRCS} ${GEN_SRCS})
###############################################################################
#Define Logic
###############################################################################
if(WIN32)
  list(APPEND CTEST_CPPFLAGS_EXPORT )
  list(APPEND CTEST_CPPFLAGS "-D_SCL_SECURE_NO_WARNINGS" "-D_CRT_SECURE_NO_WARNINGS" $ENV{PARALLEL_COMPILE} )
elseif(CMAKE_COMPILER_IS_GNUCXX)
list(APPEND CTEST_CPPFLAGS_EXPORT )
  list(APPEND CTEST_CPPFLAGS  ${CodeSynthesis_CPPFLAGS})
  list(APPEND CTEST_LDFLAGS "-Wl,--no-as-needed" )
endif()

if (CTEST_BUILD_STATIC)
  add_definitions("-DCTEST_BUILT_AS_STATIC")
endif()

add_executable(test_driver ${CTEST_SOURCES} ${CTEST_HEADERS})
set_target_properties(test_driver
  PROPERTIES
  DEFINE_SYMBOL test_driver_EXPORTS
  FOLDER "Binaries"
  OUTPUT_NAME "test_driver"
  COMPILE_PDB_NAME "test_driver"
  PROJECT_LABEL "test_driver"
  DEBUG_POSTFIX "${CMAKE_DEBUG_POSTFIX}"
  CXX_STANDARD 14
  VS_DEBUGGER_WORKING_DIRECTORY ${PROJECT_BINARY_DIR}/runtime
  VS_DEBUGGER_ENVIRONMENT "${VS_DEBUGGER_ENVIRONMENT_LIST}"
  )
   
  target_compile_definitions(test_driver PRIVATE ${CTEST_CPPFLAGS} )
  target_compile_options(test_driver PRIVATE $<$<PLATFORM_ID:Windows>:/bigobj>    PRIVATE $<$<PLATFORM_ID:Windows>:/MP${Biogears_BUILD_PARALLEL_LEVEL}>)
###############################################################################
# COMPILATION & LINKAGE MODIFICATIONS
###############################################################################
list(APPEND CTEST_INCLUDES
      CTEST_INCLUDE_DIR
      CTEST_PRIVATE_INCLUDE_DIR
      CTEST_GENERATED_INCLUDE_DIR
      bio_INCLUDE_DIR
)
list(REMOVE_DUPLICATES CTEST_INCLUDES)

set(CTEST_LIBS
      ${CMAKE_THREAD_LIBS_INIT}
      ${CMAKE_DL_LIBS}
      Biogears::libbiogears_cdm
      Biogears::libbiogears
      Biogears::libbiogears_circuit_test
)
#if(WIN32)
#  list(APPEND CTEST_LIBS libbiogears_string)
#endif()
set(CTEST_LIBS ${CTEST_LIBS} PARENT_SCOPE)
target_link_libraries(test_driver  ${CTEST_LIBS} )
if(TARGET stage_runtime AND Biogears_STAGE_RUNTIME) 
add_dependencies(test_driver stage_runtime)
endif()



if(CMAKE_COMPILER_IS_GNUCXX)
    set_target_properties(test_driver PROPERTIES LINK_FLAGS ${CTEST_LDFLAGS})
endif()

 install(TARGETS test_driver
     RUNTIME DESTINATION bin
     LIBRARY DESTINATION ${LIBRARY_INSTALL_DIR}
     ARCHIVE DESTINATION lib
  )

endif()
