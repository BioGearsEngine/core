
###############################################################################
# CSharp SWIG Bindings biogears_csharp
###############################################################################
if(SWIG_csharp_FOUND AND Biogears_SWIG_CSHARP_BINDINGS)
   message(STATUS "            : biogears_csharp")
   
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
   
   if(Biogears_BUILD_STATIC)
       set(LIBRARY_TYPE STATIC)
   else()
       set(LIBRARY_TYPE SHARED)
   endif()

   option(UNITTEST_biogears-swig_csharp "Enable biogears_csharp UnitTest " ${Biogears_SWIG_CSHARP_BINDINGS})
   set(SWIG_GEN_DIR "${CMAKE_CURRENT_BINARY_DIR}/generated/")
   set(SWIG_STAGE_DIR "${CMAKE_BINARY_DIR}/outputs/bindings/csharp/")
   swig_add_library(biogears_csharp 
	   TYPE ${LIBRARY_TYPE}
         LANGUAGE csharp 
         OUTPUT_DIR  "${SWIG_STAGE_DIR}"
         OUTFILE_DIR "${SWIG_GEN_DIR}"
         SOURCES ${CSharp_SWIGS}
   )
   target_link_libraries(biogears_csharp PUBLIC libbiogears )
   target_include_directories(biogears_csharp PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/../bindings")
   target_compile_options(biogears_csharp PRIVATE $<$<PLATFORM_ID:Windows>:/bigobj> PRIVATE $<$<PLATFORM_ID:Windows>:/MP${Biogears_BUILD_PARALLEL_LEVEL}>)
   set_target_properties(biogears_csharp PROPERTIES
      POSITION_INDEPENDENT_CODE ON
      FOLDER Libraries
      CXX_STANDARD 14
   )  
   get_property(support_files TARGET biogears_csharp PROPERTY SWIG_SUPPORT_FILES)

    ######################################################################################################
    # INSTALL LOGIC
    ######################################################################################################
    install(TARGETS biogears_csharp EXPORT libBiogears
             RUNTIME DESTINATION bin
             LIBRARY DESTINATION ${LIBRARY_INSTALL_DIR}
             ARCHIVE DESTINATION lib
    )
    if(WIN32 AND NOT Biogears_BUILD_STATIC)
      install(FILES $<TARGET_PDB_FILE:biogears_csharp> DESTINATION lib/${Biogears_PLATFORM_PREFIX}$<$<BOOL:${Biogears_BREAKOUT_INSTALL}>:/$<CONFIG>> OPTIONAL)
    endif()


  ######################################################################################################
  # CSharp Unit Test
  ######################################################################################################
 file(GLOB_RECURSE  swig_generated_csharp  LIST_DIRECTORIES False CONFIGURE_DEPENDS "${SWIG_STAGE_DIR}/*.cs")
 
  set_property(GLOBAL PROPERTY USE_FOLDERS ON)
  get_property(support_files TARGET biogears_csharp PROPERTY SWIG_SUPPORT_FILES)

  include(CSharpUtilities)
  foreach(file IN LISTS support_files)
    LIST(APPEND libiogears_csharp_biogears ${file})
  endforeach()

  if (WIN32)
    enable_language( CSharp )
    add_executable(csharp_example ${swig_generated_csharp}  src/howto-swig_bindings.cs)
    add_executable(csharp_radiation_debugging ${swig_generated_csharp}  src/howto-swig_radiation.cs)

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
      
    target_link_libraries(csharp_example PUBLIC  libbiogears biogears_csharp)
    target_include_directories(csharp_example PUBLIC ${CMAKE_CURRENT_BINARY_DIR}/bindings )

    target_link_libraries(csharp_radiation_debugging PUBLIC  libbiogears biogears_csharp)
    target_include_directories(csharp_radiation_debugging PUBLIC ${CMAKE_CURRENT_BINARY_DIR}/bindings ${CMAKE_CURRENT_BINARY_DIR}/bindings/biogears)
  endif()
endif()


