# Find Protobuf
#
# Find the native Protobuf headers and libraries.
#[==[
#   Defines an Import Target Protobuf::protobuf
#   Defines an Import Target Protobuf::protoc
#...Defines an Import Target Protobuf::lite
#   Protobuf_FOUND          - True if Protobuf found.
#   Protobuf_VERSION_STRING - the version of Protobuf found (since CMake 2.8.8)
#   Provides macros for generating code with protobuffs
#   REGISTER_PROTOBUF _filename _component
#   --_filename : The proto file to be parsed
#   --_component: File path relative to BINARY_DIR where the code should be placed
#   CUSTOM_PROTOBUF_GENERATE_CPP PATH HDRS SRCS
#   -- PATH : The Proto file to be parsed
#   -- SRCS : Var to store the srcs
#   -- HDRS : Var to store the hdrs
#   PROTOBUF_GENERATE_CPP    SRCS HDRS
#   -- SRCS : Var to store the srcs
#   -- HDRS : Var to store the hdrs
#   PROTOBUF_GENERATE_PYTHON SRCS
#   -- SRCS : Var to store the srcs

#]==]
function(REGISTER_PROTOBUF _filename _component)

  if( NOT EXISTS ${CMAKE_CURRENT_BINARY_DIR}/${_component} )
          file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/${_component} )
  endif()
  
  if( NOT EXISTS ${CMAKE_CURRENT_BINARY_DIR}/${_component}/${_filename}.pb.h OR NOT EXISTS ${CMAKE_CURRENT_BINARY_DIR}/${_component}/${_filename}.pb.cc )
    message(STATUS "Generating ${CMAKE_CURRENT_BINARY_DIR}/${_component}/${_filename}.pb.{h,cc}")
    message (STATUS "COMMAND ${CMAKE_COMMAND} -E env LD_LIBRARY_PATH=${ARC4_DIR_HOST_THIRD_PARTY}/lib ${Protobuf_PROTOC_EXECUTABLE} --cpp_out=${CMAKE_CURRENT_BINARY_DIR}/${_component} ${_filename}.proto" )
    message (STATUS "WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/share/proto/")
    execute_process( COMMAND ${CMAKE_COMMAND} -E env LD_LIBRARY_PATH=${ARC4_DIR_HOST_THIRD_PARTY}/lib ${Protobuf_PROTOC_EXECUTABLE} --cpp_out=${CMAKE_CURRENT_BINARY_DIR}/${_component} ${_filename}.proto
                    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/share/proto/
                    ERROR_VARIABLE PROTO_ERROR
                    OUTPUT_QUIET
    )

    if(PROTO_ERROR)
      message(FATAL_ERROR "Protobuf Generation Failed!?  Check the value of Protobuf_PROTOC_EXECUTABLE\n"
                          "Error Msg:\n"
                          "${PROTO_ERROR}\n"
             )
    endif(PROTO_ERROR)
  endif()
  add_custom_command( OUTPUT  ${CMAKE_CURRENT_BINARY_DIR}/${_component}/${_filename}.pb.h ${CMAKE_CURRENT_BINARY_DIR}/${_component}/${_filename}.pb.cc
                      COMMAND ${CMAKE_COMMAND} -E env LD_LIBRARY_PATH=${ARC4_DIR_HOST_THIRD_PARTY}/lib ${Protobuf_PROTOC_EXECUTABLE} --cpp_out=${CMAKE_CURRENT_BINARY_DIR}/${_component} ${_filename}.proto
                      WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/share/proto/
                      DEPENDS ${Protobuf_PROTOC_EXECUTABLE} ${PROJECT_SOURCE_DIR}/share/proto/${_filename}.proto
                      COMMENT "Generating Protobuf ${_filename}.proto")
  
  add_custom_target( proto_${_component}_${_filename} DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/${_component}/${_filename}.pb.h ${CMAKE_CURRENT_BINARY_DIR}/${_component}/${_filename}.pb.cc
                     COMMENT "\t\t${_component}/${_filename}.pb.{h,cc}" )
  
  install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${_component}/${_filename}.pb.h 

        DESTINATION ${CMAKE_INSTALL_PREFIX}/include/arc4/${_component}/${_filename}.pb.h
        PERMISSIONS  OWNER_READ GROUP_READ WORLD_READ
       )
  set_target_properties(proto_${_component}_${_filename}
      PROPERTIES
      FOLDER "Code Generators"
      PROJECT_LABEL "Proto_${_filename}")
endfunction(REGISTER_PROTOBUF)

function(CUSTOM_PROTOBUF_GENERATE_CPP PATH HDRS SRCS)
  if(NOT ARGN)
    message(SEND_ERROR "Error: PROTOBUF_GENERATE_CPP() called without any proto files")
    return()
  endif()

  if(PROTOBUF_GENERATE_CPP_APPEND_PATH)
    # Create an include path for each file specified
    foreach(FIL ${ARGN})
      get_filename_component(ABS_FIL ${FIL} ABSOLUTE)
      get_filename_component(ABS_PATH ${ABS_FIL} PATH)
      list(FIND _protobuf_include_path ${ABS_PATH} _contains_already)
      if(${_contains_already} EQUAL -1)
          list(APPEND _protobuf_include_path -I ${ABS_PATH})
      endif()
    endforeach()
  else()
    set(_protobuf_include_path -I ${CMAKE_CURRENT_SOURCE_DIR})
  endif()

  if(DEFINED PROTOBUF_IMPORT_DIRS)
    foreach(DIR ${PROTOBUF_IMPORT_DIRS})
      get_filename_component(ABS_PATH ${DIR} ABSOLUTE)
      list(FIND _protobuf_include_path ${ABS_PATH} _contains_already)
      if(${_contains_already} EQUAL -1)
          list(APPEND _protobuf_include_path -I ${ABS_PATH})
      endif()
    endforeach()
  endif()

  set(${SRCS})
  set(${HDRS})
  foreach(FIL ${ARGN})
    get_filename_component(ABS_FIL ${FIL} ABSOLUTE)
    get_filename_component(FIL_WE ${FIL} NAME_WE)

    list(APPEND ${SRCS} "${CMAKE_CURRENT_SOURCE_DIR}/${PATH}/${FIL_WE}.pb.cc")
    list(APPEND ${HDRS} "${CMAKE_CURRENT_SOURCE_DIR}/${PATH}/${FIL_WE}.pb.h")

    execute_process(COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_CURRENT_SOURCE_DIR}/${PATH})

    add_custom_command(
      OUTPUT "${CMAKE_CURRENT_SOURCE_DIR}/${PATH}/${FIL_WE}.pb.cc"
             "${CMAKE_CURRENT_SOURCE_DIR}/${PATH}/${FIL_WE}.pb.h"
      COMMAND  ${PROTOBUF_PROTOC_EXECUTABLE}
      ARGS --cpp_out  ${CMAKE_CURRENT_SOURCE_DIR}/${PATH} ${_protobuf_include_path} ${ABS_FIL}
      DEPENDS ${ABS_FIL}
      COMMENT "Running C++ protocol buffer compiler on ${FIL}"
      VERBATIM 
	)
  endforeach()

  set_source_files_properties(${${SRCS}} ${${HDRS}} PROPERTIES GENERATED TRUE)
  set(${SRCS} ${${SRCS}} PARENT_SCOPE)
  set(${HDRS} ${${HDRS}} PARENT_SCOPE)
endfunction()

function(PROTOBUF_GENERATE_CPP SRCS HDRS)
  if(NOT ARGN)
    message(SEND_ERROR "Error: PROTOBUF_GENERATE_CPP() called without any proto files")
    return()
  endif()

  if(PROTOBUF_GENERATE_CPP_APPEND_PATH)
    # Create an include path for each file specified
    foreach(FIL ${ARGN})
      get_filename_component(ABS_FIL ${FIL} ABSOLUTE)
      get_filename_component(ABS_PATH ${ABS_FIL} PATH)
      list(FIND _protobuf_include_path ${ABS_PATH} _contains_already)
      if(${_contains_already} EQUAL -1)
          list(APPEND _protobuf_include_path -I ${ABS_PATH})
      endif()
    endforeach()
  else()
    set(_protobuf_include_path -I ${CMAKE_CURRENT_SOURCE_DIR})
  endif()

  if(DEFINED Protobuf_IMPORT_DIRS)
    foreach(DIR ${Protobuf_IMPORT_DIRS})
      get_filename_component(ABS_PATH ${DIR} ABSOLUTE)
      list(FIND _protobuf_include_path ${ABS_PATH} _contains_already)
      if(${_contains_already} EQUAL -1)
          list(APPEND _protobuf_include_path -I ${ABS_PATH})
      endif()
    endforeach()
  endif()

  set(${SRCS})
  set(${HDRS})
  foreach(FIL ${ARGN})
    get_filename_component(ABS_FIL ${FIL} ABSOLUTE)
    get_filename_component(FIL_WE ${FIL} NAME_WE)

    list(APPEND ${SRCS} "${CMAKE_CURRENT_BINARY_DIR}/${FIL_WE}.pb.cc")
    list(APPEND ${HDRS} "${CMAKE_CURRENT_BINARY_DIR}/${FIL_WE}.pb.h")

    add_custom_command(
      OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/${FIL_WE}.pb.cc"
             "${CMAKE_CURRENT_BINARY_DIR}/${FIL_WE}.pb.h"
      COMMAND  ${Protobuf_PROTOC_EXECUTABLE}
      ARGS --cpp_out  ${CMAKE_CURRENT_BINARY_DIR} ${_protobuf_include_path} ${ABS_FIL}
      DEPENDS ${ABS_FIL} ${Protobuf_PROTOC_EXECUTABLE}
      COMMENT "Running C++ protocol buffer compiler on ${FIL}"
      VERBATIM )
  endforeach()

  set_source_files_properties(${${SRCS}} ${${HDRS}} PROPERTIES GENERATED TRUE)
  set(${SRCS} ${${SRCS}} PARENT_SCOPE)
  set(${HDRS} ${${HDRS}} PARENT_SCOPE)
endfunction()

function(PROTOBUF_GENERATE_PYTHON SRCS)
  if(NOT ARGN)
    message(SEND_ERROR "Error: PROTOBUF_GENERATE_PYTHON() called without any proto files")
    return()
  endif()

  if(PROTOBUF_GENERATE_CPP_APPEND_PATH)
    # Create an include path for each file specified
    foreach(FIL ${ARGN})
      get_filename_component(ABS_FIL ${FIL} ABSOLUTE)
      get_filename_component(ABS_PATH ${ABS_FIL} PATH)
      list(FIND _protobuf_include_path ${ABS_PATH} _contains_already)
      if(${_contains_already} EQUAL -1)
          list(APPEND _protobuf_include_path -I ${ABS_PATH})
      endif()
    endforeach()
  else()
    set(_protobuf_include_path -I ${CMAKE_CURRENT_SOURCE_DIR})
  endif()

  if(DEFINED Protobuf_IMPORT_DIRS)
    foreach(DIR ${Protobuf_IMPORT_DIRS})
      get_filename_component(ABS_PATH ${DIR} ABSOLUTE)
      list(FIND _protobuf_include_path ${ABS_PATH} _contains_already)
      if(${_contains_already} EQUAL -1)
          list(APPEND _protobuf_include_path -I ${ABS_PATH})
      endif()
    endforeach()
  endif()

  set(${SRCS})
  foreach(FIL ${ARGN})
    get_filename_component(ABS_FIL ${FIL} ABSOLUTE)
    get_filename_component(FIL_WE ${FIL} NAME_WE)

    list(APPEND ${SRCS} "${CMAKE_CURRENT_BINARY_DIR}/${FIL_WE}_pb2.py")
    add_custom_command(
      OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/${FIL_WE}_pb2.py"
      COMMAND  ${Protobuf_PROTOC_EXECUTABLE} --python_out ${CMAKE_CURRENT_BINARY_DIR} ${_protobuf_include_path} ${ABS_FIL}
      DEPENDS ${ABS_FIL} ${Protobuf_PROTOC_EXECUTABLE}
      COMMENT "Running Python protocol buffer compiler on ${FIL}"
      VERBATIM )
  endforeach()

  set(${SRCS} ${${SRCS}} PARENT_SCOPE)
endfunction()

# Environment

if(CMAKE_SIZEOF_VOID_P EQUAL 8)
  set(_PROTOBUF_ARCH_DIR x64/)
endif()

include(SelectLibraryConfigurations)

# Internal function: search for normal library as well as a debug one
#    if the debug one is specified also include debug/optimized keywords
#    in *_LIBRARIES variable
function(_protobuf_find_libraries name filename)
  if(${name}_LIBRARIES)
    # Use result recorded by a previous call.
  elseif(${name}_LIBRARY)
    # Honor cache entry used by CMake 3.5 and lower.
    set(${name}_LIBRARIES "${${name}_LIBRARY}" PARENT_SCOPE)
  else()
    find_library(${name}_LIBRARY_RELEASE
      NAMES
        ${filename}
        lib${filename}
      HINTS
      PATHS
      PATH_SUFFIXES
        release
    )
  
    find_library(${name}_LIBRARY_DEBUG
      NAMES
        ${filename}_d
        lib${filename}_d
        ${filename}d
        lib${filename}d
        ${filename}
        lib${filename}
      PATH_SUFFIXES
        debug
    )

    select_library_configurations(${name})
    set(${name}_LIBRARY ${${name}_LIBRARY} PARENT_SCOPE)
    set(${name}_LIBRARIES ${${name}_LIBRARIES} PARENT_SCOPE)
  endif()
endfunction()

# Internal function: find threads library
function(_protobuf_find_threads)
    set(CMAKE_THREAD_PREFER_PTHREAD TRUE)
    find_package(Threads)
    if(Threads_FOUND)
        list(APPEND PROTOBUF_LIBRARIES ${CMAKE_THREAD_LIBS_INIT})
        set(PROTOBUF_LIBRARIES "${PROTOBUF_LIBRARIES}" PARENT_SCOPE)
    endif()
endfunction()

#
# Main.
#

# By default have PROTOBUF_GENERATE_CPP macro pass -I to protoc
# for each directory where a proto file is referenced.
if(NOT DEFINED PROTOBUF_GENERATE_CPP_APPEND_PATH)
  set(PROTOBUF_GENERATE_CPP_APPEND_PATH TRUE)
endif()

# The Protobuf library
_protobuf_find_libraries(Protobuf protobuf)

# The Protobuf Lite library
_protobuf_find_libraries(Protobuf_LITE protobuf-lite)

# The Protobuf Protoc Library
_protobuf_find_libraries(Protobuf_PROTOC protoc)

if(UNIX)
  _protobuf_find_threads()
endif()

# Find the include directory
find_path(Protobuf_INCLUDE_DIR 
  NAMES
    google/protobuf/compiler/cpp/cpp_generator.h
  PATH_SUFFIXES
    include
)

# Find the protoc Executable
if( CMAKE_HOST_WIN32 )
  find_file(Protobuf_PROTOC_EXECUTABLE
            NAMES "protoc.exe"
            HINTS ${ARC4_DIR_HOST_THIRD_PARTY}
            PATH_SUFFIXES "bin" )
else()
  find_file(Protobuf_PROTOC_EXECUTABLE
            NAMES "bin/protoc"
            HINTS "${ARC4_DIR_HOST_THIRD_PARTY}"
            NO_DEFAULT_PATH
            )
endif()

# Version info variable
# set(Protobuf_VERSION "3.1.0")

if(Protobuf_DEBUG)
    message(STATUS "[ ${CMAKE_CURRENT_LIST_FILE}:${CMAKE_CURRENT_LIST_LINE} ] "
        "requested version of Google Protobuf is ${Protobuf_FIND_VERSION}")
endif()

if(Protobuf_INCLUDE_DIR)
  set(_PROTOBUF_COMMON_HEADER ${Protobuf_INCLUDE_DIR}/google/protobuf/stubs/common.h)

  if(Protobuf_DEBUG)
    message(STATUS "[ ${CMAKE_CURRENT_LIST_FILE}:${CMAKE_CURRENT_LIST_LINE} ] "
                   "location of common.h: ${_PROTOBUF_COMMON_HEADER}")
  endif()

  set(Protobuf_VERSION "")
  set(Protobuf_LIB_VERSION "")
  file(STRINGS ${_PROTOBUF_COMMON_HEADER} _PROTOBUF_COMMON_H_CONTENTS REGEX "#define[ \t]+GOOGLE_PROTOBUF_VERSION[ \t]+")
  if(_PROTOBUF_COMMON_H_CONTENTS MATCHES "#define[ \t]+GOOGLE_PROTOBUF_VERSION[ \t]+([0-9]+)")
      set(Protobuf_LIB_VERSION "${CMAKE_MATCH_1}")
  endif()
  unset(_PROTOBUF_COMMON_H_CONTENTS)

  math(EXPR _PROTOBUF_MAJOR_VERSION "${Protobuf_LIB_VERSION} / 1000000")
  math(EXPR _PROTOBUF_MINOR_VERSION "${Protobuf_LIB_VERSION} / 1000 % 1000")
  math(EXPR _PROTOBUF_SUBMINOR_VERSION "${Protobuf_LIB_VERSION} % 1000")
  set(Protobuf_VERSION "${_PROTOBUF_MAJOR_VERSION}.${_PROTOBUF_MINOR_VERSION}.${_PROTOBUF_SUBMINOR_VERSION}")

  if(Protobuf_DEBUG)
    message(STATUS "[ ${CMAKE_CURRENT_LIST_FILE}:${CMAKE_CURRENT_LIST_LINE} ] "
        "${_PROTOBUF_COMMON_HEADER} reveals protobuf ${Protobuf_VERSION}")
  endif()

  # Check Protobuf compiler version to be aligned with libraries version
  execute_process(COMMAND ${Protobuf_PROTOC_EXECUTABLE} --version
                  OUTPUT_VARIABLE _PROTOBUF_PROTOC_EXECUTABLE_VERSION)

  if("${_PROTOBUF_PROTOC_EXECUTABLE_VERSION}" MATCHES "libprotoc ([0-9.]+)")
    set(_PROTOBUF_PROTOC_EXECUTABLE_VERSION "${CMAKE_MATCH_1}")
  endif()

  if(Protobuf_DEBUG)
    message(STATUS "[ ${CMAKE_CURRENT_LIST_FILE}:${CMAKE_CURRENT_LIST_LINE} ] "
        "${Protobuf_PROTOC_EXECUTABLE} reveals version ${_PROTOBUF_PROTOC_EXECUTABLE_VERSION}")
  endif()

  if(NOT "${_PROTOBUF_PROTOC_EXECUTABLE_VERSION}" VERSION_EQUAL "${Protobuf_VERSION}")
      message(WARNING "Protobuf compiler version ${_PROTOBUF_PROTOC_EXECUTABLE_VERSION}"
          " doesn't match library version ${Protobuf_VERSION}")
  endif()
endif()

include(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(Protobuf
    REQUIRED_VARS Protobuf_PROTOC_EXECUTABLE Protobuf_LIBRARIES Protobuf_INCLUDE_DIR
    VERSION_VAR Protobuf_VERSION
)

if(Protobuf_FOUND)
    add_library(Protobuf::protobuf IMPORTED  GLOBAL)
    set_target_properties(Protobuf::protobuf
        PROPERTIES
        IMPORTED_IMPLIB_DEBUG   ${Protobuf_LIBRARY_DEBUG}
        IMPORTED_IMPLIB ${Protobuf_LIBRARY}`
        INTERFACE_SYSTEM_INCLUDE_DIRECTORIES
        ${Protobuf_INCLUDE_DIR}
       )
    add_library(Protobuf::protoc IMPORTED  GLOBAL)
    set_target_properties(Protobuf::protoc
        PROPERTIES
        IMPORTED_IMPLIB_DEBUG   ${Protobuf_PROTOC_LIBRARY_DEBUG}
        IMPORTED_IMPLIB ${Protobuf_PROTOC_LIBRARY}
        INTERFACE_SYSTEM_INCLUDE_DIRECTORIES
        ${Protobuf_INCLUDE_DIR}
        PROTOBUF_PROTOC_LIBRARY )
    add_library(Protobuf::lite IMPORTED  GLOBAL)
    set_target_properties(Protobuf::lite
        PROPERTIES
        IMPORTED_IMPLIB_DEBUG   ${Protobuf_LITE_LIBRARY_DEBUG}
        IMPORTED_IMPLIB ${Protobuf_LITE_LIBRARY}
        INTERFACE_SYSTEM_INCLUDE_DIRECTORIES
        ${Protobuf_INCLUDE_DIR}
      PROTOBUF_LITE_LIBRARY )

    set(Protobuf_INCLUDE_DIRS ${Protobuf_INCLUDE_DIR})
	mark_as_advanced(Protobuf_INCLUDE_DIR)
	mark_as_advanced(Protobuf_LIBRARY)
	mark_as_advanced(Protobuf_LIBRARY_DEBUG)
	mark_as_advanced(Protobuf_PROTOC_LIBRARY)
	mark_as_advanced(Protobuf_PROTOC_LIBRARY_DEBUG)
	mark_as_advanced(Protobuf_LITE_LIBRARY)
	mark_as_advanced(Protobuf_LITE_LIBRARY_DEBUG)
	mark_as_advanced(Protobuf_DIR)
endif()
