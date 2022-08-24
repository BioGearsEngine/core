cmake_minimum_required(VERSION 3.21.0)
message(STATUS "  Configuring libbiogears_io")

option(UNITTEST_biogears-core_io "Enable libBiogears_io UnitTest "  ON)
option(Biogears_IO_USE_SHORT_TARGET_NAMES "Reduce the size of target names to accomidate windows path limitations "  OFF)
###############################################################################
#Source files
###############################################################################
function(generate_hex_header GENERATED_HDRS GENERATED_SRCS)
  set(options SHORT_NAMES)
  set(oneValueArgs INPUT OUTPUT PREFIX PATH)
  set(multiValueArgs NAMESPACE)

  cmake_parse_arguments("" "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN} )
  string (REPLACE ";" "\\;" _NS_SAFE "${_NAMESPACE}")
  string (REPLACE ";" "\\;" _NS_SAFE "${_NAMESPACE}")

  string(MAKE_C_IDENTIFIER  "${_PREFIX}" _c_safe_prefix)
  string(MAKE_C_IDENTIFIER  "${_TYPE}" _c_safe_type)

  set(_header ${_OUTPUT}.hxx)
  set(_source ${_OUTPUT}.cxx)

  if (_INPUT AND _OUTPUT)
    get_filename_component( content_name ${_INPUT} NAME_WE)
    string (REPLACE "@" "_at_" content_name "${content_name}")
    string(MAKE_C_IDENTIFIER "${content_name}" _c_safe_content_name)
    get_filename_component(_source_name ${_OUTPUT} NAME)
    get_filename_component(_source_directory ${_OUTPUT} DIRECTORY )

     if( NOT EXISTS ${_header} OR NOT EXISTS ${_source} )
       message(STATUS  "                   ${_source_name}.{hxx,cxx}")
     endif()

    add_custom_command(
      OUTPUT  "${_OUTPUT}.hxx" "${_OUTPUT}.cxx"
      DEPENDS ${_INPUT} ${PROJECT_SOURCE_DIR}/cmake/common/c_encode_file.cmake
      COMMAND ${CMAKE_COMMAND} -DIN="${_INPUT}" -DOUT="${_OUTPUT}" -DNS="${_NS_SAFE}"  -DPATH="${_PATH}" -P "${PROJECT_SOURCE_DIR}/cmake/common/c_encode_file.cmake"
    )
  
  else()    
    message(FATAL_ERROR "generate_hex_header requires arguments INPUT <file> and OUTPUT <file> ")
  endif()
endfunction(generate_hex_header)

function(configure_io_directory_file )

  set(options SHORT_NAMES)
  set(oneValueArgs OUTPUT RESOURCE_PATH TYPE HEADERS SOURCES)
  set(multiValueArgs FILE_LIST)

  cmake_parse_arguments("" "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN} )

  foreach( src IN LISTS _FILE_LIST ) 
    #We going to have to add a runtime directory.in to get rid of this.
    if(src STREQUAL "BioGearsConfiguration.xml")
      set(_true_source "${src}")
    else()
      set(_true_source "${_TYPE}${src}")
    endif()
    get_filename_component(src_dir "${_true_source}" DIRECTORY)
    get_filename_component(src_dir_wle ${src} NAME_WLE  )
    string(REGEX REPLACE "@" "_at_" safe_src_dir_wle ${src_dir_wle})
    string(MAKE_C_IDENTIFIER ${safe_src_dir_wle} safe_src_dir_wle)
    if(Biogears_IO_USE_SHORT_TARGET_NAMES)
      set(ADDITIONAL_ARGS SHORT_NAMES)
    endif()
    
    generate_hex_header( NAMESPACE biogears io INPUT ${_RESOURCE_PATH}/${_true_source} OUTPUT ${BIOGEARS_BINARY_DIR}/generated/${IO_INCLUDE}/biogears/io/${_true_source}
                         PATH ${src} PREFIX ${src_dir} ${ADDITIONAL_ARGS})
   
    
    LIST(APPEND ${_HEADERS}  "${BIOGEARS_BINARY_DIR}/generated/${IO_INCLUDE}/biogears/io/${_true_source}.hxx")
    LIST(APPEND ${_SOURCES}  "${BIOGEARS_BINARY_DIR}/generated/${IO_INCLUDE}/biogears/io/${_true_source}.cxx")

    source_group("Headers/Generated/${IO_VERSION}/${src_dir}" FILES "${BIOGEARS_BINARY_DIR}/generated/${IO_INCLUDE}/biogears/io/${_true_source}.hxx" )
    source_group("Sources/Generated/${IO_VERSION}/${src_dir}" FILES "${BIOGEARS_BINARY_DIR}/generated/${IO_INCLUDE}/biogears/io/${_true_source}.cxx" )

  if(src STREQUAL "BioGearsConfiguration.xml")
    string(APPEND RESOURCE_INCLUDE_LIST "#include <biogears/io/${src}.hxx>\n")
  else()
  string(APPEND RESOURCE_INCLUDE_LIST "#include <biogears/io/${_true_source}.hxx>\n")
  endif()

    string(APPEND RESOURCE_FILE_LIST "    get_${safe_src_dir_wle}_path(),\n")

    if(src STREQUAL "BioGearsConfiguration.xml")
      string(APPEND GENERATE_DIRECTORY_BODY  "    result &= write_file(get_${safe_src_dir_wle}_path(), get_${safe_src_dir_wle}_text(), ${safe_src_dir_wle}_text_size());\n")
    else()
      string(APPEND GENERATE_DIRECTORY_BODY  "    result &= write_file(dir/get_${safe_src_dir_wle}_path(), get_${safe_src_dir_wle}_text(), ${safe_src_dir_wle}_text_size());\n")
    endif()

    string(APPEND DOES_EMBEDDED_FILE_EXIST_PATH_BODY "  ( path == get_${safe_src_dir_wle}_path())\n    ||  ")
    string(APPEND GET_FILE_SHA1_PATH_BODY
    "    if ( path == get_${safe_src_dir_wle}_path()) {\n"
    "      return get_${safe_src_dir_wle}_sha1();\n"
    "    }\n"
    )

    string(APPEND GET_EMBEDED_CONTENTS_PATH_BODY
    "    if ( path == get_${safe_src_dir_wle}_path()) {\n"
    "        content_size = ${safe_src_dir_wle}_text_size();\n"
    "        return get_${safe_src_dir_wle}_text();\n"
    "    }\n"
    )
    string(APPEND GET_EMBEDED_CONTENTS_SIZE_PATH_BODY
    "    if ( std::strcmp(path, get_${safe_src_dir_wle}_path()) ) {\n"
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

if(Biogears_IO_USE_SHORT_TARGET_NAMES)
  set(NAME_ARG SHORT_NAMES)
endif()

set ( IO_VERSION "${Biogears_LIB_VERSION}")
set ( IO_INCLUDE "c_encoded/${IO_VERSION}")

message(STATUS "  Iterating over source files to be encoded as C strings. This is a long operation, but only on the initial pass or when the source file changes.")
message(STATUS "  Registering file encoding")

message(STATUS "            : xsd/")
FILE(GLOB_RECURSE XSD_FILES RELATIVE "${PROJECT_SOURCE_DIR}/share/xsd" LIST_DIRECTORIES false CONFIGURE_DEPENDS "${PROJECT_SOURCE_DIR}/share/xsd/*.xsd" )
configure_io_directory_file( ${NAME_ARG} OUTPUT "directories/xsd.cpp.in" TYPE "xsd/" FILE_LIST ${XSD_FILES}  RESOURCE_PATH ${PROJECT_SOURCE_DIR}/share/ HEADERS GEN_HDRS SOURCES GEN_SRCS)

message(STATUS "            : data/config")
FILE(GLOB_RECURSE DATA_CONFIG_FILES      RELATIVE "${PROJECT_SOURCE_DIR}/share/data/config" LIST_DIRECTORIES false CONFIGURE_DEPENDS "${PROJECT_SOURCE_DIR}/share/data/config/*.xml" "${PROJECT_SOURCE_DIR}/share/data/config/*.config" )
LIST(APPEND DATA_CONFIG_FILES   "BioGearsConfiguration.xml")
configure_io_directory_file( ${NAME_ARG} OUTPUT "directories/config.cpp.in"  TYPE "config/" FILE_LIST ${DATA_CONFIG_FILES} RESOURCE_PATH ${PROJECT_SOURCE_DIR}/share/data HEADERS GEN_HDRS SOURCES GEN_SRCS)

message(STATUS "            : data/ecg")
FILE(GLOB_RECURSE DATA_ECG_FILES         RELATIVE "${PROJECT_SOURCE_DIR}/share/data/ecg" LIST_DIRECTORIES false CONFIGURE_DEPENDS "${PROJECT_SOURCE_DIR}/share/data/ecg/*.xml")
configure_io_directory_file( ${NAME_ARG} OUTPUT "directories/ecg.cpp.in"  TYPE "ecg/" FILE_LIST ${DATA_ECG_FILES} RESOURCE_PATH ${PROJECT_SOURCE_DIR}/share/data HEADERS GEN_HDRS SOURCES GEN_SRCS)

message(STATUS "            : data/environments")
FILE(GLOB_RECURSE DATA_ENIRONMENT_FILES  RELATIVE "${PROJECT_SOURCE_DIR}/share/data/environments" LIST_DIRECTORIES false CONFIGURE_DEPENDS "${PROJECT_SOURCE_DIR}/share/data/environments/*.xml")
configure_io_directory_file( ${NAME_ARG} OUTPUT "directories/environments.cpp.in"  TYPE "environments/" FILE_LIST ${DATA_ENIRONMENT_FILES} RESOURCE_PATH ${PROJECT_SOURCE_DIR}/share/data HEADERS GEN_HDRS SOURCES GEN_SRCS)

message(STATUS "            : data/nutrition")
FILE(GLOB_RECURSE DATA_NUTRITION_FILES   RELATIVE "${PROJECT_SOURCE_DIR}/share/data/nutrition" LIST_DIRECTORIES false CONFIGURE_DEPENDS "${PROJECT_SOURCE_DIR}/share/data/nutrition/*.xml")
configure_io_directory_file( ${NAME_ARG} OUTPUT "directories/nutrition.cpp.in"  TYPE "nutrition/" FILE_LIST ${DATA_NUTRITION_FILES} RESOURCE_PATH ${PROJECT_SOURCE_DIR}/share/data HEADERS GEN_HDRS SOURCES GEN_SRCS)

message(STATUS "            : data/override")
FILE(GLOB_RECURSE DATA_OVERRIDE_FILES    RELATIVE "${PROJECT_SOURCE_DIR}/share/data/override" LIST_DIRECTORIES false CONFIGURE_DEPENDS "${PROJECT_SOURCE_DIR}/share/data/override/*.xml")
configure_io_directory_file( ${NAME_ARG} OUTPUT "directories/override.cpp.in"  TYPE "override/" FILE_LIST ${DATA_OVERRIDE_FILES} RESOURCE_PATH ${PROJECT_SOURCE_DIR}/share/data HEADERS GEN_HDRS SOURCES GEN_SRCS)

message(STATUS "            : data/patients")
FILE(GLOB_RECURSE DATA_PATIENT_FILES     RELATIVE "${PROJECT_SOURCE_DIR}/share/data/patients" LIST_DIRECTORIES false CONFIGURE_DEPENDS "${PROJECT_SOURCE_DIR}/share/data/patients/*.xml")
configure_io_directory_file( ${NAME_ARG} OUTPUT "directories/patients.cpp.in"  TYPE "patients/" FILE_LIST ${DATA_PATIENT_FILES} RESOURCE_PATH ${PROJECT_SOURCE_DIR}/share/data HEADERS GEN_HDRS SOURCES GEN_SRCS)

option(Biogears_IO_INCLUDE_STATES "Add default states which can be generated using bg-cli to the IO library. This greatly increases the size of the io library" OFF)
if (Biogears_IO_INCLUDE_STATES)
message(STATUS "            : data/states")
FILE(GLOB_RECURSE DATA_STATE_FILES       RELATIVE "${PROJECT_SOURCE_DIR}/share/data/states" LIST_DIRECTORIES false CONFIGURE_DEPENDS "${PROJECT_SOURCE_DIR}/share/data/states/*.xml")
configure_io_directory_file( ${NAME_ARG} OUTPUT "directories/states.cpp.in" TYPE "states/" FILE_LIST ${DATA_STATE_FILES} RESOURCE_PATH ${PROJECT_SOURCE_DIR}/share/data HEADERS GEN_HDRS SOURCES GEN_SRCS)
endif()

message(STATUS "            : data/substances")
FILE(GLOB_RECURSE DATA_SUBSTANCE_FILES   RELATIVE "${PROJECT_SOURCE_DIR}/share/data/substances" LIST_DIRECTORIES false CONFIGURE_DEPENDS "${PROJECT_SOURCE_DIR}/share/data/substances/*.xml")
configure_io_directory_file( ${NAME_ARG} OUTPUT "directories/substances.cpp.in" TYPE "substances/" FILE_LIST ${DATA_SUBSTANCE_FILES} RESOURCE_PATH ${PROJECT_SOURCE_DIR}/share/data HEADERS GEN_HDRS SOURCES GEN_SRCS )

option(Biogears_IO_INCLUDE_DATA_TEMPLATES "Add default data templates which can be generated using bg-cli to the IO library. This greatly increases the size of the io library" OFF)
if (Biogears_IO_INCLUDE_DATA_TEMPLATES)
message(STATUS "            : data/templates")
FILE(GLOB_RECURSE DATA_SUBSTANCE_FILES   RELATIVE "${PROJECT_SOURCE_DIR}/share/data/templates" LIST_DIRECTORIES false CONFIGURE_DEPENDS "${PROJECT_SOURCE_DIR}/share/data/templates/*.csv")
configure_io_directory_file( ${NAME_ARG} OUTPUT "directories/templates.cpp.in" TYPE "templates/" FILE_LIST ${DATA_SUBSTANCE_FILES} RESOURCE_PATH ${PROJECT_SOURCE_DIR}/share/data HEADERS GEN_HDRS SOURCES GEN_SRCS )
endif()

option(Biogears_IO_INCLUDE_SCENARIOS "Add default scenarios which can be generated using bg-cli to the IO library. This greatly increases the size of the io library" OFF)
if (Biogears_IO_INCLUDE_SCENARIOS)
message(STATUS "            : Scenarios")
FILE(GLOB_RECURSE DATA_SUBSTANCE_FILES   RELATIVE "${PROJECT_SOURCE_DIR}/share/Scenarios" LIST_DIRECTORIES false CONFIGURE_DEPENDS "${PROJECT_SOURCE_DIR}/share/Scenarios/*.xml")
configure_io_directory_file( ${NAME_ARG} OUTPUT "directories/Scenarios.cpp.in" TYPE "Scenarios/" FILE_LIST ${DATA_SUBSTANCE_FILES} RESOURCE_PATH ${PROJECT_SOURCE_DIR}/share/ HEADERS GEN_HDRS SOURCES GEN_SRCS )
endif()

configure_file( src/constants.h.in  ${BIOGEARS_BINARY_DIR}/generated/biogears/io/constants.h NEWLINE_STYLE LF)
add_source_files(GEN_HDRS LOCATION ${BIOGEARS_BINARY_DIR}/generated/biogears/io/
               REGEX "constants.h" SOURCE_GROUP  "Headers\\Private\\")
add_source_files(INTERFACE_HEADERS LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/include/
               REGEX "*.h" SOURCE_GROUP  "Headers\\Public\\")
add_source_files(INTERFACE_SOURCES LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/src/
               REGEX "*.cpp" SOURCE_GROUP  "Sources\\")
##############################################################################
# Generator Target
##############################################################################
add_custom_target( hex_bindings 
                   COMMENT "Generating hex encoding headers" 
                   DEPENDS ${GEN_HDRS} ${GEN_SRCS} )
set_target_properties( hex_bindings PROPERTIES 
        FOLDER "Code Generators" )

###############################################################################
# libBiogears_io
###############################################################################
if(NOT Biogears_BUILD_STATIC)
  add_library(libbiogears_io  SHARED ${GEN_HDRS} ${GEN_SRCS} ${INTERFACE_HEADERS} ${INTERFACE_SOURCES})
  add_library(Biogears::libbiogears_io ALIAS libbiogears_io)
  set_target_properties(libbiogears_io
    PROPERTIES
    DEFINE_SYMBOL biogears_io_EXPORTS
    FOLDER "Libraries"
    OUTPUT_NAME "biogears_io"
    COMPILE_PDB_NAME "biogears_io"
    PROJECT_LABEL "libbiogears_io"
    DEBUG_POSTFIX "${CMAKE_DEBUG_POSTFIX}"
    CXX_STANDARD 14
    )
  set_target_properties(libbiogears_io PROPERTIES SOVERSION "${Biogears_LIB_VERSION}")
  add_dependencies(libbiogears_io hex_bindings xsd_bindings)
  target_compile_definitions(libbiogears_io  PUBLIC BIOGEARS_IO_PRESENT
                                                           $<$<BOOL:${Biogears_IO_INCLUDE_STATES}>:BIOGEARS_IO_EMBED_STATES> 
                                                           $<$<BOOL:${Biogears_IO_INCLUDE_DATA_TEMPLATES}>:BIOGEARS_IO_EMBED_TEMPLATES> 
                                                           $<$<BOOL:${Biogears_IO_INCLUDE_SCENARIOS}>:BIOGEARS_IO_EMBED_SCENARIOS>)
  ###############################################################################
  # libBiogears_io COMPILATION & LINKAGE MODIFICATIONS
  ###############################################################################
  set( BIO_CPPFLAGS $<$<PLATFORM_ID:Windows>:_SCL_SECURE_NO_WARNINGS _CRT_SECURE_NO_WARNINGS NOMINMAX > )
  target_compile_definitions(libbiogears_io  PUBLIC ${BIO_CPPFLAGS_INTERFACE} $<$<STREQUAL:${CMAKE_SYSTEM_PROCESSOR},armv7-a>:XSD_NO_EXPORT>)
  target_compile_definitions(libbiogears_io  PRIVATE ${BIO_CPPFLAGS} PRIVATE)
  target_compile_options(libbiogears_io PRIVATE $<$<PLATFORM_ID:Windows>:/bigobj>  PRIVATE $<$<PLATFORM_ID:Windows>:/MP${Biogears_BUILD_PARALLEL_LEVEL}>)
  
  target_include_directories(libbiogears_io 
      PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include
              ${BIOGEARS_BINARY_DIR}/generated
              ${BIOGEARS_BINARY_DIR}/generated/${IO_INCLUDE}
      
      INTERFACE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
      INTERFACE $<BUILD_INTERFACE:${BIOGEARS_BINARY_DIR}/generated>
      INTERFACE $<INSTALL_INTERFACE:include>
  )
  
  set(BIO_io_LIBS
            Biogears::libbiogears_common
  )
  target_link_libraries(libbiogears_io  PUBLIC ${BIO_io_LIBS} )
  
  install(DIRECTORY 
        ${CMAKE_CURRENT_SOURCE_DIR}/include/
        DESTINATION include
        FILE_PERMISSIONS OWNER_READ GROUP_READ WORLD_READ
   )
  
  install(TARGETS libbiogears_io  EXPORT libBiogears
      RUNTIME DESTINATION bin/${Biogears_PLATFORM_PREFIX}$<$<BOOL:${Biogears_BREAKOUT_INSTALL}>:/$<CONFIG>>
      LIBRARY DESTINATION ${LIBRARY_INSTALL_DIR}/${Biogears_PLATFORM_PREFIX}$<$<BOOL:${Biogears_BREAKOUT_INSTALL}>:/$<CONFIG>>
      ARCHIVE DESTINATION lib/${Biogears_PLATFORM_PREFIX}$<$<BOOL:${Biogears_BREAKOUT_INSTALL}>:/$<CONFIG>>
  )
  if(WIN32 AND NOT Biogears_BUILD_STATIC)
  install(FILES $<TARGET_PDB_FILE:libbiogears_io> DESTINATION lib/${Biogears_PLATFORM_PREFIX}$<$<BOOL:${Biogears_BREAKOUT_INSTALL}>:/$<CONFIG>> OPTIONAL)
endif()
endif()
