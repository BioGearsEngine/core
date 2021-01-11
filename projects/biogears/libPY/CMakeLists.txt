###############################################################################
#Sorce and Header Defines
###############################################################################
message(STATUS "\t adding libbiogears_py")

#Biogears CDM
add_source_files(PY_HDRS LOCATION ${BIOGEARS_SOURCE_DIR}/libPy/src REGEX "*.h" SOURCE_GROUP "Headers\\Private\\")
add_source_files(PY_SRCS LOCATION ${BIOGEARS_SOURCE_DIR}/libPy/src REGEX "*.cpp" SOURCE_GROUP "Sources\\")
set(LIB${PREFIX}_PY_HEADERS ${PY_HDRS})
set(LIB${PREFIX}_PY_SOURCES ${PY_SRCS})

#Define Logic
###############################################################################
 

  ###############################################################################
  # libBiogears_Py
  ###############################################################################
     pybind11_add_module(lib${PROJECT_NAME}_py  SHARED ${LIB${PREFIX}_PY_SOURCES} ${LIB${PREFIX}_PY_HEADERS})
     add_library(Biogears::lib${PROJECT_NAME}_py ALIAS lib${PROJECT_NAME}_py)
  	   set_target_properties(lib${PROJECT_NAME}_py
       PROPERTIES
       DEFINE_SYMBOL ${PROJECT_NAME}_py_EXPORTS
       FOLDER "Libraries"
       OUTPUT_NAME "py${PROJECT_NAME}"
       COMPILE_PDB_NAME "${PROJECT_NAME}_py"
       PROJECT_LABEL "lib${PROJECT_NAME}_py"
       DEBUG_POSTFIX ${CMAKE_DEBUG_POSTFIX}
       CXX_STANDARD 14
       )
     if(WIN32)
       set_target_properties(lib${PROJECT_NAME}_py PROPERTIES SUFFIX ".pyd")
     elseif(APPLE)
       set_target_properties(lib${PROJECT_NAME}_py PROPERTIES SUFFIX ".dylib")
     else()
     set_target_properties(lib${PROJECT_NAME}_py PROPERTIES SUFFIX ".so")
   endif()
   ###############################################################################
   # libBiogears_py COMPILATION & LINKAGE MODIFICATIONS
   ###############################################################################
     set( ${PREFIX}_CPPFLAGS $<$<PLATFORM_ID:Windows>:_SCL_SECURE_NO_WARNINGS _CRT_SECURE_NO_WARNINGS NOMINMAX > )
     target_compile_definitions(lib${PROJECT_NAME}_py  PUBLIC ${${PREFIX}_CPPFLAGS_INTERFACE})
     target_compile_definitions(lib${PROJECT_NAME}_py  PRIVATE ${${PREFIX}_CPPFLAGS} )
     target_compile_options(lib${PROJECT_NAME}_py PRIVATE $<$<PLATFORM_ID:Windows>:/bigobj>  PRIVATE $<$<PLATFORM_ID:Windows>:/MP>)

     target_include_directories(lib${PROJECT_NAME}_py 
         PRIVATE  ${${PREFIX}_INCLUDE_DIR} ${${PREFIX}_PRIVATE_INCLUDE_DIR} ${${PREFIX}_GENERATED_INCLUDE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/libPY/include
               PUBLIC   ${PY_INCLUDE_DIRS}
         INTERFACE 
                   $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/libPY/include>
                   $<INSTALL_INTERFACE:include>
     )

    set(${PREFIX}_py_LIBS
        CodeSynthesis::XSD
        Eigen3::Eigen
        pybind11::module
        Biogears::lib${PROJECT_NAME}
              
    )
    
    target_link_libraries(lib${PROJECT_NAME}_py  PUBLIC ${${PREFIX}_py_LIBS} )


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
