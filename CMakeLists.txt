###############################################################################
# CMAKE DEFINITIONS AND setTINGS
#
# Requires the following environmental variables:
#
#   BUILD TYPE      ENV VARIABLE        Description
#   ANDROID         ANDROID_NDK         Android NDK root folder location
#                                           (also required for toolchain file)
#   ANDROID         GRADLE_EXECUTABLE   Location of the Gradle executable
###############################################################################
set(ROOT_PROJECT_NAME Biogears)
set(${ROOT_PROJECT_NAME}_CMAKE_MIN_VERSION 3.11.0)
set(${ROOT_PROJECT_NAME}_CMAKE_POLICY 3.11.0)

cmake_minimum_required(VERSION ${${ROOT_PROJECT_NAME}_CMAKE_MIN_VERSION})

include(cmake/cmake-common_logic.cmake)


################################################################################
# STEP 1: 
# Change this to custimize your project
################################################################################
project( ${ROOT_PROJECT_NAME}  LANGUAGES CXX C)

 list(APPEND CMAKE_MODULE_PATH   ${PROJECT_SOURCE_DIR}/cmake  ${PROJECT_SOURCE_DIR}/cmake/common )

 message(STATUS "
    Using the following search paths
    CMAKE_MODULE_PATH = ${CMAKE_MODULE_PATH}
    CMAKE_PREFIX_PATH  = ${CMAKE_PREFIX_PATH}
    CMAKE_FIND_ROOT_PATH = ${CMAKE_FIND_ROOT_PATH}
    ")

find_package(Git)
if(Git_FOUND)
  configure_version_information(GIT_SUCESS)
endif()
if(NOT GIT_SUCESS)
  # Update these variables to ensure out of source build have some sane tags
  message(WARNING "GIT_EXECUTABLE was not found or no corrisponding GIT_REPO is avaliable.\n" 
    "If you are building with in a git repo you should ensure GIT is in your current path. " 
	"If you are building outside of a repo ignore this message.")
  set(${ROOT_PROJECT_NAME}_VERSION_MAJOR 7)
  set(${ROOT_PROJECT_NAME}_VERSION_MINOR 2)
  set(${ROOT_PROJECT_NAME}_VERSION_PATCH 2)
  set(${ROOT_PROJECT_NAME}_VERSION_TWEAK 0)
  set(${ROOT_PROJECT_NAME}_LIB_VERSION ${${ROOT_PROJECT_NAME}_VERSION_MAJOR}.${${ROOT_PROJECT_NAME}_VERSION_MINOR} )
  set(${ROOT_PROJECT_NAME}_DIRTY_BUILD true)
endif()
###############################################################################
# Add Repository Subdirectories
###############################################################################
add_subdirectory(projects)
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT scenario_driver)
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
if(${ROOT_PROJECT_NAME}_BUILD_${ROOT_PROJECT_NAME})
    add_dependencies(STAGE ${ROOT_PROJECT_NAME})
  if( TARGET EXISTS scenario_driver)
    add_dependencies(STAGE scenario_driver)
  elseif ( TARGET EXISTS scenario_driver)
    add_dependencies(STAGE test_driver)
  endif()
  if( TARGET EXISTS unittest)
    add_dependencies(STAGE test_driver)
  endif()
endif()

###############################################################################
# Step 3:
# Infrastructure Installation for each library
# your project outputs create a Find${lib}.cmake file
# list them here. This will install them in your sysroot
# so other projects can build off those libraries
###############################################################################
install(EXPORT lib${ROOT_PROJECT_NAME}
  NAMESPACE ${ROOT_PROJECT_NAME}::
  FILE ${ROOT_PROJECT_NAME}.cmake
  DESTINATION lib/cmake/${ROOT_PROJECT_NAME}
)

include(CMakePackageConfigHelpers)
# generate the config file that is includes the exports
configure_package_config_file(${PROJECT_SOURCE_DIR}/cmake/${ROOT_PROJECT_NAME}Config.cmake.in
  "${CMAKE_CURRENT_BINARY_DIR}/${ROOT_PROJECT_NAME}Config.cmake"
  INSTALL_DESTINATION "lib/cmake/${ROOT_PROJECT_NAME}"
  NO_SET_AND_CHECK_MACRO
  NO_CHECK_REQUIRED_COMPONENTS_MACRO
)

# generate the version file for the config file
write_basic_package_version_file(
  "${CMAKE_CURRENT_BINARY_DIR}/${ROOT_PROJECT_NAME}ConfigVersion.cmake"
  VERSION "${${ROOT_PROJECT_NAME}_VERSION_MAJOR}.${${ROOT_PROJECT_NAME}_VERSION_MINOR}"
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
  ${CMAKE_CURRENT_BINARY_DIR}/${ROOT_PROJECT_NAME}Config.cmake
  ${Packages}
  DESTINATION lib/cmake/${ROOT_PROJECT_NAME}
)

generate_project_suffix()
