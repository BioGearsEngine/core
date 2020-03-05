###############################################################################
# Policy adjustments
###############################################################################
cmake_minimum_required(VERSION 3.8.0)
cmake_policy(VERSION 3.8.0)
cmake_policy(SET CMP0087 NEW)
###############################################################################
# Options
###############################################################################
option(${ROOT_PROJECT_NAME}_BUILD_CMD_TOOLS "Toggle building of cmd_bio tools" ON)
if(${ROOT_PROJECT_NAME}_BUILD_CMD_TOOLS)
###############################################################################
# Base Variables
###############################################################################
set(PROJECT_NAME cmd_bio)
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
add_source_files(HDRS ${CMAKE_CURRENT_SOURCE_DIR}/src "*.h"   "Headers\\Private\\")
add_source_files(HDRS ${CMAKE_CURRENT_SOURCE_DIR}/src "*.hpp"   "Headers\\Private\\")
add_source_files(SRCS ${CMAKE_CURRENT_SOURCE_DIR}/src "*.cpp" "Sources\\")
add_source_files(SRCS ${CMAKE_CURRENT_SOURCE_DIR}/src "*.c" "Sources\\")
configure_file(src/utils/KnownPaths.h.in src/utils/KnownPaths.h )
set(${PREFIX}_HEADERS ${HDRS} ${PUBLIC_HDRS} ${GEN_HDRS}  ${CMAKE_CURRENT_BINARY_DIR}/src/utils/KnownPaths.h)
source_group(  "Headers\\Private\\utils"  FILES "${CMAKE_CURRENT_BINARY_DIR}/src/utils/KnownPaths.h" )

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
  DEBUG_POSTFIX ${CMAKE_DEBUG_POSTFIX}
  CXX_STANDARD 14
  VS_DEBUGGER_WORKING_DIRECTORY ${PROJECT_BINARY_DIR}/runtime
  )
  target_compile_definitions(${PROJECT_NAME} PRIVATE ${${PREFIX}_CPPFLAGS} )
  target_compile_options(${PROJECT_NAME} PRIVATE $<$<PLATFORM_ID:Windows>:/bigobj> PRIVATE $<$<PLATFORM_ID:Windows>:/MP${${ROOT_PROJECT_NAME}_BUILD_PARALLEL_LEVEL}>)
###############################################################################
# COMPILATION & LINKAGE MODIFICATIONS
###############################################################################
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_BINARY_DIR}/src/utils/ )
set(${PREFIX}_LIBS
      ${CMAKE_THREAD_LIBS_INIT}
      ${CMAKE_DL_LIBS}
      Biogears::libbiogears_cdm
      Biogears::libbiogears
	  minizip::lib
    )
if( ${ROOT_PROJECT}_BUILD_CIRCUIT_TESTS )
  list(APPEND ${PREFIX}_LIBS Biogears::libbiogears_test_circuit)
  target_compile_definitions(${PROJECT_NAME} PRIVATE CMD_BIO_SUPPORT_CIRCUIT_TEST} )
endif()
set(${PREFIX}_LIBS ${${PREFIX}_LIBS} PARENT_SCOPE)
target_link_libraries(${PROJECT_NAME}  ${${PREFIX}_LIBS} )
 
if(TARGET prepare_runtime_dir AND ${ROOT_PROJECT_NAME}_PREPARE_RUNTIME_DIR) 
add_dependencies(${PROJECT_NAME} prepare_runtime_dir)
endif()

if(CMAKE_COMPILER_IS_GNUCXX)
    set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS ${${PREFIX}_LDFLAGS})
endif()

 install(TARGETS ${PROJECT_NAME}
     RUNTIME DESTINATION bin/${${ROOT_PROJECT_NAME}_PLATFORM_PREFIX}$<$<BOOL:${${ROOT_PROJECT_NAME}_BREAKOUT_INSTALL}>:/$<CONFIG>>
     LIBRARY DESTINATION ${LIBRARY_INSTALL_DIR}/${${ROOT_PROJECT_NAME}_PLATFORM_PREFIX}$<$<BOOL:${${ROOT_PROJECT_NAME}_BREAKOUT_INSTALL}>:/$<CONFIG>>
     ARCHIVE DESTINATION lib/${${ROOT_PROJECT_NAME}_PLATFORM_PREFIX}$<$<BOOL:${${ROOT_PROJECT_NAME}_BREAKOUT_INSTALL}>:/$<CONFIG>>
  )
if(NOT CMAKE_CROSSCOMPILING AND NOT APPLE)
  install(CODE "
  include(BundleUtilities)

  function(EXELIST result curdir)
    file(GLOB children RELATIVE \${curdir} \${curdir}/*)
    set(dirlist \"\")
    foreach(child \${children})
      if(NOT IS_DIRECTORY \${curdir}/\${child} AND \${child} MATCHES \".*${CMAKE_EXECUTABLE_SUFFIX}\" )
        list(APPEND dirlist \${curdir}/\${child})
	    set(\${result} \${dirlist} PARENT_SCOPE)
	    return()
      endif()
    endforeach()
  endfunction()

  set(CONFIG_SUFFIX ${CMAKE_${CONFIG}_POSTFIX})

  set(CMAKE_PREFIX_PATH  \"${CMAKE_PREFIX_PATH}\")

  foreach(_dir IN LISTS CMAKE_PREFIX_PATH)
    list(APPEND THIRD_PARTY  \${_dir})
    list(APPEND THIRD_PARTY_LIB \${_dir}/lib)
    list(APPEND THIRD_PARTY_BIN \${_dir}/bin)
  endforeach()


	FILE(TO_CMAKE_PATH   \"\${CMAKE_INSTALL_PREFIX}\"  INSTALL_PATH)

	set( Biogears_RUNTIME_DESTINATION \"\${INSTALL_PATH}/bin/${${ROOT_PROJECT_NAME}_PLATFORM_PREFIX}$<$<BOOL:${${ROOT_PROJECT_NAME}_BREAKOUT_INSTALL}>:/$<CONFIG>>\" )
    set( Biogears_LIBRARY_DESTINATION \"\${INSTALL_PATH}/${LIBRARY_INSTALL_DIR}/${${ROOT_PROJECT_NAME}_PLATFORM_PREFIX}$<$<BOOL:${${ROOT_PROJECT_NAME}_BREAKOUT_INSTALL}>:/$<CONFIG>>\" )
    set( Biogears_ARCHIVE_DESTINATION \"\${INSTALL_PATH}/lib/${${ROOT_PROJECT_NAME}_PLATFORM_PREFIX}$<$<BOOL:${${ROOT_PROJECT_NAME}_BREAKOUT_INSTALL}>:/$<CONFIG>>\" )

    EXELIST(TARGET \"\${INSTALL_PATH}/bin/${${ROOT_PROJECT_NAME}_PLATFORM_PREFIX}$<$<BOOL:${${ROOT_PROJECT_NAME}_BREAKOUT_INSTALL}>:/$<CONFIG>>/\" )

    fixup_bundle(\"\${TARGET}\"
        \"\"
        \"\${THIRD_PARTY_LIB};\${THIRD_PARTY_BIN}\;\${INSTALL_PATH}/bin;\${INSTALL_PATH}/lib;\${Biogears_RUNTIME_DESTINATION};\${Biogears_LIBRARY_DESTINATION};\${Biogears_ARCHIVE_DESTINATION}\"
   
    )
")
endif()
message(STATUS "")
endif()
