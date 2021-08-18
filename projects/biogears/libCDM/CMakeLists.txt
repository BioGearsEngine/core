###############################################################################
#Sorce and Header Defines
###############################################################################
message(STATUS "  Configuring libbiogears_cdm")

option(UNITTEST_biogears-core_cdm "Enable libBiogears_cdm UnitTest "  ON)

#Biogears CDM
add_source_files(CDM_PUBLIC_HDRS LOCATION ${BIOGEARS_SOURCE_DIR}/libCDM/include/biogears/ REGEX "*.h" SOURCE_GROUP "Headers\\Public\\")
add_source_files(CDM_GEN_HDRS LOCATION ${BIOGEARS_BINARY_DIR}/generated/biogears/schema REGEX "*.hxx" SOURCE_GROUP  "Headers\\Public\\")
add_source_files(CDM_GEN_SRCS LOCATION ${BIOGEARS_BINARY_DIR}/generated/biogears/schema REGEX "*.cxx" SOURCE_GROUP "Sources\\")

set(LIB${PREFIX}_CDM_HEADERS ${CDM_PUBLIC_HDRS} ${CDM_GEN_HDRS})
set(LIB${PREFIX}_CDM_SOURCES ${CDM_PUBLIC_SRCS} ${CDM_GEN_SRCS})

###############################################################################
#Code Generation
###############################################################################

  ###############################################################################
  # libBiogears_cdm
  ###############################################################################
  add_library(lib${PROJECT_NAME}_cdm    SHARED ${LIB${PREFIX}_CDM_SOURCES} ${LIB${PREFIX}_CDM_HEADERS})
  add_library(Biogears::lib${PROJECT_NAME}_cdm ALIAS lib${PROJECT_NAME}_cdm)
  set_target_properties(lib${PROJECT_NAME}_cdm
    PROPERTIES
    DEFINE_SYMBOL ${PROJECT_NAME}_cdm_EXPORTS
    FOLDER "Libraries"
    OUTPUT_NAME "${PROJECT_NAME}_cdm"
    COMPILE_PDB_NAME "${PROJECT_NAME}_cdm"
    PROJECT_LABEL "lib${PROJECT_NAME}_cdm"
    DEBUG_POSTFIX "${CMAKE_DEBUG_POSTFIX}"
    CXX_STANDARD 14
    )
  set_target_properties(lib${PROJECT_NAME}_cdm PROPERTIES SOVERSION "${${ROOT_PROJECT_NAME}_LIB_VERSION}")
  add_dependencies(lib${PROJECT_NAME}_cdm ${XSD_TARGETS})

  ###############################################################################
  # libBiogears_cdm COMPILATION & LINKAGE MODIFICATIONS
  ###############################################################################
  set( ${PREFIX}_CPPFLAGS $<$<PLATFORM_ID:Windows>:_SCL_SECURE_NO_WARNINGS _CRT_SECURE_NO_WARNINGS NOMINMAX > )
  target_compile_definitions(lib${PROJECT_NAME}_cdm  PUBLIC ${${PREFIX}_CPPFLAGS_INTERFACE} $<$<STREQUAL:${CMAKE_SYSTEM_PROCESSOR},armv7-a>:XSD_NO_EXPORT>)
  target_compile_definitions(lib${PROJECT_NAME}_cdm  PRIVATE ${${PREFIX}_CPPFLAGS} )
  target_compile_options(lib${PROJECT_NAME}_cdm PUBLIC $<$<PLATFORM_ID:Android>:-w> PRIVATE $<$<PLATFORM_ID:Windows>:/bigobj>  PRIVATE $<$<PLATFORM_ID:Windows>:/MP${${ROOT_PROJECT_NAME}_BUILD_PARALLEL_LEVEL}>)
  
  target_include_directories(lib${PROJECT_NAME}_cdm 
      PRIVATE  ${${PREFIX}_INCLUDE_DIR} ${${PREFIX}_PRIVATE_INCLUDE_DIR} ${${PREFIX}_GENERATED_INCLUDE_DIR} ${BIOGEARS_SOURCE_DIR}/libCDM/include
      INTERFACE 
                $<BUILD_INTERFACE:${BIOGEARS_SOURCE_DIR}/libCDM/include>
                $<INSTALL_INTERFACE:include>
  )
  
  set(${PREFIX}_cdm_LIBS
            CodeSynthesis::XSD
            Eigen3::Eigen
  )
  target_link_libraries(lib${PROJECT_NAME}_cdm  PUBLIC ${${PREFIX}_cdm_LIBS} )

  ######################################################################################################
  # INSTALL LOGIC
  ######################################################################################################
  install(TARGETS lib${PROJECT_NAME}_cdm  EXPORT libBiogears
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
	install(FILES $<TARGET_PDB_FILE:lib${PROJECT_NAME}_cdm> DESTINATION lib/${${ROOT_PROJECT_NAME}_PLATFORM_PREFIX}$<$<BOOL:${${ROOT_PROJECT_NAME}_BREAKOUT_INSTALL}>:/$<CONFIG>> OPTIONAL)
  endif()

