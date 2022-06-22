
###############################################################################
# CSharp SWIG Bindings libBiogears_csharp
###############################################################################
if(SWIG_csharp_FOUND AND ${ROOT_PROJECT_NAME}_SWIG_CSHARP_BINDINGS)
   message(STATUS "            : lib${PROJECT_NAME}_csharp")
   enable_language( CSharp )
   set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/outputs/chsarp" PARENT_SCOPE)
   set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/outputs/chsarp" PARENT_SCOPE)
   set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/outputs/chsarp" PARENT_SCOPE)

   file(GLOB_RECURSE CSharp_SWIGS LIST_DIRECTORIES False CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/../bindings/*.swg" )
   source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}/bindings/*.swg" PREFIX "Source Files"  FILES ${csharp_test_files} )

   foreach(_file IN LISTS CSharp_SWIGS)
     set_property(SOURCE ${_file} PROPERTY CPLUSPLUS ON)
     set_property(SOURCE ${_file} PROPERTY UseSWIG_MODULE_VERSION 2)
     set_property(SOURCE ${_file} PROPERTY USE_TARGET_INCLUDE_DIRECTORIES ON)

     get_filename_component(_file_name ${_file} NAME_WE)
     set_property(SOURCE ${_file} PROPERTY SWIG_MODULE_NAME "SWIG_${_file_name}" )
     set_property(SOURCE ${_file} PROPERTY COMPILE_OPTIONS )
   endforeach()
   
   option(UNITTEST_biogears-swig_csharp "Enable libBiogears_csharp UnitTest " ${${ROOT_PROJECT_NAME}_SWIG_CSHARP_BINDINGS})
   swig_add_library(lib${PROJECT_NAME}_csharp 
         TYPE SHARED 
         LANGUAGE csharp 
         OUTPUT_DIR "${PROJECT_BINARY_DIR}/outputs/csharp"
         OUTFILE_DIR "${PROJECT_BINARY_DIR}/outputs/csharp"
         SOURCES ${CSharp_SWIGS}
   )
   target_link_libraries(lib${PROJECT_NAME}_csharp PUBLIC lib${PROJECT_NAME} )
   target_include_directories(lib${PROJECT_NAME}_csharp PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/../bindings")
   target_compile_options(lib${PROJECT_NAME}_csharp PRIVATE $<$<PLATFORM_ID:Windows>:/bigobj> PRIVATE $<$<PLATFORM_ID:Windows>:/MP${${ROOT_PROJECT_NAME}_BUILD_PARALLEL_LEVEL}>)
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
  file(GLOB_RECURSE  libiogears_csharp_biogears  LIST_DIRECTORIES False CONFIGURE_DEPENDS "${CMAKE_BINARY_DIR}/outputs/csharp/biogears/*.cs")
  file(GLOB_RECURSE  libiogears_csharp_cdm  LIST_DIRECTORIES False CONFIGURE_DEPENDS "${CMAKE_BINARY_DIR}/outputs/csharp/mil/*.cs")
  file(GLOB_RECURSE  libiogears_csharp_std  LIST_DIRECTORIES False CONFIGURE_DEPENDS "${CMAKE_BINARY_DIR}/outputs/csharp/std/*.cs")
  file(GLOB_RECURSE  libiogears_csharp_unittest  LIST_DIRECTORIES False CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cs")
  
  set_property(GLOBAL PROPERTY USE_FOLDERS ON)
  source_group(TREE ${CMAKE_BINARY_DIR}/outputs/csharp/ PREFIX "Source Files"  FILES ${libiogears_csharp_biogears} ${libiogears_csharp_cdm} ${libiogears_csharp_std})
  source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}/src PREFIX "Source Files"  FILES ${libiogears_csharp_unittest} )
  get_property(support_files TARGET lib${PROJECT_NAME}_csharp PROPERTY SWIG_SUPPORT_FILES)

  include(CSharpUtilities)
  foreach(file IN LISTS support_files)
    LIST(APPEND libiogears_csharp_biogears ${file})
  endforeach()

  add_executable(csharp_example ${libiogears_csharp_biogears} ${libiogears_csharp_cdm} ${libiogears_csharp_std} src/howto-swig_bindings.cs)
  add_executable(csharp_radiation_debugging ${libiogears_csharp_biogears} ${libiogears_csharp_cdm} ${libiogears_csharp_std} src/howto-swig_radiation.cs)
  set_target_properties(csharp_example PROPERTIES
      POSITION_INDEPENDENT_CODE ON
      FOLDER "Swig\\CSharp"
      PROJECT_LABEL "Example"
      FOLDERS ON
    )
  set_target_properties(csharp_radiation_debugging PROPERTIES
      POSITION_INDEPENDENT_CODE ON
      FOLDER "Swig\\CSharp"
      PROJECT_LABEL "Debugging"
      FOLDERS ON
    )
    
  target_link_libraries(csharp_example PUBLIC  lib${PROJECT_NAME} lib${PROJECT_NAME}_csharp)
  target_include_directories(csharp_example PUBLIC ${CMAKE_BINARY_DIR}/outputs/csharp ${CMAKE_BINARY_DIR}/outputs/csharp/biogears)

  target_link_libraries(csharp_radiation_debugging PUBLIC  lib${PROJECT_NAME} lib${PROJECT_NAME}_csharp)
  target_include_directories(csharp_radiation_debugging PUBLIC ${CMAKE_BINARY_DIR}/outputs/csharp ${CMAKE_BINARY_DIR}/outputs/csharp/biogears)
endif()


