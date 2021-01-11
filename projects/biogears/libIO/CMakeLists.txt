message(STATUS "  Configuring lib${PROJECT_NAME}_io")
###############################################################################
#Sorce and Header Defines
###############################################################################

###############################################################################
#Define Logic
###############################################################################
FILE(GLOB_RECURSE XSD_FILES              RELATIVE "${PROJECT_SOURCE_DIR}/share/" LIST_DIRECTORIES false CONFIGURE_DEPENDS "${PROJECT_SOURCE_DIR}/share/xsd/*.xsd" )
FILE(GLOB_RECURSE DATA_CONFIG_FILES      RELATIVE "${PROJECT_SOURCE_DIR}/share/" LIST_DIRECTORIES false CONFIGURE_DEPENDS "${PROJECT_SOURCE_DIR}/share/data/config/*.xml")
FILE(GLOB_RECURSE DATA_ECG_FILES         RELATIVE "${PROJECT_SOURCE_DIR}/share/" LIST_DIRECTORIES false CONFIGURE_DEPENDS "${PROJECT_SOURCE_DIR}/share/data/ecg/*.xml")
FILE(GLOB_RECURSE DATA_ENIRONMENT_FILES  RELATIVE "${PROJECT_SOURCE_DIR}/share/" LIST_DIRECTORIES false CONFIGURE_DEPENDS "${PROJECT_SOURCE_DIR}/share/data/environments/*.xml")
FILE(GLOB_RECURSE DATA_NUTRITION_FILES   RELATIVE "${PROJECT_SOURCE_DIR}/share/" LIST_DIRECTORIES false CONFIGURE_DEPENDS "${PROJECT_SOURCE_DIR}/share/data/nutrition/*.xml")
FILE(GLOB_RECURSE DATA_OVERRIDE_FILES    RELATIVE "${PROJECT_SOURCE_DIR}/share/" LIST_DIRECTORIES false CONFIGURE_DEPENDS "${PROJECT_SOURCE_DIR}/share/data/override/*.xml")
FILE(GLOB_RECURSE DATA_PATIENT_FILES     RELATIVE "${PROJECT_SOURCE_DIR}/share/" LIST_DIRECTORIES false CONFIGURE_DEPENDS "${PROJECT_SOURCE_DIR}/share/data/patients/*.xml")
FILE(GLOB_RECURSE DATA_STATE_FILES       RELATIVE "${PROJECT_SOURCE_DIR}/share/" LIST_DIRECTORIES false CONFIGURE_DEPENDS "${PROJECT_SOURCE_DIR}/share/data/states/*.xml")
FILE(GLOB_RECURSE DATA_SUBSTANCE_FILES   RELATIVE "${PROJECT_SOURCE_DIR}/share/" LIST_DIRECTORIES false CONFIGURE_DEPENDS "${PROJECT_SOURCE_DIR}/share/data/substances/*.xml")

LIST(APPEND IO_SOURCE_FILES ${XSD_FILES} ${DATA_CONFIG_FILES} ${DATA_ECG_FILES} ${DATA_ENVIORNMENT_FILES} ${DATA_NUTRITION_FILES} ${DATA_OVERRIDE_FILES} ${DATA_PATIENT_FILES}
                            ${DATA_STATE_FILES} ${DATA_SUBSTANCE_FILES})

function(generate_hex_header)

  set(options )
  set(oneValueArgs INPUT OUTPUT FOLDER)
  set(multiValueArgs NAMESPACE)

  cmake_parse_arguments("" "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN} )
  string (REPLACE ";" "\\;" _NS_SAFE "${_NAMESPACE}")

  set(_header ${_OUTPUT}.hxx)
  set(_source ${_OUTPUT}.cxx)


  if (_INPUT AND _OUTPUT)
    if( NOT EXISTS ${_header} OR NOT EXISTS ${_source} )
      get_filename_component(_source_name ${_OUTPUT} NAME)
      get_filename_component(_source_directory ${_OUTPUT} DIRECTORY )
      
      message(STATUS "\tGenerating ${_source_name}.{hxx,cxx}")
      execute_process( WORKING_DIRECTORY ${BIOGEARS_BINARY_DIR}/
      COMMAND ${CMAKE_COMMAND} -DIN=${_INPUT} -DOUT=${_OUTPUT} -DNS=${_NS_SAFE}  -P ${PROJECT_SOURCE_DIR}/cmake/common/c_encode_file.cmake
                        ERROR_VARIABLE CMAKE_ERROR
                        #OUTPUT_QUIET
                      )
      if(CMAKE_ERROR)
        message(FATAL_ERROR "Failed to generate c_encoding\n"
                            "Error Msg:\n"
                            "${CMAKE_ERROR}\n"
                )
      endif(CMAKE_ERROR)
    endif()
    add_custom_command(
      OUTPUT  "${_OUTPUT}.hxx" "${_OUTPUT}.cxx"
      DEPENDS ${_INPUT} ${PROJECT_SOURCE_DIR}/cmake/common/c_encode_file.cmake
      COMMAND ${CMAKE_COMMAND} -DIN="${_INPUT}" -DOUT="${_OUTPUT}" -DNS="${_NS_SAFE}"  -P "${PROJECT_SOURCE_DIR}/cmake/common/c_encode_file.cmake"
    )
    get_filename_component( content_name ${_INPUT} NAME_WE)

    string (REPLACE "@" "_at_" content_name "${content_name}")
  
    add_custom_target( generate_${content_name}   DEPENDS ${_OUTPUT})
    set_target_properties(generate_${content_name} PROPERTIES 
      FOLDER ${_FOLDER}
    )
  else()    
    message(FATAL_ERROR "generate_hex_header requires arguments INPUT <file> and OUTPUT <file> ")
  endif()

endfunction(generate_hex_header)

set ( IO_VERSION "${${ROOT_PROJECT_NAME}_LIB_VERSION}")
set ( IO_INCLUDE "c_encoded/${IO_VERSION}")

message(STATUS "  Iterating over source files to be encoded as C strings. This is a long operation, but only on the initial pass or when the source file changes.")

foreach(src IN LISTS IO_SOURCE_FILES)
  get_filename_component(src_dir ${src} DIRECTORY)
  generate_hex_header( NAMESPACE biogears io INPUT ${PROJECT_SOURCE_DIR}/share/${src} OUTPUT ${BIOGEARS_BINARY_DIR}/generated/${IO_INCLUDE}/biogears/io/${src}
                       FOLDER "Code Generators/IO/${src_dir}"
  )
endforeach()

add_source_files(GEN_HDRS LOCATION ${BIOGEARS_BINARY_DIR}/generated/${IO_INCLUDE}/biogears/io/
               REGEX "*.hxx" SOURCE_GROUP  "Headers\\Generated\\${IO_VERSION}\\")
add_source_files(GEN_SRCS LOCATION ${BIOGEARS_BINARY_DIR}/generated/${IO_INCLUDE}/biogears/io/
               REGEX "*.cxx" SOURCE_GROUP  "Sources\\Generated\\${IO_VERSION}\\")

###############################################################################
# libBiogears_io
###############################################################################
add_library(lib${PROJECT_NAME}_io  SHARED ${GEN_HDRS} ${GEN_SRCS})
add_library(Biogears::lib${PROJECT_NAME}_io ALIAS lib${PROJECT_NAME}_io)
set_target_properties(lib${PROJECT_NAME}_io
  PROPERTIES
  DEFINE_SYMBOL ${PROJECT_NAME}_io_EXPORTS
  FOLDER "Libraries"
  OUTPUT_NAME "${PROJECT_NAME}_io"
  COMPILE_PDB_NAME "${PROJECT_NAME}_io"
  PROJECT_LABEL "lib${PROJECT_NAME}_io"
  DEBUG_POSTFIX ${CMAKE_DEBUG_POSTFIX}
  CXX_STANDARD 14
  )
set_target_properties(lib${PROJECT_NAME}_io PROPERTIES SOVERSION "${${ROOT_PROJECT_NAME}_LIB_VERSION}")
add_dependencies(lib${PROJECT_NAME}_io ${XSD_TARGETS})

###############################################################################
# libBiogears_io COMPILATION & LINKAGE MODIFICATIONS
###############################################################################
set( ${PREFIX}_CPPFLAGS $<$<PLATFORM_ID:Windows>:_SCL_SECURE_NO_WARNINGS _CRT_SECURE_NO_WARNINGS NOMINMAX > )
target_compile_definitions(lib${PROJECT_NAME}_io  PUBLIC ${${PREFIX}_CPPFLAGS_INTERFACE} $<$<STREQUAL:${CMAKE_SYSTEM_PROCESSOR},armv7-a>:XSD_NO_EXPORT>)
target_compile_definitions(lib${PROJECT_NAME}_io  PRIVATE ${${PREFIX}_CPPFLAGS} )
target_compile_options(lib${PROJECT_NAME}_io PRIVATE $<$<PLATFORM_ID:Windows>:/bigobj>  PRIVATE $<$<PLATFORM_ID:Windows>:/MP${${ROOT_PROJECT_NAME}_BUILD_PARALLEL_LEVEL}>)

target_include_directories(lib${PROJECT_NAME}_io 
    PRIVATE  ${${PREFIX}_INCLUDE_DIR} ${${PREFIX}_PRIVATE_INCLUDE_DIR} ${${PREFIX}_GENERATED_INCLUDE_DIR} ${BIOGEARS_SOURCE_DIR}/libIO/include
    INTERFACE 
              $<BUILD_INTERFACE:${BIOGEARS_SOURCE_DIR}/libIO/include>
              $<INSTALL_INTERFACE:include>
)

set(${PREFIX}_io_LIBS
          CodeSynthesis::XSD
          Eigen3::Eigen
)
target_link_libraries(lib${PROJECT_NAME}_io  PUBLIC ${${PREFIX}_io_LIBS} )

install(TARGETS lib${PROJECT_NAME}_io  EXPORT libBiogears
    RUNTIME DESTINATION bin/${${ROOT_PROJECT_NAME}_PLATFORM_PREFIX}$<$<BOOL:${${ROOT_PROJECT_NAME}_BREAKOUT_INSTALL}>:/$<CONFIG>>
    LIBRARY DESTINATION ${LIBRARY_INSTALL_DIR}/${${ROOT_PROJECT_NAME}_PLATFORM_PREFIX}$<$<BOOL:${${ROOT_PROJECT_NAME}_BREAKOUT_INSTALL}>:/$<CONFIG>>
    ARCHIVE DESTINATION lib/${${ROOT_PROJECT_NAME}_PLATFORM_PREFIX}$<$<BOOL:${${ROOT_PROJECT_NAME}_BREAKOUT_INSTALL}>:/$<CONFIG>>
    )

if(WIN32)
install(FILES $<TARGET_PDB_FILE:lib${PROJECT_NAME}_io> DESTINATION lib/${${ROOT_PROJECT_NAME}_PLATFORM_PREFIX}$<$<BOOL:${${ROOT_PROJECT_NAME}_BREAKOUT_INSTALL}>:/$<CONFIG>> OPTIONAL)
endif()
