###############################################################################
#Sorce and Header Defines
###############################################################################
message(STATUS "  Configuring libbiogears")

#Biogears CDM
add_source_files(CDM_PUBLIC_HDRS LOCATION ${BIOGEARS_SOURCE_DIR}/libCDM/include/ REGEX "*.h" SOURCE_GROUP "Headers\\Public\\")
add_source_files(CDM_GEN_HDRS LOCATION ${BIOGEARS_BINARY_DIR}/biogears REGEX "*.hxx" SOURCE_GROUP "Headers\\Public\\")
add_source_files(CDM_GEN_SRCS LOCATION  ${BIOGEARS_BINARY_DIR}/biogears REGEX "*.cxx" SOURCE_GROUP "Sources\\")

#Biogears Core
add_source_files(CORE_HDRS LOCATION ${BIOGEARS_SOURCE_DIR}/libBiogears/src REGEX "*.h" SOURCE_GROUP "Headers\\Private\\")
add_source_files(CORE_SRCS LOCATION ${BIOGEARS_SOURCE_DIR}/libBiogears/src REGEX "*.cpp" SOURCE_GROUP "Sources\\")
add_source_files(CORE_GEN_SRCS LOCATION ${BIOGEARS_BINARY_DIR}/libBiogears/src REGEX "version.cpp" SOURCE_GROUP"Sources\\")
add_source_files(CORE_PUBLIC_HDRS LOCATION ${BIOGEARS_SOURCE_DIR}/libBiogears/include REGEX "*.h" SOURCE_GROUP "Headers\\Public\\")
add_source_files(CORE_PUBLIC_HDRS LOCATION ${BIOGEARS_SOURCE_DIR}/libBiogears/include REGEX "*.inl" SOURCE_GROUP "Headers\\Public\\")

set(LIB${PREFIX}_CDM_HEADERS ${CDM_HDRS} ${CDM_PUBLIC_HDRS} ${CDM_GEN_HDRS})
set(LIB${PREFIX}_CDM_SOURCES ${CDM_SRCS} ${CDM_GEN_SRCS})

set(LIB${PREFIX}_CORE_HEADERS ${CORE_HDRS} ${CORE_PUBLIC_HDRS} ${CORE_GEN_HDRS})
set(LIB${PREFIX}_CORE_SOURCES ${CORE_SRCS} ${CORE_GEN_SRCS})

###############################################################################
#Define Logic
###############################################################################
if(${PREFIX}_BUILD_STATIC)
  add_library(lib${PROJECT_NAME} STATIC ${LIB${PREFIX}_CORE_SOURCES} ${LIB${PREFIX}_CORE_HEADERS} 
                                        ${LIB${PREFIX}_CDM_HEADERS}  ${LIB${PREFIX}_CDM_SOURCES})
  add_library(Biogears::lib${PROJECT_NAME} ALIAS lib${PROJECT_NAME})
	set_target_properties(lib${PROJECT_NAME}
    PROPERTIES
    DEFINE_SYMBOL ${PROJECT_NAME}_EXPORTS
    FOLDER "Libraries"
    OUTPUT_NAME "${PROJECT_NAME}"
    COMPILE_PDB_NAME "${PROJECT_NAME}"
    PROJECT_LABEL "lib${PROJECT_NAME}"
    DEBUG_POSTFIX "${CMAKE_DEBUG_POSTFIX}"
    CXX_STANDARD 14
    )
  set_target_properties(lib${PROJECT_NAME} PROPERTIES SOVERSION "${${ROOT_PROJECT_NAME}_LIB_VERSION}")
   if(${PROJECT_NAME}_USE_OVERRIDE_CONTROL)
      target_compile_definitions( lib${PROJECT_NAME} PRIVATE BIOGEARS_USE_OVERRIDE_CONTROL)
   endif()
  if(UNITTEST_biogears-core)
     target_compile_definitions( lib${PROJECT_NAME} PUBLIC ENABLE_UNIT_TEST_API)
  endif()
  if(WIN32)
    list(APPEND ${PREFIX}_CPPFLAGS _SCL_SECURE_NO_WARNINGS _CRT_SECURE_NO_WARNINGS NOMINMAX $ENV{PARALLEL_COMPILE} )
  elseif(CMAKE_COMPILER_IS_GNUCXX)
    list(APPEND ${PREFIX}_CPPFLAGS "-Wattributes" ${CodeSynthesis_CPPFLAGS})
    list(APPEND ${PREFIX}_LDFLAGS "-Wl,--no-as-needed" )
    set_target_properties(lib${PROJECT_NAME} PROPERTIES LINK_FLAGS ${${PREFIX}_LDFLAGS})
  endif()
  ###############################################################################
  # COMPILATION & LINKAGE MODIFICATIONS
  ###############################################################################
  target_compile_definitions(lib${PROJECT_NAME} PUBLIC ${PREFIX}_BUILT_AS_STATIC ${${PREFIX}_CPPFLAGS_INTERFACE} $<$<STREQUAL:${CMAKE_SYSTEM_PROCESSOR},armv7-a>:XSD_NO_EXPORT>)
  target_compile_definitions(lib${PROJECT_NAME} PRIVATE ${${PREFIX}_CPPFLAGS} )
  target_compile_options(lib${PROJECT_NAME} PRIVATE $<$<PLATFORM_ID:Windows>:/bigobj>  
                                            PRIVATE $<$<PLATFORM_ID:Windows>:/MP${${ROOT_PROJECT_NAME}_BUILD_PARALLEL_LEVEL}>
                                            )
  if(${ROOT_PROJECT_NAME}_THROW_READONLY_EXCEPTIONS)
        target_compile_definitions(lib${PROJECT_NAME} PUBLIC BIOGEARS_THROW_READONLY_EXCEPTIONS)
  endif()
   if(${ROOT_PROJECT_NAME}_THROW_NAN_EXCEPTIONS )
        target_compile_definitions(lib${PROJECT_NAME} PUBLIC BIOGEARS_THROW_NAN_EXCEPTIONS )
  endif()
  target_include_directories(lib${PROJECT_NAME}
      PRIVATE  ${${PREFIX}_INCLUDE_DIR} ${${PREFIX}_PRIVATE_INCLUDE_DIR} ${${PREFIX}_GENERATED_INCLUDE_DIR}
      INTERFACE $<BUILD_INTERFACE:${${PREFIX}_GENERATED_INCLUDE_DIR}> 
                $<BUILD_INTERFACE:${${PREFIX}_INCLUDE_DIR}>
                $<INSTALL_INTERFACE:include>
  )
  
  if(${ROOT_PROJECT_NAME}_BUILD_JAVATOOLS)
	  target_include_directories(lib${PROJECT_NAME} PRIVATE ${Java_INCLUDE_DIRS})
  endif()

  set(${PREFIX}_LIBS
        Threads::Threads
        Eigen3::Eigen
        CodeSynthesis::XSD
        ${CMAKE_DL_LIBS}
  )
  target_link_libraries(lib${PROJECT_NAME} PUBLIC Biogears::common ${${PREFIX}_LIBS} )
  if(ANDROID)
    target_link_libraries(lib${PUBLIC_NAME} PUBLIC android log)
  endif() 
  add_dependencies(lib${PROJECT_NAME} ${XSD_TARGETS} prepare_runtime_dir)

else()
  ###############################################################################
  # libBiogears
  ###############################################################################
  add_library(lib${PROJECT_NAME} SHARED ${LIB${PREFIX}_CORE_SOURCES} ${LIB${PREFIX}_CORE_HEADERS})
  add_library(Biogears::lib${PROJECT_NAME} ALIAS lib${PROJECT_NAME})
  	set_target_properties(lib${PROJECT_NAME}
    PROPERTIES
    DEFINE_SYMBOL ${PROJECT_NAME}_EXPORTS
    FOLDER "Libraries"
    OUTPUT_NAME "${PROJECT_NAME}"
    COMPILE_PDB_NAME "${PROJECT_NAME}"
    PROJECT_LABEL "lib${PROJECT_NAME}"
    DEBUG_POSTFIX "${CMAKE_DEBUG_POSTFIX}"
    CXX_STANDARD 14
    )
  set_target_properties(lib${PROJECT_NAME} PROPERTIES SOVERSION "${${ROOT_PROJECT_NAME}_LIB_VERSION}")
  if(UNITTEST_biogears-core)
     target_compile_definitions( lib${PROJECT_NAME} PUBLIC ENABLE_UNIT_TEST_API)
  endif()
  ###############################################################################
  # libBiogears COMPILATION & LINKAGE MODIFICATIONS
  ###############################################################################
  set(${PREFIX}_CPPFLAGS $<$<PLATFORM_ID:Windows>:_SCL_SECURE_NO_WARNINGS _CRT_SECURE_NO_WARNINGS NOMINMAX >)
  target_compile_definitions(lib${PROJECT_NAME}  PUBLIC ${${PREFIX}_CPPFLAGS_INTERFACE})
  target_compile_definitions(lib${PROJECT_NAME}  PRIVATE ${${PREFIX}_CPPFLAGS} )
  target_compile_options(lib${PROJECT_NAME} PRIVATE $<$<PLATFORM_ID:Windows>:/bigobj> PRIVATE $<$<PLATFORM_ID:Windows>:/MP${${ROOT_PROJECT_NAME}_BUILD_PARALLEL_LEVEL}>)
  if(${ROOT_PROJECT_NAME}_THROW_READONLY_EXCEPTIONS)
      target_compile_definitions(lib${PROJECT_NAME} PUBLIC BIOGEARS_THROW_READONLY_EXCEPTIONS)
  endif()
  if(${ROOT_PROJECT_NAME}_THROW_NAN_EXCEPTIONS )
       target_compile_definitions(lib${PROJECT_NAME} PUBLIC BIOGEARS_THROW_NAN_EXCEPTIONS)
  endif()
  if(${ROOT_PROJECT_NAME}_USE_OVERRIDE_CONTROL)
       target_compile_definitions( lib${PROJECT_NAME}  PRIVATE BIOGEARS_USE_OVERRIDE_CONTROL)
  endif()

  target_include_directories(lib${PROJECT_NAME} 
      PRIVATE  ${${PREFIX}_INCLUDE_DIR} ${${PREFIX}_PRIVATE_INCLUDE_DIR} ${${PREFIX}_GENERATED_INCLUDE_DIR} 
      INTERFACE 
                $<BUILD_INTERFACE:${${PREFIX}_INCLUDE_DIR}>
		$<BUILD_INTERFACE:${${PREFIX}_GENERATED_INCLUDE_DIR}>
                $<INSTALL_INTERFACE:include>
  )  
  set(${PREFIX}_LIBS
            Eigen3::Eigen
            Biogears::lib${PROJECT_NAME}_common
            Biogears::lib${PROJECT_NAME}_cdm
  )

  target_link_libraries(lib${PROJECT_NAME}  PUBLIC ${${PREFIX}_LIBS} )
  if(ANDROID)
    target_link_libraries(lib${PROJECT_NAME} PUBLIC android log)
  endif() 
  if(TARGET Biogears::lib${PROJECT_NAME}_io)
    target_link_libraries(lib${PROJECT_NAME}  PUBLIC Biogears::lib${PROJECT_NAME}_io )
  endif()
  if(WIN32)
    install(FILES $<TARGET_PDB_FILE:lib${PROJECT_NAME}> DESTINATION lib/${${ROOT_PROJECT_NAME}_PLATFORM_PREFIX}$<$<BOOL:${${ROOT_PROJECT_NAME}_BREAKOUT_INSTALL}>:/$<CONFIG>> OPTIONAL)
    # lib//biogears.lib               #When Biogears_BREAKOUT_INSTALL == FAlSE AND Biogears_PLATFORM_PREFIX = ""
    # lib//biogears_d.lib
    # lib//Release/libbiogears.lib    #When Biogears_BREAKOUT_INSTALL == TRUE AND Biogears_PLATFORM_PREFIX = ""
    # lib//Debug/libbiogears_d.lib
    # lib/Win64/Debug/libbiogears_d.lib  #When Biogears_BREAKOUT_INSTALL == FAlSE AND Biogears_PLATFORM_PREFIX = "Win64"
  endif()
endif() #NOT ${PREFIX}_BUILD_STATIC

######################################################################################################
# INSTALL LOGIC
######################################################################################################
install(TARGETS lib${PROJECT_NAME} EXPORT libBiogears
     RUNTIME DESTINATION bin/${${ROOT_PROJECT_NAME}_PLATFORM_PREFIX}$<$<BOOL:${${ROOT_PROJECT_NAME}_BREAKOUT_INSTALL}>:/$<CONFIG>>
     LIBRARY DESTINATION ${LIBRARY_INSTALL_DIR}/${${ROOT_PROJECT_NAME}_PLATFORM_PREFIX}$<$<BOOL:${${ROOT_PROJECT_NAME}_BREAKOUT_INSTALL}>:/$<CONFIG>>
     ARCHIVE DESTINATION lib/${${ROOT_PROJECT_NAME}_PLATFORM_PREFIX}$<$<BOOL:${${ROOT_PROJECT_NAME}_BREAKOUT_INSTALL}>:/$<CONFIG>>
)

if(WIN32)
  install(FILES $<TARGET_PDB_FILE:lib${PROJECT_NAME}> DESTINATION lib/${${ROOT_PROJECT_NAME}_PLATFORM_PREFIX}$<$<BOOL:${${ROOT_PROJECT_NAME}_BREAKOUT_INSTALL}>:/$<CONFIG>> OPTIONAL)
endif()

 
install(DIRECTORY 
    include/biogears
    ${${PREFIX}_GENERATED_INCLUDE_DIR}/biogears
    DESTINATION include/
    FILE_PERMISSIONS OWNER_READ GROUP_READ WORLD_READ
    FILES_MATCHING 
    PATTERN "*.h"
    PATTERN "*.hxx"
    PATTERN "*.inl"
)
