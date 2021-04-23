
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
           
     foreach(_file IN LISTS Python_SWIGS)
       set_property(SOURCE ${_file} PROPERTY CPLUSPLUS ON)
       set_property(SOURCE ${_file} PROPERTY UseSWIG_MODULE_VERSION 2)
       set_property(SOURCE ${_file} PROPERTY USE_TARGET_INCLUDE_DIRECTORIES ON)

      set_property(SOURCE ${_file} PROPERTY COMPILE_OPTIONS "-py3;-DPY3")
       get_filename_component(_file_name ${_file} NAME_WE)
       set_property(SOURCE ${_file} PROPERTY SWIG_MODULE_NAME ${_file_name} )
     endforeach()
     
     
     swig_add_library(lib${PROJECT_NAME}_py
           TYPE SHARED 
           LANGUAGE python 
           OUTPUT_DIR "${PROJECT_BINARY_DIR}/outputs/python"
           OUTFILE_DIR "${PROJECT_BINARY_DIR}/outputs/python"
           SOURCES  ${Python_SWIGS}
     )
     add_library(Biogears::lib${PROJECT_NAME}_py ALIAS lib${PROJECT_NAME}_py)
     target_include_directories(lib${PROJECT_NAME}_py PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/../bindings"
                                                      PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/../../libbiogears/include"
     )
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

