#############################################################################
#Sorce and Header Defines
###############################################################################
message(STATUS "  Configuring libbiogears")

configure_file( ${CMAKE_CURRENT_SOURCE_DIR}/include/biogears/config.h.in  ${CMAKE_CURRENT_BINARY_DIR}/../generated/biogears/config.h NEWLINE_STYLE LF)

#Biogears CDM
add_source_files(CDM_PUBLIC_HDRS LOCATION ${BIOGEARS_SOURCE_DIR}/libCDM/include/ REGEX "*.h" SOURCE_GROUP "Headers\\Public\\schema\\")
add_source_files(CDM_GEN_HDRS LOCATION ${BIOGEARS_BINARY_DIR}/generated/biogears/schema REGEX "*.hxx" SOURCE_GROUP "Headers\\Public\\schema\\")
add_source_files(CDM_GEN_SRCS LOCATION  ${BIOGEARS_BINARY_DIR}/generated/biogears/schema REGEX "*.cxx" SOURCE_GROUP "Sources\\schema\\")

#Biogears IO
add_source_files(IO_HDRS LOCATION ${BIOGEARS_SOURCE_DIR}/libIO/include REGEX "*.h" SOURCE_GROUP "Headers\\Public\\")
add_source_files(IO_GEN_HDRS LOCATION ${BIOGEARS_BINARY_DIR}/generated/biogears/io REGEX "*.h" SOURCE_GROUP "Headers\\Public\\generated\\")
add_source_files(IO_GEN_SRCS LOCATION  ${BIOGEARS_BINARY_DIR}/generated/biogears/io REGEX "*.cpp" SOURCE_GROUP "Sources\\generated\\")
add_source_files(IO_EMBEDED_HDRS LOCATION "${BIOGEARS_BINARY_DIR}/generated/c_encoded/${Biogears_LIB_VERSION}" REGEX "*.hxx" SOURCE_GROUP "Headers\\Public\\generated\\")
add_source_files(IO_EMBEDED_SRCS LOCATION  "${BIOGEARS_BINARY_DIR}/generated/c_encoded/${Biogears_LIB_VERSION}/" REGEX "*.cxx" SOURCE_GROUP "Sources\\generated\\")

#Biogears Core
add_source_files(CORE_HDRS LOCATION ${BIOGEARS_SOURCE_DIR}/libBiogears/src REGEX "*.h" SOURCE_GROUP "Headers\\Private\\")
add_source_files(CORE_SRCS LOCATION ${BIOGEARS_SOURCE_DIR}/libBiogears/src REGEX "*.cpp" SOURCE_GROUP "Sources\\")
add_source_files(CORE_GEN_SRCS LOCATION ${BIOGEARS_BINARY_DIR}/libBiogears/src REGEX "version.cpp" SOURCE_GROUP"Sources\\")
add_source_files(CORE_PUBLIC_HDRS LOCATION ${BIOGEARS_SOURCE_DIR}/libBiogears/include REGEX "*.h" SOURCE_GROUP "Headers\\Public\\")
add_source_files(CORE_PUBLIC_HDRS LOCATION ${BIOGEARS_SOURCE_DIR}/libBiogears/include REGEX "*.inl" SOURCE_GROUP "Headers\\Public\\")

set(BIO_CDM_HEADERS ${CDM_HDRS} ${CDM_PUBLIC_HDRS} ${CDM_GEN_HDRS})
set(BIO_CDM_SOURCES ${CDM_SRCS} ${CDM_GEN_SRCS})

set(BIO_IO_HEADERS ${IO_HDRS} ${IO_EMBEDED_HDRS} ${IO_GEN_HDRS})
set(BIO_IO_SOURCES ${IO_EMBEDED_SRCS} ${IO_GEN_SRCS})

set(BIO_CORE_HEADERS ${CORE_HDRS} ${CORE_PUBLIC_HDRS} ${CORE_GEN_HDRS})
set(BIO_CORE_SOURCES ${CORE_SRCS} ${CORE_GEN_SRCS})

###############################################################################
#Define Logic
###############################################################################
if(Biogears_BUILD_STATIC)
  add_library(libbiogears STATIC ${BIO_CORE_SOURCES} ${BIO_CORE_HEADERS} 
                                 ${BIO_CDM_HEADERS}  ${BIO_CDM_SOURCES}
                                 ${BIO_IO_HEADERS}   ${BIO_IO_SOURCES}
  )
  add_library(Biogears::libbiogears ALIAS libbiogears)
  set_target_properties(libbiogears
    PROPERTIES
    DEFINE_SYMBOL biogears_EXPORTS
    FOLDER "Libraries"
    OUTPUT_NAME "biogears"
    COMPILE_PDB_NAME "biogears"
    PROJECT_LABEL "libbiogears"
    DEBUG_POSTFIX "${CMAKE_DEBUG_POSTFIX}"
    CXX_STANDARD 14
    )
    add_dependencies(libbiogears hex_bindings)
  set_target_properties(libbiogears PROPERTIES SOVERSION "${Biogears_LIB_VERSION}")
   if(biogears_USE_OVERRIDE_CONTROL)
      target_compile_definitions( libbiogears PRIVATE BIOGEARS_USE_OVERRIDE_CONTROL)
   endif()
  if(UNITTEST_biogears-core)
     target_compile_definitions( libbiogears PUBLIC ENABLE_UNIT_TEST_API)
  endif()
  if(WIN32)
    list(APPEND BIO_CPPFLAGS _SCL_SECURE_NO_WARNINGS _CRT_SECURE_NO_WARNINGS NOMINMAX $ENV{PARALLEL_COMPILE} )
  elseif(CMAKE_COMPILER_IS_GNUCXX)
    list(APPEND BIO_CPPFLAGS "-Wattributes" ${CodeSynthesis_CPPFLAGS})
    list(APPEND BIO_LDFLAGS "-Wl,--no-as-needed" )
    set_target_properties(libbiogears PROPERTIES LINK_FLAGS ${BIO_LDFLAGS})
  endif()
  ###############################################################################
  # COMPILATION & LINKAGE MODIFICATIONS
  ###############################################################################
  target_compile_definitions(libbiogears PUBLIC BIO_BUILT_AS_STATIC ${BIO_CPPFLAGS_INTERFACE} $<$<STREQUAL:${CMAKE_SYSTEM_PROCESSOR},armv7-a>:XSD_NO_EXPORT>)
  target_compile_definitions(libbiogears PRIVATE ${BIO_CPPFLAGS} )
  target_compile_options(libbiogears PRIVATE $<$<PLATFORM_ID:Windows>:/bigobj>  
                                            PRIVATE $<$<PLATFORM_ID:Windows>:/MP${Biogears_BUILD_PARALLEL_LEVEL}>
                                            )
  if(Biogears_THROW_READONLY_EXCEPTIONS)
        target_compile_definitions(libbiogears PUBLIC BIOGEARS_THROW_READONLY_EXCEPTIONS)
  endif()
   if(Biogears_THROW_NAN_EXCEPTIONS )
        target_compile_definitions(libbiogears PUBLIC BIOGEARS_THROW_NAN_EXCEPTIONS )
  endif()
   if(Biogears_BUILD_STATIC)
        target_compile_definitions(libbiogears PUBLIC BIOGEARS_STATIC_BUILD)
  endif()
  target_include_directories(libbiogears
      PRIVATE  ${BIO_INCLUDE_DIR} ${BIO_PRIVATE_INCLUDE_DIR} ${BIO_GENERATED_INCLUDE_DIR}
                ${BIO_CDM_INCLUDE_DIR} # CDM
                ${BIO_CIRCUIT_INCLUDE_DIR} # Circuit
                ${BIO_IO_INCLUDE_DIR} # IO
                ${BIO_IO_GENERATED_INCLUDE_DIR} # IO
                ${BIO_IO_EMBEDED_INCLUDE_DIR} # IO
      INTERFACE $<BUILD_INTERFACE:${BIO_GENERATED_INCLUDE_DIR}> 
                $<BUILD_INTERFACE:${BIO_INCLUDE_DIR}>
                $<BUILD_INTERFACE:${BIO_CDM_INCLUDE_DIR}> # CDM
                $<BUILD_INTERFACE:${BIO_CIRCUIT_INCLUDE_DIR}> # Circuit
                $<BUILD_INTERFACE:${BIO_IO_INCLUDE_DIR}> # IO
                $<BUILD_INTERFACE:${BIO_IO_GENERATED_INCLUDE_DIR}> # IO
                $<BUILD_INTERFACE:${BIO_IO_EMBEDED_INCLUDE_DIR}> # IO
                $<INSTALL_INTERFACE:include>
  )
  
  if(Biogears_BUILD_JAVATOOLS)
	  target_include_directories(libbiogears PRIVATE ${Java_INCLUDE_DIRS})
  endif()

  set(BIO_LIBS
        Threads::Threads
        Eigen3::Eigen
        CodeSynthesis::XSD
        ${CMAKE_DL_LIBS}
        Biogears::libbiogears_common
  )
  target_link_libraries(libbiogears PUBLIC ${BIO_LIBS} )
  if(ANDROID)
    target_link_libraries(lib${PUBLIC_NAME} PUBLIC android log)
  endif() 
  add_dependencies(libbiogears  xsd_bindings prepare_runtime_dir)
else()
  ###############################################################################
  # libBiogears
  ###############################################################################
  add_library(libbiogears SHARED ${BIO_CORE_SOURCES} ${BIO_CORE_HEADERS})
  add_library(Biogears::libbiogears ALIAS libbiogears)
  	set_target_properties(libbiogears
    PROPERTIES
    DEFINE_SYMBOL biogears_EXPORTS
    FOLDER "Libraries"
    OUTPUT_NAME "biogears"
    COMPILE_PDB_NAME "biogears"
    PROJECT_LABEL "libbiogears"
    DEBUG_POSTFIX "${CMAKE_DEBUG_POSTFIX}"
    CXX_STANDARD 14
    )
  set_target_properties(libbiogears PROPERTIES SOVERSION "${Biogears_LIB_VERSION}")
  if(UNITTEST_biogears-core)
     target_compile_definitions( libbiogears PUBLIC ENABLE_UNIT_TEST_API)
  endif()
  ###############################################################################
  # libBiogears COMPILATION & LINKAGE MODIFICATIONS
  ###############################################################################
  set(BIO_CPPFLAGS $<$<PLATFORM_ID:Windows>:_SCL_SECURE_NO_WARNINGS _CRT_SECURE_NO_WARNINGS NOMINMAX >)
  target_compile_definitions(libbiogears  PUBLIC ${BIO_CPPFLAGS_INTERFACE})
  target_compile_definitions(libbiogears  PRIVATE ${BIO_CPPFLAGS} )
  target_compile_options(libbiogears PRIVATE $<$<PLATFORM_ID:Windows>:/bigobj> PRIVATE $<$<PLATFORM_ID:Windows>:/MP${Biogears_BUILD_PARALLEL_LEVEL}>)
  if(Biogears_THROW_READONLY_EXCEPTIONS)
      target_compile_definitions(libbiogears PUBLIC BIOGEARS_THROW_READONLY_EXCEPTIONS)
  endif()
  if(Biogears_THROW_NAN_EXCEPTIONS )
       target_compile_definitions(libbiogears PUBLIC BIOGEARS_THROW_NAN_EXCEPTIONS)
  endif()
  if(Biogears_USE_OVERRIDE_CONTROL)
       target_compile_definitions( libbiogears  PRIVATE BIOGEARS_USE_OVERRIDE_CONTROL)
  endif()

  target_include_directories(libbiogears 
      PRIVATE  ${BIO_INCLUDE_DIR} ${BIO_PRIVATE_INCLUDE_DIR} ${BIO_GENERATED_INCLUDE_DIR} 
      INTERFACE 
                $<BUILD_INTERFACE:${BIO_INCLUDE_DIR}>
		$<BUILD_INTERFACE:${BIO_GENERATED_INCLUDE_DIR}>
                $<INSTALL_INTERFACE:include>
  )  
  set(BIO_LIBS
            Eigen3::Eigen
            Biogears::libbiogears_common
            Biogears::libbiogears_cdm
  )

  target_link_libraries(libbiogears  PUBLIC ${BIO_LIBS} )
  if(ANDROID)
    target_link_libraries(libbiogears PUBLIC android log)
  endif() 
  if(TARGET Biogears::libbiogears_io)
    target_link_libraries(libbiogears  PUBLIC Biogears::libbiogears_io )
  endif()
  if(WIN32)
    if ( NOT Biogears_BUILD_STATIC )
      install(FILES $<TARGET_PDB_FILE:libbiogears> DESTINATION lib/${Biogears_PLATFORM_PREFIX}$<$<BOOL:${Biogears_BREAKOUT_INSTALL}>:/$<CONFIG>> OPTIONAL)
    endif()
  endif()
endif() #NOT BIO_BUILD_STATIC

######################################################################################################
# INSTALL LOGIC
######################################################################################################
install(TARGETS libbiogears EXPORT libBiogears
     RUNTIME DESTINATION bin/${Biogears_PLATFORM_PREFIX}$<$<BOOL:${Biogears_BREAKOUT_INSTALL}>:/$<CONFIG>>
     LIBRARY DESTINATION ${LIBRARY_INSTALL_DIR}/${Biogears_PLATFORM_PREFIX}$<$<BOOL:${Biogears_BREAKOUT_INSTALL}>:/$<CONFIG>>
     ARCHIVE DESTINATION lib/${Biogears_PLATFORM_PREFIX}$<$<BOOL:${Biogears_BREAKOUT_INSTALL}>:/$<CONFIG>>
)

if(WIN32 AND NOT Biogears_BUILD_STATIC)
  install(FILES $<TARGET_PDB_FILE:libbiogears> DESTINATION lib/${Biogears_PLATFORM_PREFIX}$<$<BOOL:${Biogears_BREAKOUT_INSTALL}>:/$<CONFIG>> OPTIONAL)
endif()

 
install(DIRECTORY 
    include/biogears
    ${BIO_GENERATED_INCLUDE_DIR}/biogears
    DESTINATION include/
    FILE_PERMISSIONS OWNER_READ GROUP_READ WORLD_READ
    FILES_MATCHING 
    PATTERN "*.h"
    PATTERN "*.hxx"
    PATTERN "*.inl"
)
