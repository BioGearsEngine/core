###############################################################################
# Policy adjustments
###############################################################################
cmake_minimum_required(VERSION 3.8.0)
cmake_policy(VERSION 3.8.0)
cmake_policy(SET CMP0087 NEW)
###############################################################################
# Options
###############################################################################
option(${ROOT_PROJECT_NAME}_BUILD_TOOLS "Toggle building of biogears command line tools" ON)
if(${ROOT_PROJECT_NAME}_BUILD_TOOLS)

###############################################################################
# Requirments
###############################################################################
if (${PREFIX}_BUILD_STATIC)
  add_definitions("-D${PREFIX}_BUILT_AS_STATIC")
  add_definitions("-DARACORE_BUILT_AS_STATIC")
endif()

###############################################################################
#Sorce and Header Defines
###############################################################################
message(STATUS "")
message(STATUS "Configuring ${PROJECT_NAME}")

#Scenario Driver
add_source_files(HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/include "*.h"   "Headers\\Public\\")
add_source_files(SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/ "*.cpp"   "Sources\\")
add_source_files(CONTROLLER_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/ "bg-cli-main.cpp" "Sources\\")
add_source_files(DRONE_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/ "bg-scenario-main.cpp" "Sources\\")
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
  DEBUG_POSTFIX ${CMAKE_DEBUG_POSTFIX}
  CXX_STANDARD 14
  VS_DEBUGGER_WORKING_DIRECTORY ${PROJECT_BINARY_DIR}/runtime
  )


target_compile_options(bg-cli PRIVATE $<$<PLATFORM_ID:Windows>:/bigobj> PRIVATE $<$<PLATFORM_ID:Windows>:/MP${${ROOT_PROJECT_NAME}_BUILD_PARALLEL_LEVEL}>)
target_include_directories(bg-cli PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include PRIVATE ${CMAKE_CURRENT_BINARY_DIR}/src/utils/ PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src/)

target_link_libraries(bg-cli  ${${PREFIX}_LIBS}      
      Biogears::libbiogears
      minizip::lib
)

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
  DEBUG_POSTFIX ${CMAKE_DEBUG_POSTFIX}
  CXX_STANDARD 14
  VS_DEBUGGER_WORKING_DIRECTORY ${PROJECT_BINARY_DIR}/runtime
  )


target_compile_options(bg-scenario PRIVATE $<$<PLATFORM_ID:Windows>:/bigobj> PRIVATE $<$<PLATFORM_ID:Windows>:/MP${${ROOT_PROJECT_NAME}_BUILD_PARALLEL_LEVEL}>)
target_include_directories(bg-scenario PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include PRIVATE ${CMAKE_CURRENT_BINARY_DIR}/src/utils/ PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src/)
target_link_libraries(bg-scenario  ${${PREFIX}_LIBS} 
      Biogears::libbiogears
      minizip::lib )

add_dependencies(bg-cli bg-scenario)


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

   target_compile_definitions(bg-scenario PRIVATE BIOGEARS_SUBPROCESS_SUPPORT)
   target_link_libraries(bg-scenario 
      Boost::disable_autolinking
      Boost::dynamic_linking
      Boost::program_options
      Boost::filesystem
   )
endif()
####
install(TARGETS bg-cli bg-scenario
     RUNTIME DESTINATION bin
     LIBRARY DESTINATION ${LIBRARY_INSTALL_DIR}
     ARCHIVE DESTINATION lib
)
message(STATUS "")
endif()
