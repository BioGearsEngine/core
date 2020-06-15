# Options
###############################################################################
option(Biogears_Python_BUILD_WEBSITE "Use Python to build the website" OFF)


###############################################################################
# Base Variables
###############################################################################
set(PROJECT_NAME Python_BioGears)
set(PREFIX biogears_py)

set(${PREFIX}_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include" PARENT_SCOPE)
set(${PREFIX}_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include" )
set(${PREFIX}_PRIVATE_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src" PARENT_SCOPE)
set(${PREFIX}_PRIVATE_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src" )
set(${PREFIX}_GENERATED_INCLUDE_DIR "${CMAKE_CURRENT_BINARY_DIR}" PARENT_SCOPE)
set(${PREFIX}_GENERATED_INCLUDE_DIR "${CMAKE_CURRENT_BINARY_DIR}" )
set(${PREFIX}_UNIT_DIR "${CMAKE_CURRENT_SOURCE_DIR}/unit" PARENT_SCOPE)

set(biogears_reference_file "D:/biogears/references/BioGears.bib")
set(doxygen_dot_dir "C:/Program Files/graphviz-2.38/release/bin/")
###############################################################################
# Requirments
###############################################################################
if(Biogears_Python_BUILD_WEBSITE)
    find_package(Perl REQUIRED)
        find_package(Doxygen REQUIRED)
        find_package (Python COMPONENTS Interpreter Development REQUIRED )
endif()

