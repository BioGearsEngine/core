###############################################################################
# CMAKE DEFINITIONS AND SETTINGS
#
# Requires the following environmental variables:
#
#   BUILD TYPE      ENV VARIABLE        Description
#   ANDROID         ANDROID_NDK         Android NDK root folder location
#                                           (also required for toolchain file)
#   ANDROID         GRADLE_EXECUTABLE   Location of the Gradle executable
###############################################################################
set(Biogears_CMAKE_MIN_VERSION 3.11.0)
set(Biogears_CMAKE_POLICY 3.11.0)

cmake_minimum_required(VERSION ${Biogears_CMAKE_MIN_VERSION})

include(cmake/cmake-common_logic.cmake)


################################################################################
# STEP 1: 
# Change this to custimize your project
################################################################################
project( Biogears  LANGUAGES CXX C )

 list(APPEND CMAKE_MODULE_PATH   ${PROJECT_SOURCE_DIR}/cmake  ${PROJECT_SOURCE_DIR}/cmake/common )

 message(STATUS "
    Using the following search paths
    CMAKE_MODULE_PATH = ${CMAKE_MODULE_PATH}
    CMAKE_PREFIX_PATH  = ${CMAKE_PREFIX_PATH}
    CMAKE_FIND_ROOT_PATH = ${CMAKE_FIND_ROOT_PATH}
    ")

find_package(Git QUIET)
configure_version_information(GIT_SUCESS MAJOR 7 MINOR 5 PATCH 0 TWEAK "Source" )
if(GIT_FOUND AND NOT GIT_SUCESS)
  # Update these variables to ensure out of source build have some sane tags
  message(WARNING "GIT_EXECUTABLE was not found or no corresponding GIT_REPO is available." 
  "If you are building with in a git repo you should ensure GIT is in your current path." 
	"If you are building outside of a repo ignore this message.")
endif()
###############################################################################
# Add Repository Subdirectories
###############################################################################
add_subdirectory(projects)

if(TARGET bg-cli)
	set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT bg-cli)
endif()

if(NOT TARGET gather_runtime_dependencies)
  ###############################################################################
  # Stage Code for IDE Testing
  ###############################################################################
  create_cache_file()
  create_stage()
  ###############################################################################
  # Step 2:
  # You likely need to add on project to stage for every executable you would like
  # to stage (AKA move dep DLLs to the run folder for debugging.
  ################################################################################
  if(TARGET  scenario_driver)
    add_dependencies(gather_runtime_dependencies scenario_driver)
  endif()
  if( TARGET test_driver)
    add_dependencies(gather_runtime_dependencies test_driver)
  endif()
  if(TARGET  unittest)
    add_dependencies(gather_runtime_dependencies unittest)
  endif()
endif()

###############################################################################
# Step 3:
# Infrastructure Installation for each library
# your project outputs create a Find${lib}.cmake file
# list them here. This will install them in your sysroot
# so other projects can build off those libraries
###############################################################################
install(EXPORT libBiogears
  NAMESPACE Biogears::
  FILE Biogears.cmake
  DESTINATION lib/cmake/Biogears
)

include(CMakePackageConfigHelpers)
# generate the config file that is includes the exports
configure_package_config_file(${PROJECT_SOURCE_DIR}/cmake/BiogearsConfig.cmake.in
  "${CMAKE_CURRENT_BINARY_DIR}/BiogearsConfig.cmake"
  INSTALL_DESTINATION "lib/cmake/Biogears"
  NO_SET_AND_CHECK_MACRO
  NO_CHECK_REQUIRED_COMPONENTS_MACRO
)

# generate the version file for the config file
write_basic_package_version_file(
  "${CMAKE_CURRENT_BINARY_DIR}/BiogearsConfigVersion.cmake"
  VERSION "${Biogears_VERSION_MAJOR}.${Biogears_VERSION_MINOR}"
  COMPATIBILITY AnyNewerVersion
)

# install the configuration file
if(NOT Log4cpp_DIR)
  list(APPEND Packages ${CMAKE_CURRENT_SOURCE_DIR}/cmake/common/FindLog4cpp.cmake)
endif()
if(NOT Xerces-c_DIR)
  list(APPEND Packages ${CMAKE_CURRENT_SOURCE_DIR}/cmake/common/FindXerces-c.cmake)
endif()
if(NOT CodeSynthesis_DIR)
  list(APPEND Packages ${CMAKE_CURRENT_SOURCE_DIR}/cmake/common/FindCodeSynthesis.cmake)
endif()
install(FILES
  ${CMAKE_CURRENT_BINARY_DIR}/BiogearsConfig.cmake
  ${Packages}
  DESTINATION lib/cmake/Biogears
)

if( PROJECT_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR )
  message(STATUS "Preparing Installer Packages")
  add_subdirectory(cmake/packaging)
endif()

generate_project_suffix()
