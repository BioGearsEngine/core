###############################################################################
# Policy adjustments
###############################################################################
cmake_minimum_required(VERSION 3.8.0)
cmake_policy(VERSION 3.8.0)
cmake_policy(SET CMP0087 NEW)
###############################################################################
# Options
###############################################################################
option(Biogears_BUILD_TOOLS "Toggle building of biogears command line tools" ON)
if(Biogears_BUILD_TOOLS)

###############################################################################
# Requirments
###############################################################################
if (${PREFIX}_BUILD_STATIC)
	add_definitions("-DBIOGEARS_BUILT_AS_STATIC")
  add_definitions("-DARACORE_BUILT_AS_STATIC")
endif()

###############################################################################
#Sorce and Header Defines
###############################################################################
message(STATUS "")
message(STATUS "Configuring bg-cli")

#Scenario Driver
add_source_files(HEADERS LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/include REGEX "*.h"  SOURCE_GROUP "Headers\\Public\\")
add_source_files(SOURCES LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/src/ REGEX "*.cpp" SOURCE_GROUP "Sources\\")
add_source_files(CONTROLLER_SOURCES LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/ REGEX "bg-cli-main.cpp" SOURCE_GROUP "Sources\\")
add_source_files(DRONE_SOURCES LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/ REGEX "bg-scenario-main.cpp" SOURCE_GROUP "Sources\\")
configure_file(src/utils/KnownPaths.h.in src/utils/KnownPaths.h )

##########################
#  Biogears Command Line #
##########################

add_executable(bg-cli ${HEADERS} ${SOURCES} ${CONTROLLER_SOURCES})
set_target_properties(bg-cli
  PROPERTIES
  FOLDER "Binaries"
  OUTPUT_NAME "bg-cli"
  COMPILE_PDB_NAME "bg-cli"
  PROJECT_LABEL "bg-cli"
  DEBUG_POSTFIX "${CMAKE_DEBUG_POSTFIX}"
  CXX_STANDARD 14
  VS_DEBUGGER_WORKING_DIRECTORY ${PROJECT_BINARY_DIR}/runtime
  VS_DEBUGGER_ENVIRONMENT "${VS_DEBUGGER_ENVIRONMENT_LIST}"
  )
if(WIN32)
  target_sources(bg-cli  PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/bg-cli.rc)
endif()
target_compile_definitions(bg-cli PRIVATE DEBUG_POSTFIX=$<$<CONFIG:Debug>:"${CMAKE_DEBUG_POSTFIX}">)
target_compile_options(bg-cli PRIVATE $<$<PLATFORM_ID:Windows>:/bigobj> PRIVATE $<$<PLATFORM_ID:Windows>:/MP${Biogears_BUILD_PARALLEL_LEVEL}>)
target_include_directories(bg-cli PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include PRIVATE ${CMAKE_CURRENT_BINARY_DIR}/src/utils/ PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src/)


target_link_libraries(bg-cli 
      Biogears::libbiogears
      minizip::lib
)

if(TARGET Biogears::libbiogears_io)
  target_link_libraries(bg-cli Biogears::libbiogears_io)
  target_compile_definitions(bg-cli PRIVATE "BIOGEARS_IO_PRESENT")
endif()

###################
#  SCENARIO DRONE #
###################

add_executable(bg-scenario ${HEADERS} ${SOURCES}  ${DRONE_SOURCES})
set_target_properties(bg-scenario
  PROPERTIES
  FOLDER "Binaries"
  OUTPUT_NAME "bg-scenario"
  COMPILE_PDB_NAME "bg-scenario"
  PROJECT_LABEL "bg-scenario"
  DEBUG_POSTFIX "${CMAKE_DEBUG_POSTFIX}"
  CXX_STANDARD 14
  VS_DEBUGGER_WORKING_DIRECTORY ${PROJECT_BINARY_DIR}/runtime
  VS_DEBUGGER_ENVIRONMENT "${VS_DEBUGGER_ENVIRONMENT_LIST}"
  )
  if(WIN32)
    target_sources(bg-scenario  PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/bg-scenario.rc)
  endif()
  set_property(INSTALL "$<TARGET_FILE_NAME:bg-scenario>"
  PROPERTY CPACK_START_MENU_SHORTCUTS "BioGears Scenario Driver"
)

target_compile_definitions(bg-scenario PRIVATE DEBUG_POSTFIX=$<$<CONFIG:Debug>:"${CMAKE_DEBUG_POSTFIX}">)
target_compile_options(bg-scenario PRIVATE $<$<PLATFORM_ID:Windows>:/bigobj> PRIVATE $<$<PLATFORM_ID:Windows>:/MP${Biogears_BUILD_PARALLEL_LEVEL}>)
target_include_directories(bg-scenario PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include PRIVATE ${CMAKE_CURRENT_BINARY_DIR}/src/utils/ PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src/)
target_link_libraries(bg-scenario 
      Biogears::libbiogears
      minizip::lib )

##########################
# Optional Boost Support #
##########################

find_package(Boost 1.73.0)
if(Boost_FOUND)
   set(Boost_USE_STATIC_LIBS FALSE)
   find_package(Boost 1.73.0 COMPONENTS system date_time filesystem program_options REQUIRED)

   target_compile_definitions(bg-cli PRIVATE BIOGEARS_SUBPROCESS_SUPPORT)
   target_link_libraries(bg-cli
      Boost::disable_autolinking
      Boost::dynamic_linking
      Boost::filesystem
      Boost::date_time
      Boost::program_options
   )
   add_dependencies(bg-cli bg-scenario )
   
   target_compile_definitions(bg-scenario PRIVATE BIOGEARS_SUBPROCESS_SUPPORT)
   target_link_libraries(bg-scenario 
      Boost::disable_autolinking
      Boost::dynamic_linking
      Boost::program_options
      Boost::filesystem
   )
endif()
####
if(NOT APPLE)
install(TARGETS bg-cli bg-scenario
     RUNTIME DESTINATION bin
     LIBRARY DESTINATION ${LIBRARY_INSTALL_DIR}
     ARCHIVE DESTINATION lib
)
else()
install(TARGETS bg-cli bg-scenario
     RUNTIME DESTINATION "./"
     LIBRARY DESTINATION ${LIBRARY_INSTALL_DIR}
     ARCHIVE DESTINATION lib
)
endif()
set_property(INSTALL "$<TARGET_FILE_NAME:bg-cli>"
  PROPERTY CPACK_START_MENU_SHORTCUTS "BioGears Commnad Line"
)

if(NOT (CMAKE_CROSSCOMPILING OR APPLE))
  install(CODE "
    include(BundleUtilities)

    function(EXELIST result curdir)
    file(REAL_PATH \${curdir} absolute_path BASE_DIRECTORY ${PROJECT_BINARY_DIR})
    file(GLOB children RELATIVE \${absolute_path} \${absolute_path}/*)
    set(dirlist \"\")
    foreach(child \${children})
      if(NOT IS_DIRECTORY \${absolute_path}/\${child} AND \${child} MATCHES \".*${CMAKE_EXECUTABLE_SUFFIX}\" )
      list(APPEND dirlist \${absolute_path}/\${child})
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

	  set( Biogears_RUNTIME_DESTINATION \"\${INSTALL_PATH}/bin/${Biogears_PLATFORM_PREFIX}$<$<BOOL:${Biogears_BREAKOUT_INSTALL}>:/$<CONFIG>>\" )
    set( Biogears_LIBRARY_DESTINATION \"\${INSTALL_PATH}/${LIBRARY_INSTALL_DIR}/${Biogears_PLATFORM_PREFIX}$<$<BOOL:${Biogears_BREAKOUT_INSTALL}>:/$<CONFIG>>\" )
    set( Biogears_ARCHIVE_DESTINATION \"\${INSTALL_PATH}/lib/${Biogears_PLATFORM_PREFIX}$<$<BOOL:${Biogears_BREAKOUT_INSTALL}>:/$<CONFIG>>\" )

    EXELIST(TARGET \"\${INSTALL_PATH}/bin/${Biogears_PLATFORM_PREFIX}$<$<BOOL:${Biogears_BREAKOUT_INSTALL}>:/$<CONFIG>>/\" )
    if(NOT APPLE)
      fixup_bundle(\"\${TARGET}\"
        \"\"
        \"\${THIRD_PARTY_LIB};\${THIRD_PARTY_BIN}\;\${INSTALL_PATH}/bin;\${INSTALL_PATH}/lib;\${Biogears_RUNTIME_DESTINATION};\${Biogears_LIBRARY_DESTINATION};\${Biogears_ARCHIVE_DESTINATION}\"
   
      )
    endif()
  ")
endif()
###
message(STATUS "")
endif()
