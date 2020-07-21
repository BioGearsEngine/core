###############################################################################
# Policy adjustments
###############################################################################
cmake_minimum_required(VERSION 3.8.0)
cmake_policy(VERSION 3.8.0)
cmake_policy(SET CMP0087 NEW)
###############################################################################
# Options
###############################################################################
option(${ROOT_PROJECT_NAME}_BUILD_PLAYSCHOOL "Toggle building of playschool tools" ON)
if(${ROOT_PROJECT_NAME}_BUILD_PLAYSCHOOL)
###############################################################################
# Base Variables
###############################################################################
set(PROJECT_NAME playschool)
set(PREFIX bio)

set(${PREFIX}_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include" PARENT_SCOPE)
set(${PREFIX}_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include" )
set(${PREFIX}_PRIVATE_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src" PARENT_SCOPE)
set(${PREFIX}_PRIVATE_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src" )



###############################################################################
# Requirments
###############################################################################
if (${PREFIX}_BUILD_STATIC)
  add_definitions("-D${PREFIX}_BUILT_AS_STATIC")
  add_definitions("-DARACORE_BUILT_AS_STATIC")
endif()
###############################################################################
#Code Generation
###############################################################################

###############################################################################
#Sorce and Header Defines
###############################################################################
message(STATUS "")
message(STATUS "Configuring ${PROJECT_NAME}")

#Scenario Driver
add_source_files(HDRS ${CMAKE_CURRENT_SOURCE_DIR}/include "*.h"   "Headers\\Public\\")
add_source_files(SRCS ${CMAKE_CURRENT_SOURCE_DIR}/src "*.cpp" "Sources\\")
add_source_files(SRCS ${CMAKE_CURRENT_SOURCE_DIR}/src "*.c" "Sources\\")

set(${PREFIX}_SOURCES ${HDRS} ${SRCS})
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
  DEBUG_POSTFIX ${CMAKE_DEBUG_POSTFIX}
  CXX_STANDARD 14
  VS_DEBUGGER_WORKING_DIRECTORY ${PROJECT_BINARY_DIR}/runtime
  )

  list(APPEND ${PREFIX}_CPPFLAGS "-DASIO_SEPARATE_COMPILATION" "-DASIO_HEADER_ONLY"  )
  target_compile_definitions(${PROJECT_NAME} PRIVATE ${${PREFIX}_CPPFLAGS} )
  target_compile_options(${PROJECT_NAME} PRIVATE $<$<PLATFORM_ID:Windows>:/bigobj> PRIVATE $<$<PLATFORM_ID:Windows>:/MP${${ROOT_PROJECT_NAME}_BUILD_PARALLEL_LEVEL}>)
###############################################################################
# COMPILATION & LINKAGE MODIFICATIONS
###############################################################################
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include PRIVATE {CMAKE_CURRENT_SOURCE_DIR}/src/asio-1.16.1/include)
set(${PREFIX}_LIBS
      ${CMAKE_THREAD_LIBS_INIT}
      ${CMAKE_DL_LIBS}
    )

set(${PREFIX}_LIBS ${${PREFIX}_LIBS} PARENT_SCOPE)
target_link_libraries(${PROJECT_NAME}  ${${PREFIX}_LIBS} )
 
message(STATUS "")
endif()
