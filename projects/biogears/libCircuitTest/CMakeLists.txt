###############################################################################
#Sorce and Header Defines
###############################################################################
message(STATUS "  Configuring libbiogears_circuit_test")

option(UNITTEST_biogears-circuit "Enable Biogears circuit UnitTest "  ${UNITTEST_biogears-core})

#Biogears circuit_test
add_source_files(CIRCUIT_TEST_PUBLIC_HDRS LOCATION ${BIOGEARS_SOURCE_DIR}/libCircuitTest/include REGEX "*.h" SOURCE_GROUP "Headers\\Public\\")
add_source_files(CIRCUIT_TEST_SRCS LOCATION ${BIOGEARS_SOURCE_DIR}/libCircuitTest/src REGEX  "*cpp" SOURCE_GROUP "Sources\\")

set(LIB${PREFIX}_CIRCUIT_TEST_HEADERS ${CIRCUIT_TEST_HDRS} ${PUBLIC_CIRCUIT_TEST_HDRS} ${GEN_CIRCUIT_TEST_HDRS})
set(LIB${PREFIX}_CIRCUIT_TEST_SOURCES ${CIRCUIT_TEST_SRCS} ${GEN_CIRCUIT_TEST_SRCS})

###############################################################################
# libBiogears_circuit_test
###############################################################################
if( ${ROOT_PROJECT_NAME}_BUILD_CIRCUIT_TESTS)
    add_library(lib${PROJECT_NAME}_circuit_test  SHARED ${LIB${PREFIX}_CIRCUIT_TEST_HEADERS} ${LIB${PREFIX}_CIRCUIT_TEST_SOURCES})
    add_library(Biogears::lib${PROJECT_NAME}_circuit_test ALIAS lib${PROJECT_NAME}_circuit_test)
    set_target_properties(lib${PROJECT_NAME}_circuit_test
    PROPERTIES
    DEFINE_SYMBOL ${PROJECT_NAME}_CIRCUIT_TEST_EXPORTS
    FOLDER "Libraries"
    OUTPUT_NAME "${PROJECT_NAME}_circuit_test"
    COMPILE_PDB_NAME "${PROJECT_NAME}_circuit_test"
    PROJECT_LABEL "lib${PROJECT_NAME}_circuit_test"
    DEBUG_POSTFIX "${CMAKE_DEBUG_POSTFIX}"
    CXX_STANDARD 14
    )
  set_target_properties(lib${PROJECT_NAME}_circuit_test PROPERTIES SOVERSION "${${ROOT_PROJECT_NAME}_LIB_VERSION}")
  ###############################################################################
  # libBiogears_circuit_test COMPILATION & LINKAGE MODIFICATIONS
  ###############################################################################
  set( ${PREFIX}_CPPFLAGS $<$<PLATFORM_ID:Windows>:_SCL_SECURE_NO_WARNINGS _CRT_SECURE_NO_WARNINGS NOMINMAX > )
  target_compile_definitions(lib${PROJECT_NAME}_circuit_test  PUBLIC ${${PREFIX}_CPPFLAGS_INTERFACE})
  target_compile_definitions(lib${PROJECT_NAME}_circuit_test  PRIVATE ${${PREFIX}_CPPFLAGS} )
  target_compile_options(lib${PROJECT_NAME}_circuit_test PRIVATE $<$<PLATFORM_ID:Windows>:/bigobj>  PRIVATE $<$<PLATFORM_ID:Windows>:/MP${${ROOT_PROJECT_NAME}_BUILD_PARALLEL_LEVEL}>)

  target_include_directories(lib${PROJECT_NAME}_circuit_test 
      PRIVATE  ${${PREFIX}_INCLUDE_DIR} ${${PREFIX}_PRIVATE_INCLUDE_DIR} ${${PREFIX}_GENERATED_INCLUDE_DIR} ${BIOGEARS_SOURCE_DIR}/libCircuitTest/include
      PUBLIC   ${CIRCUIT_TEST_INCLUDE_DIRS}
      INTERFACE 
                $<BUILD_INTERFACE:${BIOGEARS_SOURCE_DIR}/libCircuitTest/include>
                $<INSTALL_INTERFACE:include>
  )

  set(${PREFIX}_CIRCUIT_TEST_LIBS
            CodeSynthesis::XSD
            Eigen3::Eigen
            Biogears::lib${PROJECT_NAME}
  )
  target_link_libraries(lib${PROJECT_NAME}_circuit_test  PUBLIC ${${PREFIX}_CIRCUIT_TEST_LIBS} )

  ######################################################################################################
  # INSTALL LOGIC
  ######################################################################################################
  install(TARGETS lib${PROJECT_NAME}_circuit_test EXPORT libBiogears
      RUNTIME DESTINATION bin/${${ROOT_PROJECT_NAME}_PLATFORM_PREFIX}$<$<BOOL:${${ROOT_PROJECT_NAME}_BREAKOUT_INSTALL}>:/$<CONFIG>>
      LIBRARY DESTINATION ${LIBRARY_INSTALL_DIR}/${${ROOT_PROJECT_NAME}_PLATFORM_PREFIX}$<$<BOOL:${${ROOT_PROJECT_NAME}_BREAKOUT_INSTALL}>:/$<CONFIG>>
      ARCHIVE DESTINATION lib/${${ROOT_PROJECT_NAME}_PLATFORM_PREFIX}$<$<BOOL:${${ROOT_PROJECT_NAME}_BREAKOUT_INSTALL}>:/$<CONFIG>>
  )

  install(DIRECTORY 
      include/biogears
      DESTINATION include/
      FILE_PERMISSIONS OWNER_READ GROUP_READ WORLD_READ
  )

  if(WIN32)
    install(FILES $<TARGET_PDB_FILE:lib${PROJECT_NAME}_circuit_test> DESTINATION lib/${${ROOT_PROJECT_NAME}_PLATFORM_PREFIX}$<$<BOOL:${${ROOT_PROJECT_NAME}_BREAKOUT_INSTALL}>:/$<CONFIG>> OPTIONAL)
  endif()
endif()
