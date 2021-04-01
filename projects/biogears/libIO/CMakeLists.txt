cmake_minimum_required(VERSION 3.19.0)
message(STATUS "  Configuring lib${PROJECT_NAME}_io")
###############################################################################
#Source files
###############################################################################
function(generate_hex_header)
  set(options )
  set(oneValueArgs INPUT OUTPUT FOLDER PATH)
  set(multiValueArgs NAMESPACE)

  cmake_parse_arguments("" "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN} )
  string (REPLACE ";" "\\;" _NS_SAFE "${_NAMESPACE}")

  set(_header ${_OUTPUT}.hxx)
  set(_source ${_OUTPUT}.cxx)


  if (_INPUT AND _OUTPUT)
    
    get_filename_component( content_name ${_INPUT} NAME_WE)
    string (REPLACE "@" "_at_" content_name "${content_name}")

    if( NOT EXISTS ${_header} OR NOT EXISTS ${_source} )
      get_filename_component(_source_name ${_OUTPUT} NAME)
      get_filename_component(_source_directory ${_OUTPUT} DIRECTORY )

      message(STATUS  "                   ${_source_name}.{hxx,cxx}")
      file(WRITE ${_header} "//! THIS IS PLACEHOLDER DATA THAT WILL BE REPLACED BY THE FIRST RUN OF TARGET generate_${content_name}")
      file(WRITE ${_source} "//! THIS IS PLACEHOLDER DATA THAT WILL BE REPLACED BY THE FIRST RUN OF TARGET generate_${content_name}")
    endif()

    add_custom_command(
      OUTPUT  "${_OUTPUT}.hxx" "${_OUTPUT}.cxx"
      DEPENDS ${_INPUT} ${PROJECT_SOURCE_DIR}/cmake/common/c_encode_file.cmake
      COMMAND ${CMAKE_COMMAND} -DIN="${_INPUT}" -DOUT="${_OUTPUT}" -DNS="${_NS_SAFE}"  -DPATH="${_PATH}" -P "${PROJECT_SOURCE_DIR}/cmake/common/c_encode_file.cmake"
    )
  

    add_custom_target( generate_${content_name}
      DEPENDS  "${_OUTPUT}.hxx" "${_OUTPUT}.cxx"
    )
    set_target_properties(generate_${content_name} PROPERTIES 
      FOLDER ${_FOLDER}
    )
  else()    
    message(FATAL_ERROR "generate_hex_header requires arguments INPUT <file> and OUTPUT <file> ")
  endif()
endfunction(generate_hex_header)

function(configure_io_directory_file )

  set(options )
  set(oneValueArgs OUTPUT RESOURCE_PATH HEADERS SOURCES)
  set(multiValueArgs FILE_LIST)

  cmake_parse_arguments("" "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN} )

  foreach( src IN LISTS _FILE_LIST ) 
    get_filename_component(src_dir ${src} DIRECTORY)
    get_filename_component(src_dir_wle ${src} NAME_WLE  )
    string(REGEX REPLACE "@" "_at_" safe_src_dir_wle ${src_dir_wle})
    generate_hex_header( NAMESPACE biogears io INPUT ${_RESOURCE_PATH}/${src} OUTPUT ${BIOGEARS_BINARY_DIR}/generated/${IO_INCLUDE}/biogears/io/${src}
                         FOLDER "Code Generators/IO/${src_dir}"  PATH ${src}
    )
  
    LIST(APPEND ${_HEADERS}  "${BIOGEARS_BINARY_DIR}/generated/${IO_INCLUDE}/biogears/io/${src}.hxx")
    LIST(APPEND ${_SOURCES}  "${BIOGEARS_BINARY_DIR}/generated/${IO_INCLUDE}/biogears/io/${src}.cxx")

    source_group("Headers/Generated/${IO_VERSION}/${src_dir}" FILES "${BIOGEARS_BINARY_DIR}/generated/${IO_INCLUDE}/biogears/io/${src}.hxx" )
    source_group("Sources/Generated/${IO_VERSION}/${src_dir}" FILES "${BIOGEARS_BINARY_DIR}/generated/${IO_INCLUDE}/biogears/io/${src}.cxx" )

    string(APPEND RESOURCE_FILE_LIST "    get_${safe_src_dir_wle}_path(),\n")
     
    string(APPEND RESOURCE_INCLUDE_LIST "#include <biogears/io/${src}.hxx>\n")
    string(APPEND GENERATE_DIRECTORY_BODY  "    result &= write_file(get_${safe_src_dir_wle}_path(), get_${safe_src_dir_wle}_text(), ${safe_src_dir_wle}_text_size());\n")
    string(APPEND DOES_EMBEDDED_FILE_EXIST_BODY "  ( 0 == std::strcmp(path, get_${safe_src_dir_wle}_path()))\n    ||  ")
    string(APPEND DOES_EMBEDDED_FILE_EXIST_PATH_BODY "  ( path == get_${safe_src_dir_wle}_path())\n    ||  ")
    string(APPEND GET_FILE_SHA1_BODY
    "    if ( 0 == std::strcmp(path, get_${safe_src_dir_wle}_path())) {\n"
    "      return get_${safe_src_dir_wle}_sha1();\n"
    "    }\n"
    )
    string(APPEND GET_FILE_SHA1_PATH_BODY
    "    if ( path == get_${safe_src_dir_wle}_path()) {\n"
    "      return get_${safe_src_dir_wle}_sha1();\n"
    "    }\n"
    )
    string(APPEND GET_EMBEDED_CONTENTS_BODY
    "    if ( 0 == std::strcmp(path, get_${safe_src_dir_wle}_path())) {\n"
    "      return get_${safe_src_dir_wle}_text();\n"
    "    }\n"
    )
    string(APPEND GET_EMBEDED_CONTENTS_PATH_BODY
    "    if ( path == get_${safe_src_dir_wle}_path()) {\n"
    "      return get_${safe_src_dir_wle}_text();\n"
    "    }\n"
    )
    string(APPEND GET_EMBEDED_CONTENTS_SIZE_BODY
    "    if ( 0 == std::strcmp(path, get_${safe_src_dir_wle}_path())) {\n"
    "      return ${safe_src_dir_wle}_text_size();\n"
    "    }\n"
    )
    string(APPEND GET_EMBEDED_CONTENTS_SIZE_PATH_BODY
    "    if ( path == get_${safe_src_dir_wle}_path()) {\n"
    "      return ${safe_src_dir_wle}_text_size();\n"
    "    }\n"
    )
  endforeach()

  LIST(LENGTH _FILE_LIST RESOURCE_FILE_COUNT)
  get_filename_component( base_name ${_OUTPUT} NAME_WE)
  get_filename_component( base_directory ${_OUTPUT} DIRECTORY)

  configure_file( "src/${_OUTPUT}" ${BIOGEARS_BINARY_DIR}/generated/biogears/io/${base_directory}/${base_name}.cpp @ONLY NEWLINE_STYLE LF)

  add_source_files(${_SOURCES} LOCATION ${BIOGEARS_BINARY_DIR}/generated/biogears/io/${base_directory}
               REGEX "${base_name}.cpp" SOURCE_GROUP  "Sources\\")
    
  LIST(APPEND ${_SOURCES}  "${BIOGEARS_BINARY_DIR}/generated/biogears/io/${base_directory}/${base_name}.cpp")
  source_group("Sources/${base_directory}/${base_name}.cpp" FILES ${BIOGEARS_BINARY_DIR}/generated/biogears/io/${base_directory}/${base_name}.cpp )

  set(${_HEADERS} ${${_SOURCES}} PARENT_SCOPE)
  set(${_SOURCES} ${${_SOURCES}} PARENT_SCOPE)

endfunction(configure_io_directory_file)


set ( IO_VERSION "${${ROOT_PROJECT_NAME}_LIB_VERSION}")
set ( IO_INCLUDE "c_encoded/${IO_VERSION}")

message(STATUS "  Iterating over source files to be encoded as C strings. This is a long operation, but only on the initial pass or when the source file changes.")
message(STATUS "  Encoding: runtime/")

message(STATUS "            : xsd/")
FILE(GLOB_RECURSE XSD_FILES RELATIVE "${PROJECT_SOURCE_DIR}/share/" LIST_DIRECTORIES false CONFIGURE_DEPENDS "${PROJECT_SOURCE_DIR}/share/xsd/*.xsd" )
configure_io_directory_file( OUTPUT "directories/xsd.cpp.in" FILE_LIST ${XSD_FILES}  RESOURCE_PATH ${PROJECT_SOURCE_DIR}/share/ HEADERS GEN_HDRS SOURCES GEN_SRCS)

message(STATUS "            : data/config")
FILE(GLOB_RECURSE DATA_CONFIG_FILES      RELATIVE "${PROJECT_SOURCE_DIR}/share/data/" LIST_DIRECTORIES false CONFIGURE_DEPENDS "${PROJECT_SOURCE_DIR}/share/data/config/*.xml")
configure_io_directory_file( OUTPUT "directories/config.cpp.in" FILE_LIST ${DATA_CONFIG_FILES} RESOURCE_PATH ${PROJECT_SOURCE_DIR}/share/data HEADERS GEN_HDRS SOURCES GEN_SRCS)

message(STATUS "            : data/ecg")
FILE(GLOB_RECURSE DATA_ECG_FILES         RELATIVE "${PROJECT_SOURCE_DIR}/share/data/" LIST_DIRECTORIES false CONFIGURE_DEPENDS "${PROJECT_SOURCE_DIR}/share/data/ecg/*.xml")
configure_io_directory_file( OUTPUT "directories/ecg.cpp.in" FILE_LIST ${DATA_ECG_FILES} RESOURCE_PATH ${PROJECT_SOURCE_DIR}/share/data HEADERS GEN_HDRS SOURCES GEN_SRCS)

message(STATUS "            : data/environments")
FILE(GLOB_RECURSE DATA_ENIRONMENT_FILES  RELATIVE "${PROJECT_SOURCE_DIR}/share/data/" LIST_DIRECTORIES false CONFIGURE_DEPENDS "${PROJECT_SOURCE_DIR}/share/data/environments/*.xml")
configure_io_directory_file( OUTPUT "directories/environments.cpp.in" FILE_LIST ${DATA_ENIRONMENT_FILES} RESOURCE_PATH ${PROJECT_SOURCE_DIR}/share/data HEADERS GEN_HDRS SOURCES GEN_SRCS)

message(STATUS "            : data/nutrition")
FILE(GLOB_RECURSE DATA_NUTRITION_FILES   RELATIVE "${PROJECT_SOURCE_DIR}/share/data/" LIST_DIRECTORIES false CONFIGURE_DEPENDS "${PROJECT_SOURCE_DIR}/share/data/nutrition/*.xml")
configure_io_directory_file( OUTPUT "directories/nutrition.cpp.in" FILE_LIST ${DATA_NUTRITION_FILES} RESOURCE_PATH ${PROJECT_SOURCE_DIR}/share/data HEADERS GEN_HDRS SOURCES GEN_SRCS)

message(STATUS "            : data/override")
FILE(GLOB_RECURSE DATA_OVERRIDE_FILES    RELATIVE "${PROJECT_SOURCE_DIR}/share/data/" LIST_DIRECTORIES false CONFIGURE_DEPENDS "${PROJECT_SOURCE_DIR}/share/data/override/*.xml")
configure_io_directory_file( OUTPUT "directories/override.cpp.in" FILE_LIST ${DATA_OVERRIDE_FILES} RESOURCE_PATH ${PROJECT_SOURCE_DIR}/share/data HEADERS GEN_HDRS SOURCES GEN_SRCS)

message(STATUS "            : data/patients")
FILE(GLOB_RECURSE DATA_PATIENT_FILES     RELATIVE "${PROJECT_SOURCE_DIR}/share/data/" LIST_DIRECTORIES false CONFIGURE_DEPENDS "${PROJECT_SOURCE_DIR}/share/data/patients/*.xml")
configure_io_directory_file( OUTPUT "directories/patients.cpp.in" FILE_LIST ${DATA_PATIENT_FILES} RESOURCE_PATH ${PROJECT_SOURCE_DIR}/share/data HEADERS GEN_HDRS SOURCES GEN_SRCS)

option(Biogears_IO_INCLUDE_STATES "Add default states which can be generated using bg-cli to the IO library. This greatly increases the size of the io library" OFF)
if (Biogears_IO_INCLUDE_STATES)
message(STATUS "            : data/states")
FILE(GLOB_RECURSE DATA_STATE_FILES       RELATIVE "${PROJECT_SOURCE_DIR}/share/data/" LIST_DIRECTORIES false CONFIGURE_DEPENDS "${PROJECT_SOURCE_DIR}/share/data/states/*.xml")
configure_io_directory_file( OUTPUT "directories/states.cpp.in" FILE_LIST ${DATA_STATE_FILES} RESOURCE_PATH ${PROJECT_SOURCE_DIR}/share/data HEADERS GEN_HDRS SOURCES GEN_SRCS)
endif()
message(STATUS "            : data/substances")
FILE(GLOB_RECURSE DATA_SUBSTANCE_FILES   RELATIVE "${PROJECT_SOURCE_DIR}/share/data/" LIST_DIRECTORIES false CONFIGURE_DEPENDS "${PROJECT_SOURCE_DIR}/share/data/substances/*.xml")
configure_io_directory_file( OUTPUT "directories/substances.cpp.in" FILE_LIST ${DATA_SUBSTANCE_FILES} RESOURCE_PATH ${PROJECT_SOURCE_DIR}/share/data HEADERS GEN_HDRS SOURCES GEN_SRCS )

add_source_files(INTERFACE_HEADERS LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/include/
               REGEX "*.h" SOURCE_GROUP  "Headers\\Public\\")
add_source_files(INTERFACE_SOURCES LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/src/
               REGEX "*.cpp" SOURCE_GROUP  "Sources\\")


###############################################################################
# libBiogears_io
###############################################################################
add_library(lib${PROJECT_NAME}_io  SHARED ${GEN_HDRS} ${GEN_SRCS} ${INTERFACE_HEADERS} ${INTERFACE_SOURCES})
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
target_compile_definitions(lib${PROJECT_NAME}_io  PRIVATE ${${PREFIX}_CPPFLAGS} PRIVATE)
target_compile_options(lib${PROJECT_NAME}_io PRIVATE $<$<PLATFORM_ID:Windows>:/bigobj>  PRIVATE $<$<PLATFORM_ID:Windows>:/MP${${ROOT_PROJECT_NAME}_BUILD_PARALLEL_LEVEL}>)
if(Biogears_IO_INCLUDE_STATES)
  target_compile_definitions(lib${PROJECT_NAME}_io PRIVATE IO_EMBED_STATES)
endif()
target_include_directories(lib${PROJECT_NAME}_io 
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include
            ${BIOGEARS_BINARY_DIR}/generated
            ${BIOGEARS_BINARY_DIR}/generated/${IO_INCLUDE}
    
    INTERFACE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    INTERFACE $<BUILD_INTERFACE:${BIOGEARS_BINARY_DIR}/generated>
    INTERFACE $<INSTALL_INTERFACE:include>
)

set(${PREFIX}_io_LIBS
          Biogears::libbiogears_common
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
