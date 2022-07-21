###############################################################################
#Sorce and Header Defines
###############################################################################
message(STATUS "  Configuring libbiogears_cdm")

option(UNITTEST_biogears-core_cdm "Enable libBiogears_cdm UnitTest "  ON)

#Biogears CDM
add_source_files(CDM_PUBLIC_HDRS LOCATION ${BIOGEARS_SOURCE_DIR}/libCDM/include/biogears/ REGEX "*.h" SOURCE_GROUP "Headers\\Public\\")
add_source_files(CDM_GEN_HDRS LOCATION ${BIOGEARS_BINARY_DIR}/generated/biogears/schema REGEX "*.hxx" SOURCE_GROUP  "Headers\\Public\\")
add_source_files(CDM_GEN_SRCS LOCATION ${BIOGEARS_BINARY_DIR}/generated/biogears/schema REGEX "*.cxx" SOURCE_GROUP "Sources\\")

set(LIBBIO_CDM_HEADERS ${CDM_PUBLIC_HDRS} ${CDM_GEN_HDRS})
set(LIBBIO_CDM_SOURCES ${CDM_PUBLIC_SRCS} ${CDM_GEN_SRCS})

###############################################################################
#Code Generation
###############################################################################

  ###############################################################################
  # libBiogears_cdm
  ###############################################################################
  add_library(libbiogears_cdm    SHARED ${LIBBIO_CDM_SOURCES} ${LIBBIO_CDM_HEADERS})
  add_library(Biogears::libbiogears_cdm ALIAS libbiogears_cdm)
  set_target_properties(libbiogears_cdm
    PROPERTIES
    DEFINE_SYMBOL biogears_cdm_EXPORTS
    FOLDER "Libraries"
    OUTPUT_NAME "biogears_cdm"
    COMPILE_PDB_NAME "biogears_cdm"
    PROJECT_LABEL "libbiogears_cdm"
    DEBUG_POSTFIX "${CMAKE_DEBUG_POSTFIX}"
    CXX_STANDARD 14
    )
  set_target_properties(libbiogears_cdm PROPERTIES SOVERSION "${Biogears_LIB_VERSION}")
  add_dependencies(libbiogears_cdm xsd_bindings)

  ###############################################################################
  # libBiogears_cdm COMPILATION & LINKAGE MODIFICATIONS
  ###############################################################################
  set( BIO_CPPFLAGS $<$<PLATFORM_ID:Windows>:_SCL_SECURE_NO_WARNINGS _CRT_SECURE_NO_WARNINGS NOMINMAX > )
  target_compile_definitions(libbiogears_cdm  PUBLIC ${BIO_CPPFLAGS_INTERFACE} $<$<STREQUAL:${CMAKE_SYSTEM_PROCESSOR},armv7-a>:XSD_NO_EXPORT>)
  target_compile_definitions(libbiogears_cdm  PRIVATE ${BIO_CPPFLAGS} )
  target_compile_options(libbiogears_cdm PUBLIC $<$<PLATFORM_ID:Android>:-w> PRIVATE $<$<PLATFORM_ID:Windows>:/bigobj>  PRIVATE $<$<PLATFORM_ID:Windows>:/MP${Biogears_BUILD_PARALLEL_LEVEL}>)
  
  target_include_directories(libbiogears_cdm 
      PRIVATE  ${BIO_INCLUDE_DIR} ${BIO_PRIVATE_INCLUDE_DIR} ${BIO_GENERATED_INCLUDE_DIR} ${BIOGEARS_SOURCE_DIR}/libCDM/include
      INTERFACE 
                $<BUILD_INTERFACE:${BIOGEARS_SOURCE_DIR}/libCDM/include>
                $<INSTALL_INTERFACE:include>
  )
  
  set(BIO_cdm_LIBS
            CodeSynthesis::XSD
            Eigen3::Eigen
  )
  target_link_libraries(libbiogears_cdm  PUBLIC ${BIO_cdm_LIBS} )

  ######################################################################################################
  # INSTALL LOGIC
  ######################################################################################################
  install(TARGETS libbiogears_cdm  EXPORT libBiogears
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
	install(FILES $<TARGET_PDB_FILE:libbiogears_cdm> DESTINATION lib/${Biogears_PLATFORM_PREFIX}$<$<BOOL:${Biogears_BREAKOUT_INSTALL}>:/$<CONFIG>> OPTIONAL)
  endif()

