###############################################################################
#Sorce and Header Defines
###############################################################################
if( ${ROOT_PROJECT_NAME}_BUILD_SWIG_BINDINGS )
  find_package(SWIG REQUIRED COMPONENTS python csharp OPTIONAL_COMPONENTS fortran)
  
  message(STATUS "  Configuring swig_bindings")
   
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

  
    add_subdirectory(csharp)
    option (${ROOT_PROJECT_NAME}_SWIG_CSHARP_BINDINGS "Toggle the Genearting of languge bindings using SHARP (Experimental)"  ${SWIG_FOUND})
    #add_subdirectory(python)
    #option (${ROOT_PROJECT_NAME}_SWIG_PYTHON_BINDINGS "Toggle the building of Python Hooks. pybind11 is a dependency of this process (Experimental)"  ${SWIG_FOUND})
  endif()
endif() #pybind11_FOUND

