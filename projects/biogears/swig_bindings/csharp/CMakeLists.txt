
###############################################################################
# CSharp SWIG Bindings libBiogears_csharp
###############################################################################
if(SWIG_csharp_FOUND AND ${ROOT_PROJECT_NAME}_SWIG_CSHARP_BINDINGS)
   message(STATUS "            : lib${PROJECT_NAME}_csharp")
   enable_language( CSharp )
   set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/outputs/chsarp" PARENT_SCOPE)
   set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/outputs/chsarp" PARENT_SCOPE)
   set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/outputs/chsarp" PARENT_SCOPE)

   foreach(_file IN LISTS CSharp_SWIGS)
     set_property(SOURCE ${_file} PROPERTY CPLUSPLUS ON)
     set_property(SOURCE ${_file} PROPERTY UseSWIG_MODULE_VERSION 2)
     set_property(SOURCE ${_file} PROPERTY USE_TARGET_INCLUDE_DIRECTORIES ON)

     get_filename_component(_file_name ${_file} NAME_WE)
     set_property(SOURCE ${_file} PROPERTY SWIG_MODULE_NAME "SWIG_${_file_name}" )
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
   target_include_directories(lib${PROJECT_NAME}_csharp PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/../bindings")
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
  add_source_files(libiogears_csharp_files  LOCATION ${CMAKE_BINARY_DIR}/outputs/csharp/std/ REGEX "*.cs" SOURCE_GROUP "Source Files\\")
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


