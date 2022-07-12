
  ###############################################################################
  # Python SWIG Bindings libBiogears_py
  ###############################################################################
  if(SWIG_python_FOUND AND Biogears_SWIG_PYTHON_BINDINGS)
   message(STATUS "            : libbiogears_py")

   find_package(Python3  COMPONENTS Interpreter Development REQUIRED)

   if(Biogears_BUILD_PYBIND11_BINDINGS)
     message(FATAL_ERROR "Unable to build PyBind11 and SWIG Python3 Hooks at the same time. Please disable at least one project")
   endif()

   if(Python3_FOUND)
     #if(Python_VERSION VERSION_GREATER_EQUAL 3)
     #  list(APPEND CMAKE_SWIG_FLAGS "-py3;-DPY3")
     #endif()

     file(GLOB_RECURSE Python_SWIGS LIST_DIRECTORIES False "${CMAKE_CURRENT_SOURCE_DIR}/../bindings/*.swg" )
     source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}/bindings/*.swg" PREFIX "Source Files"  FILES ${csharp_test_files} )
     foreach(_file IN LISTS Python_SWIGS)
       set_property(SOURCE ${_file} PROPERTY CPLUSPLUS ON)
       set_property(SOURCE ${_file} PROPERTY UseSWIG_MODULE_VERSION 2)
       set_property(SOURCE ${_file} PROPERTY USE_TARGET_INCLUDE_DIRECTORIES ON)

      set_property(SOURCE ${_file} PROPERTY COMPILE_OPTIONS "-py3;-DPY3")
       get_filename_component(_file_name ${_file} NAME_WE)
       set_property(SOURCE ${_file} PROPERTY SWIG_MODULE_NAME ${_file_name} )
     endforeach()
     
     option(UNITTEST_biogears-swig_py     "Enable libBiogears_py UnitTest " ${Biogears_SWIG_PYTHON_BINDINGS})
     swig_add_library(libbiogears_py
           TYPE SHARED 
           LANGUAGE python 
           OUTPUT_DIR "${PROJECT_BINARY_DIR}/outputs/python"
           OUTFILE_DIR "${PROJECT_BINARY_DIR}/outputs/python"
           SOURCES  ${Python_SWIGS}
     )
     add_library(Biogears::libbiogears_py ALIAS libbiogears_py)
     target_include_directories(libbiogears_py PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/../bindings"
                                                      PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/../../libbiogears/include"
     )
     target_link_libraries(libbiogears_py PUBLIC libbiogears Python3::Python)
     set_target_properties(libbiogears_py PROPERTIES
       POSITION_INDEPENDENT_CODE ON
       PROJECT_LABEL libbiogears_py
       OUTPUT_NAME biogears
       FOLDER Libraries
       ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/outputs/python"
       ARCHIVE_OUTPUT_NAME      "_biogears"
       LIBRARY_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/outputs/python" 
       RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/outputs/python" 
       PDB_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/outputs/python"
     )

     get_property(support_files TARGET libbiogears_py PROPERTY SWIG_SUPPORT_FILES)
    ######################################################################################################
    # INSTALL LOGIC
    ######################################################################################################
    install(TARGETS libbiogears_py EXPORT libBiogears
             RUNTIME DESTINATION bin
             LIBRARY DESTINATION ${LIBRARY_INSTALL_DIR}
            ARCHIVE DESTINATION lib
    )
    if(WIN32 AND NOT Biogears_BUILD_STATIC)
      install(FILES $<TARGET_PDB_FILE:libbiogears_py> DESTINATION lib/${Biogears_PLATFORM_PREFIX}$<$<BOOL:${Biogears_BREAKOUT_INSTALL}>:/$<CONFIG>> OPTIONAL)
    endif()
   endif()
  endif()

