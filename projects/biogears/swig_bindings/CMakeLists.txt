###############################################################################
#Sorce and Header Defines
###############################################################################
find_package(SWIG REQUIRED COMPONENTS python csharp OPTIONAL_COMPONENTS fortran)

message(STATUS "  Configuring swig_bindings")

option(UNITTEST_biogears-py     "Enable libBiogears_py UnitTest " ${UNITTEST_biogears-core})
option(UNITTEST_biogears-csharp "Enable libBiogears_csharp UnitTest " ${UNITTEST_biogears-core})

set(LIB${PREFIX}_CSharp_HEADERS ${CSharp_HDRS})
set(LIB${PREFIX}_CSharp_SOURCES ${CSharp_SRCS})

###############################################################################
#Define Logic
###############################################################################
 

###############################################################################
# SWIG Bindings
###############################################################################
if(SWIG_FOUND)
  cmake_policy(SET CMP0078 NEW)
  cmake_policy(SET CMP0086 NEW)
  include(UseSWIG)
  set(SWIG_SOURCE_FILE_EXTENSIONS ".swg")
  set(UseSWIG_TARGET_NAME_PREFERENCE STANDARD)
 
  option (${ROOT_PROJECT_NAME}_SWIG_PYTHON_BINDINGS "Toggle the building of Python Hooks. pybind11 is a dependency of this process (Experimental)"  ${SWIG_python_FOUND})
  option (${ROOT_PROJECT_NAME}_SWIG_CSHARP_BINDINGS "Toggle the Genearting of languge bindings using SHARP (Experimental)"  ${SWIG_csharp_FOUND})
  
 file(GLOB_RECURSE CSharp_SWIGS LIST_DIRECTORIES False CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/bindings/*.swg" )
 file(GLOB_RECURSE Python_SWIGS LIST_DIRECTORIES False "${CMAKE_CURRENT_SOURCE_DIR}/bindings/*.swg" )
  source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}/bindings/*.swg" PREFIX "Source Files"  FILES ${csharp_test_files} )

  add_subdirectory(csharp)
  #add_subdirectory(python)
endif()

