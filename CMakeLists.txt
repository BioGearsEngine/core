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
set(${ROOT_PROJECT_NAME}_CMAKE_MIN_VERSION 3.8.0)
set(${ROOT_PROJECT_NAME}_CMAKE_POLICY 3.8.0)

cmake_minimum_required(VERSION ${${ROOT_PROJECT_NAME}_CMAKE_MIN_VERSION})

# Policy adjustments
cmake_policy(VERSION ${${ROOT_PROJECT_NAME}_CMAKE_POLICY})
if(POLICY CMP0054)
  cmake_policy(SET CMP0054 NEW)
endif()

include(cmake/cmake-common_logic.cmake)


################################################################################
# STEP 1: 
# Change this to custimize your project
################################################################################
project( ${ROOT_PROJECT_NAME}  LANGUAGES CXX C)

 if(NOT EXISTS ${ARA_${ROOT_PROJECT_NAME}_EXTERNAL})
    set(ARA_${ROOT_PROJECT_NAME}_EXTERNAL 
    "ARA_${ROOT_PROJECT_NAME}_EXTERNAL-NOTFOUND" 
    CACHE PATH
    "Directory containing UNIX style installation of external dependences" 
    )
    message(FATAL_ERROR   "
    ARA_${ROOT_PROJECT_NAME}_EXTERNAL NOTFOUND
    You must set this variable to an existing dir to continue")
 endif()

 list(APPEND CMAKE_MODULE_PATH  ${PROJECT_SOURCE_DIR}/cmake  ${PROJECT_SOURCE_DIR}/cmake/common ${ARA_${ROOT_PROJECT_NAME}_EXTERNAL}/cmake)
 list(APPEND CMAKE_PREFIX_PATH  ${ARA_${ROOT_PROJECT_NAME}_EXTERNAL} )
 list(APPEND CMAKE_LIBRARY_PATH  ${ARA_${ROOT_PROJECT_NAME}_EXTERNAL}/lib )
 list(APPEND CMAKE_INCLUDE_PATH ${ARA_${ROOT_PROJECT_NAME}_EXTERNAL}/include )
 list(APPEND CMAKE_FIND_ROOT_PATH ${ARA_${ROOT_PROJECT_NAME}_EXTERNAL} ${ARA_${ROOT_PROJECT_NAME}_EXTERNAL}/cmake ${PROJECT_SOURCE_DIR}/cmake/common)

 message(STATUS "
    Using the following search paths
    CMAKE_MODULE_PATH = ${CMAKE_MODULE_PATH}
    CMAKE_PREFIX_PATH  = ${CMAKE_PREFIX_PATH}
    CMAKE_LIBRARY_PATH = ${CMAKE_LIBRARY_PATH}
    CMAKE_INCLUDE_PATH = ${CMAKE_INCLUDE_PATH}
    CMAKE_FIND_ROOT_PATH = ${CMAKE_FIND_ROOT_PATH}
    ")
###############################################################################
# Add Repository Subdirectories
###############################################################################
add_subdirectory(projects)
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
if(ARA_${ROOT_PROJECT_NAME}_BUILD_${ROOT_PROJECT_NAME})
  add_dependencies(STAGE ${ROOT_PROJECT_NAME})
  add_dependencies(STAGE scenario_driver)
endif()

###############################################################################
# Step 3:
# Infrastructure Installation for each library
# your project outputs create a Find${lib}.cmake file
# list them here. This will install them in your sysroot
# so other projects can build off those libraries
###############################################################################
install(FILES cmake/Find${ROOT_PROJECT_NAME}.cmake
        DESTINATION cmake/)
