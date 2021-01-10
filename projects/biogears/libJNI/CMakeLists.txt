###############################################################################
#Sorce and Header Defines
###############################################################################
message(STATUS "\t adding libbiogears_jni")

#Biogears JNI
add_source_files(JNI_PUBLIC_HDRS ${BIOGEARS_SOURCE_DIR}/libJNI/include "*.h"  "Headers\\Public\\")
add_source_files(JNI_SRCS ${BIOGEARS_SOURCE_DIR}/libJNI/src "*cpp" "Sources\\")

set(LIB${PREFIX}_JNI_HEADERS ${JNI_PUBLIC_HDRS})
set(LIB${PREFIX}_JNI_SOURCES ${JNI_SRCS})

###############################################################################
# libBiogears_jni
###############################################################################
if(Java_FOUND AND ${ROOT_PROJECT_NAME}_BUILD_JAVATOOLS AND ${ROOT_PROJECT_NAME}_BUILD_CIRCUIT_TESTS)
  add_library(lib${PROJECT_NAME}_jni  SHARED ${LIB${PREFIX}_JNI_SOURCES} ${LIB${PREFIX}_JNI_HEADERS})
  add_library(Biogears::lib${PROJECT_NAME}_jni ALIAS lib${PROJECT_NAME}_jni)
  set_target_properties(lib${PROJECT_NAME}_jni
      PROPERTIES
      DEFINE_SYMBOL ${PROJECT_NAME}_jni_EXPORTS
      FOLDER "Libraries"
      OUTPUT_NAME "${PROJECT_NAME}_jni"
      COMPILE_PDB_NAME "${PROJECT_NAME}_jni"
      PROJECT_LABEL "lib${PROJECT_NAME}_jni"
      DEBUG_POSTFIX ${CMAKE_DEBUG_POSTFIX}
      CXX_STANDARD 14
  )
  set_target_properties(lib${PROJECT_NAME}_jni PROPERTIES SOVERSION "${${ROOT_PROJECT_NAME}_LIB_VERSION}")
  ###############################################################################
  # libBiogears_jni COMPILATION & LINKAGE MODIFICATIONS
  ###############################################################################
  set( ${PREFIX}_CPPFLAGS $<$<PLATFORM_ID:Windows>:_SCL_SECURE_NO_WARNINGS _CRT_SECURE_NO_WARNINGS NOMINMAX > )
  
  target_compile_definitions(lib${PROJECT_NAME}_jni  PUBLIC ${${PREFIX}_CPPFLAGS_INTERFACE})
  target_compile_definitions(lib${PROJECT_NAME}_jni  PRIVATE ${${PREFIX}_CPPFLAGS} )
  target_compile_options(lib${PROJECT_NAME}_jni PRIVATE $<$<PLATFORM_ID:Windows>:/bigobj>  PRIVATE $<$<PLATFORM_ID:Windows>:/MP${${ROOT_PROJECT_NAME}_BUILD_PARALLEL_LEVEL}>)
  
  target_include_directories(lib${PROJECT_NAME}_jni 
      PRIVATE  ${${PREFIX}_INCLUDE_DIR} ${${PREFIX}_PRIVATE_INCLUDE_DIR} ${${PREFIX}_GENERATED_INCLUDE_DIR} ${BIOGEARS_SOURCE_DIR}/libJNI/include
      PUBLIC   ${JNI_INCLUDE_DIRS}
      INTERFACE 
        $<BUILD_INTERFACE:${BIOGEARS_SOURCE_DIR}/libJNI/include>
        $<INSTALL_INTERFACE:include>
  )
  
  set(${PREFIX}_jni_LIBS
      CodeSynthesis::XSD
      Eigen3::Eigen
      Biogears::lib${PROJECT_NAME}_circuit_test
  )
  
  target_link_libraries(lib${PROJECT_NAME}_jni  PUBLIC ${${PREFIX}_jni_LIBS} )
  
  install(TARGETS lib${PROJECT_NAME}_jni EXPORT libBiogears
          RUNTIME DESTINATION bin/${${ROOT_PROJECT_NAME}_PLATFORM_PREFIX}$<$<BOOL:${${ROOT_PROJECT_NAME}_BREAKOUT_INSTALL}>:/$<CONFIG>>
          LIBRARY DESTINATION ${LIBRARY_INSTALL_DIR}/${${ROOT_PROJECT_NAME}_PLATFORM_PREFIX}$<$<BOOL:${${ROOT_PROJECT_NAME}_BREAKOUT_INSTALL}>:/$<CONFIG>>
          ARCHIVE DESTINATION lib/${${ROOT_PROJECT_NAME}_PLATFORM_PREFIX}$<$<BOOL:${${ROOT_PROJECT_NAME}_BREAKOUT_INSTALL}>:/$<CONFIG>>
  )
  if(WIN32)
    install(FILES $<TARGET_PDB_FILE:lib${PROJECT_NAME}_jni> DESTINATION lib/${${ROOT_PROJECT_NAME}_PLATFORM_PREFIX}$<$<BOOL:${${ROOT_PROJECT_NAME}_BREAKOUT_INSTALL}>:/$<CONFIG>> OPTIONAL)
  endif()
endif()

######################################################################################################
# INSTALL LOGIC
######################################################################################################

