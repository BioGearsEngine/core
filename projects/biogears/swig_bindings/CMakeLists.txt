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
  
  ###############################################################################
  # Python SWIG Bindings libBiogears_py
  ###############################################################################
  if(SWIG_python_FOUND AND ${ROOT_PROJECT_NAME}_SWIG_PYTHON_BINDINGS)
   message(STATUS "            : lib${PROJECT_NAME}_py")

   find_package(Python3  COMPONENTS Interpreter Development REQUIRED)

   if(${ROOT_PROJECT_NAME}_BUILD_PYBIND11_BINDINGS)
     message(FATAL_ERROR "Unable to build PyBind11 and SWIG Python3 Hooks at the same time. Please disable at least one project")
   endif()

   if(Python3_FOUND)
     #if(Python_VERSION VERSION_GREATER_EQUAL 3)
     #  list(APPEND CMAKE_SWIG_FLAGS "-py3;-DPY3")
     #endif()

     add_source_files(Python_SWIGS LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/src REGEX "*.swg" SOURCE_GROUP "Tempaltes\\")
     
     foreach(_file IN LISTS Python_SWIGS)
       set_property(SOURCE ${_file} PROPERTY CPLUSPLUS ON)
       set_property(SOURCE ${_file} PROPERTY UseSWIG_MODULE_VERSION 2)
       set_property(SOURCE ${_file} PROPERTY USE_TARGET_INCLUDE_DIRECTORIES ON)
       #file(RELATIVE_PATH relative_outdir  ${CMAKE_CURRENT_SOURCE_DIR}/src ${_file})
       #get_filename_component(relative_outdir ${relative_outdir} DIRECTORY)
       #set_property(SOURCE ${_file} PROPERTY OUTPUT_DIR "${PROJECT_BINARY_DIR}/outputs/python/${relative_outdir}" )
       set_property(SOURCE ${_file} PROPERTY COMPILE_OPTIONS "-py3;-DPY3")
     endforeach()
     
     
     swig_add_library(lib${PROJECT_NAME}_py
           TYPE SHARED 
           LANGUAGE python 
           OUTPUT_DIR "${PROJECT_BINARY_DIR}/outputs/python"
           OUTFILE_DIR "${PROJECT_BINARY_DIR}/outputs/python"
           SOURCES  ${Python_SWIGS}
     )
     add_library(Biogears::lib${PROJECT_NAME}_py ALIAS lib${PROJECT_NAME}_py)

     target_link_libraries(lib${PROJECT_NAME}_py PUBLIC lib${PROJECT_NAME} Python3::Python)
     set_target_properties(lib${PROJECT_NAME}_py PROPERTIES
       POSITION_INDEPENDENT_CODE ON
       PROJECT_LABEL lib${PROJECT_NAME}_py
       OUTPUT_NAME ${PROJECT_NAME}
       FOLDER Libraries
       ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/outputs/python"
       ARCHIVE_OUTPUT_NAME      "_biogears"
       LIBRARY_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/outputs/python" 
       RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/outputs/python" 
       PDB_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/outputs/python"
     )

     get_property(support_files TARGET lib${PROJECT_NAME}_py PROPERTY SWIG_SUPPORT_FILES)
    ######################################################################################################
    # INSTALL LOGIC
    ######################################################################################################
    install(TARGETS lib${PROJECT_NAME}_py EXPORT libBiogears
             RUNTIME DESTINATION bin
             LIBRARY DESTINATION ${LIBRARY_INSTALL_DIR}
            ARCHIVE DESTINATION lib
    )
    if(WIN32)
      install(FILES $<TARGET_PDB_FILE:lib${PROJECT_NAME}_py> DESTINATION lib/${${ROOT_PROJECT_NAME}_PLATFORM_PREFIX}$<$<BOOL:${${ROOT_PROJECT_NAME}_BREAKOUT_INSTALL}>:/$<CONFIG>> OPTIONAL)
    endif()
   endif()
  endif()

  ###############################################################################
  # CSharp SWIG Bindings libBiogears_csharp
  ###############################################################################
  if(SWIG_csharp_FOUND AND ${ROOT_PROJECT_NAME}_SWIG_CSHARP_BINDINGS)
   message(STATUS "            : lib${PROJECT_NAME}_csharp")
   enable_language( CSharp )
   set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/outputs/chsarp" PARENT_SCOPE)
   set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/outputs/chsarp" PARENT_SCOPE)
   set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/outputs/chsarp" PARENT_SCOPE)

   add_source_files(CSharp_SWIGS LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/src REGEX "*.swg" SOURCE_GROUP "Tempaltes\\")

   foreach(_file IN LISTS CSharp_SWIGS)
     set_property(SOURCE ${_file} PROPERTY CPLUSPLUS ON)
     set_property(SOURCE ${_file} PROPERTY UseSWIG_MODULE_VERSION 2)
     set_property(SOURCE ${_file} PROPERTY USE_TARGET_INCLUDE_DIRECTORIES ON)
     #file(RELATIVE_PATH relative_outdir  ${CMAKE_CURRENT_SOURCE_DIR}/src ${_file})
     #get_filename_component(relative_outdir ${relative_outdir} DIRECTORY)
     #set_property(SOURCE ${_file} PROPERTY OUTPUT_DIR  "${PROJECT_BINARY_DIR}/outputs/csharp/${relative_outdir}")
     set_property(SOURCE ${_file} PROPERTY COMPILE_OPTIONS )
   endforeach()

   swig_add_library(lib${PROJECT_NAME}_csharp 
         TYPE SHARED 
         LANGUAGE csharp 
         OUTPUT_DIR "${PROJECT_BINARY_DIR}/outputs/csharp"
         OUTFILE_DIR "${PROJECT_BINARY_DIR}/outputs/csharp"
         SOURCES ${CSharp_SWIGS}
   )
   target_link_libraries(lib${PROJECT_NAME}_csharp PUBLIC lib${PROJECT_NAME} )
   set_target_properties(lib${PROJECT_NAME}_csharp PROPERTIES
      POSITION_INDEPENDENT_CODE ON
      FOLDER Libraries
   )  
   get_property(support_files TARGET lib${PROJECT_NAME}_csharp PROPERTY SWIG_SUPPORT_FILES)

    ######################################################################################################
    # INSTALL LOGIC
    ######################################################################################################
    install(TARGETS lib${PROJECT_NAME}_csharp EXPORT libBiogears
             RUNTIME DESTINATION bin
             LIBRARY DESTINATION ${LIBRARY_INSTALL_DIR}
             ARCHIVE DESTINATION lib
    )
    if(WIN32)
      install(FILES $<TARGET_PDB_FILE:lib${PROJECT_NAME}_csharp> DESTINATION lib/${${ROOT_PROJECT_NAME}_PLATFORM_PREFIX}$<$<BOOL:${${ROOT_PROJECT_NAME}_BREAKOUT_INSTALL}>:/$<CONFIG>> OPTIONAL)
    endif()


  ######################################################################################################
  # CSharp Unit Test
  ######################################################################################################

  #add_source_files(libiogears_csharp_files LOCATION ${CMAKE_BINARY_DIR}/outputs/csharp/ REGEX "*.cs" SOURCE_GROUP "Source Files\\")
  add_source_files(libiogears_csharp_files  LOCATION ${CMAKE_BINARY_DIR}/outputs/csharp/biogears/ REGEX "*.cs" SOURCE_GROUP "Source Files\\")
  add_source_files(csharp_test_files LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/src REGEX "*.cs" SOURCE_GROUP "Source Files\\")
  
  set_property(GLOBAL PROPERTY USE_FOLDERS ON)
  source_group(TREE ${CMAKE_BINARY_DIR}/outputs/csharp/ PREFIX "Source Files"  FILES ${libiogears_csharp_files} )
  source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}/src PREFIX "Source Files"  FILES ${csharp_test_files} )
  get_property(support_files TARGET lib${PROJECT_NAME}_csharp PROPERTY SWIG_SUPPORT_FILES)

  include(CSharpUtilities)
  foreach(file IN LISTS support_files)
    LIST(APPEND libiogears_csharp_files ${file})

  endforeach()

  add_executable(unittest_csharp ${libiogears_csharp_files} ${csharp_test_files} )
  set_target_properties(unittest_csharp PROPERTIES
      POSITION_INDEPENDENT_CODE ON
      FOLDER UnitTest
      PROJECT_LABEL "UnitTest_CSharp"
      FOLDERS ON
    )
    
  target_link_libraries(unittest_csharp PUBLIC  lib${PROJECT_NAME} lib${PROJECT_NAME}_csharp)
  target_include_directories(unittest_csharp PUBLIC ${CMAKE_BINARY_DIR}/outputs/csharp ${CMAKE_BINARY_DIR}/outputs/csharp/biogears)
  endif()
endif()

