###############################################################################
# Policy adjustments
###############################################################################

###############################################################################
# Options
###############################################################################

 option(${ROOT_PROJECT_NAME}_COMMON_BUILD_STATIC       "If enabled Biogears biogears_common will be built as a static library"  ON)
 
###############################################################################
# Base Variables
###############################################################################
set(PROJECT_NAME biogears_common)
set(PREFIX common)

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
set(CMAKE_THREAD_PREFER_PTHREAD TRUE)
find_package(Threads REQUIRED)
###############################################################################
#Code Generation
###############################################################################

###############################################################################
#Sorce and Header Defines
###############################################################################
message(STATUS "")
message(STATUS "Configuring ${PROJECT_NAME}")

#Scenario Driver

add_source_files(PUBLIC_HDRS ${CMAKE_CURRENT_SOURCE_DIR}/include "*.h"   "Headers\\Public\\")

add_source_files(SRCS ${CMAKE_CURRENT_SOURCE_DIR}/src "*.cpp" "Sources\\")
add_source_files(SRCS ${CMAKE_CURRENT_SOURCE_DIR}/src "*.c" "Sources\\")

set(${PREFIX}_HEADERS ${HDRS} ${PUBLIC_HDRS} ${GEN_HDRS})
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


if(${ROOT_PROJECT_NAME}_COMMON_BUILD_STATIC)
  add_library(lib${PROJECT_NAME} STATIC  ${${PREFIX}_SOURCES} ${${PREFIX}_HEADERS})
else()
  add_library(lib${PROJECT_NAME} SHARED  ${${PREFIX}_SOURCES} ${${PREFIX}_HEADERS})
endif()

add_library(Biogears::libbiogears_common ALIAS lib${PROJECT_NAME})
set_target_properties(lib${PROJECT_NAME}
  PROPERTIES
  DEFINE_SYMBOL ${PROJECT_NAME}_EXPORTS
  FOLDER "Libraries"
  COMPILE_PDB_NAME "${PROJECT_NAME}"
  OUTPUT_NAME "${PROJECT_NAME}"
  PROJECT_LABEL "lib${PROJECT_NAME}"
  DEBUG_POSTFIX ${CMAKE_DEBUG_POSTFIX}
  CXX_STANDARD 14
  VS_DEBUGGER_WORKING_DIRECTORY ${PROJECT_BINARY_DIR}/runtime
  )
  target_compile_definitions(lib${PROJECT_NAME} PUBLIC  ${${PREFIX}_INTERFACE_CPPFLAGS} PRIVATE ${${PREFIX}_CPPFLAGS} )
  if (${ROOT_PROJECT_NAME}_COMMON_BUILD_STATIC)
    target_compile_definitions(lib${PROJECT_NAME} PUBLIC  "BIOGEARS_COMMON_BUILD_STATIC")
    set(LIB_TYPE STATIC)
  endif()

  target_compile_options(lib${PROJECT_NAME} PRIVATE $<$<PLATFORM_ID:Windows>:/bigobj>    PRIVATE $<$<PLATFORM_ID:Windows>:/MP${${ROOT_PROJECT_NAME}_BUILD_PARALLEL_LEVEL}>)
target_include_directories(lib${PROJECT_NAME}
  PRIVATE  ${${PREFIX}_INCLUDE_DIR} ${${PREFIX}_PRIVATE_INCLUDE_DIR} ${${PREFIX}_GENERATED_INCLUDE_DIR}
  INTERFACE $<BUILD_INTERFACE:${${PREFIX}_GENERATED_INCLUDE_DIR}> 
      $<BUILD_INTERFACE:${${PREFIX}_INCLUDE_DIR}>
      $<INSTALL_INTERFACE:include>
)
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

foreach( ITEM IN LISTS ${PREFIX}_INCLUDES)
  message(STATUS "  ${${ITEM}}")
endforeach()
set(${PREFIX}_LIBS
      ${CMAKE_THREAD_LIBS_INIT}
      ${CMAKE_DL_LIBS}
)

set(${PREFIX}_LIBS ${${PREFIX}_LIBS} PARENT_SCOPE)
target_link_libraries(lib${PROJECT_NAME}  ${${PREFIX}_LIBS} )
 
if(TARGET prepare_runtime_dir AND ${ROOT_PROJECT_NAME}_PREPARE_RUNTIME_DIR) 
	add_dependencies(lib${PROJECT_NAME} prepare_runtime_dir)
endif()

if(CMAKE_COMPILER_IS_GNUCXX)
    set_target_properties(lib${PROJECT_NAME} PROPERTIES LINK_FLAGS ${${PREFIX}_LDFLAGS})
endif()

 install(TARGETS lib${PROJECT_NAME} EXPORT libBiogears
     RUNTIME DESTINATION bin/${${ROOT_PROJECT_NAME}_PLATFORM_PREFIX}$<$<BOOL:${${ROOT_PROJECT_NAME}_BREAKOUT_INSTALL}>:/$<CONFIG>>
     LIBRARY DESTINATION ${LIBRARY_INSTALL_DIR}/${${ROOT_PROJECT_NAME}_PLATFORM_PREFIX}$<$<BOOL:${${ROOT_PROJECT_NAME}_BREAKOUT_INSTALL}>:/$<CONFIG>>
     ARCHIVE DESTINATION lib/${${ROOT_PROJECT_NAME}_PLATFORM_PREFIX}$<$<BOOL:${${ROOT_PROJECT_NAME}_BREAKOUT_INSTALL}>:/$<CONFIG>>
  )
  
  install(DIRECTORY 
    include/biogears
    DESTINATION include/
    FILE_PERMISSIONS OWNER_READ GROUP_READ WORLD_READ
    )

	install(FILES 
    "include/units.h"
    DESTINATION include/
    )

message(STATUS "")
