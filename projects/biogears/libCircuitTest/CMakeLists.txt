###############################################################################
#Sorce and Header Defines
###############################################################################
message(STATUS "  Configuring libbiogears_circuit_test")

option(UNITTEST_biogears-circuit "Enable Biogears circuit UnitTest "  ${UNITTEST_biogears-core})

#Biogears circuit_test
add_source_files(CIRCUIT_TEST_PUBLIC_HDRS LOCATION ${BIOGEARS_SOURCE_DIR}/libCircuitTest/include REGEX "*.h" SOURCE_GROUP "Headers\\Public\\")
add_source_files(CIRCUIT_TEST_SRCS LOCATION ${BIOGEARS_SOURCE_DIR}/libCircuitTest/src REGEX  "*cpp" SOURCE_GROUP "Sources\\")

set(LIBBIO_CIRCUIT_TEST_HEADERS ${CIRCUIT_TEST_HDRS} ${PUBLIC_CIRCUIT_TEST_HDRS} ${GEN_CIRCUIT_TEST_HDRS})
set(LIBBIO_CIRCUIT_TEST_SOURCES ${CIRCUIT_TEST_SRCS} ${GEN_CIRCUIT_TEST_SRCS})

###############################################################################
# libBiogears_circuit_test
###############################################################################
if( Biogears_BUILD_CIRCUIT_TESTS)
    add_library(libbiogears_circuit_test  SHARED ${LIBBIO_CIRCUIT_TEST_HEADERS} ${LIBBIO_CIRCUIT_TEST_SOURCES})
    add_library(Biogears::libbiogears_circuit_test ALIAS libbiogears_circuit_test)
    set_target_properties(libbiogears_circuit_test
    PROPERTIES
    DEFINE_SYMBOL biogears_CIRCUIT_TEST_EXPORTS
    FOLDER "Libraries"
    OUTPUT_NAME "biogears_circuit_test"
    COMPILE_PDB_NAME "biogears_circuit_test"
    PROJECT_LABEL "libbiogears_circuit_test"
    DEBUG_POSTFIX "${CMAKE_DEBUG_POSTFIX}"
    CXX_STANDARD 14
    )
  set_target_properties(libbiogears_circuit_test PROPERTIES SOVERSION "${Biogears_LIB_VERSION}")
  ###############################################################################
  # libBiogears_circuit_test COMPILATION & LINKAGE MODIFICATIONS
  ###############################################################################
  set( BIO_CPPFLAGS $<$<PLATFORM_ID:Windows>:_SCL_SECURE_NO_WARNINGS _CRT_SECURE_NO_WARNINGS NOMINMAX > )
  target_compile_definitions(libbiogears_circuit_test  PUBLIC ${BIO_CPPFLAGS_INTERFACE})
  target_compile_definitions(libbiogears_circuit_test  PRIVATE ${BIO_CPPFLAGS} )
  target_compile_options(libbiogears_circuit_test PRIVATE $<$<PLATFORM_ID:Windows>:/bigobj>  PRIVATE $<$<PLATFORM_ID:Windows>:/MP${Biogears_BUILD_PARALLEL_LEVEL}>)

  target_include_directories(libbiogears_circuit_test 
      PRIVATE  ${BIO_INCLUDE_DIR} ${BIO_PRIVATE_INCLUDE_DIR} ${BIO_GENERATED_INCLUDE_DIR} ${BIOGEARS_SOURCE_DIR}/libCircuitTest/include
      PUBLIC   ${CIRCUIT_TEST_INCLUDE_DIRS}
      INTERFACE 
                $<BUILD_INTERFACE:${BIOGEARS_SOURCE_DIR}/libCircuitTest/include>
                $<INSTALL_INTERFACE:include>
  )

  set(BIO_CIRCUIT_TEST_LIBS
            CodeSynthesis::XSD
            Eigen3::Eigen
            Biogears::libbiogears
  )
  target_link_libraries(libbiogears_circuit_test  PUBLIC ${BIO_CIRCUIT_TEST_LIBS} )

  ######################################################################################################
  # INSTALL LOGIC
  ######################################################################################################
  install(TARGETS libbiogears_circuit_test EXPORT libBiogears
      RUNTIME DESTINATION bin/${Biogears_PLATFORM_PREFIX}$<$<BOOL:${Biogears_BREAKOUT_INSTALL}>:/$<CONFIG>>
      LIBRARY DESTINATION ${LIBRARY_INSTALL_DIR}/${Biogears_PLATFORM_PREFIX}$<$<BOOL:${Biogears_BREAKOUT_INSTALL}>:/$<CONFIG>>
      ARCHIVE DESTINATION lib/${Biogears_PLATFORM_PREFIX}$<$<BOOL:${Biogears_BREAKOUT_INSTALL}>:/$<CONFIG>>
  )

  install(DIRECTORY 
      include/biogears
      DESTINATION include/
      FILE_PERMISSIONS OWNER_READ GROUP_READ WORLD_READ
  )

  if(WIN32 AND NOT Biogears_BUILD_STATIC)
    install(FILES $<TARGET_PDB_FILE:libbiogears_circuit_test> DESTINATION lib/${Biogears_PLATFORM_PREFIX}$<$<BOOL:${Biogears_BREAKOUT_INSTALL}>:/$<CONFIG>> OPTIONAL)
  endif()
endif()
