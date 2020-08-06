include(BundleUtilities)
include(cmake-common_cache.cmake)
include("${PROJECT_SOURCE_DIR}/cmake/cmake-common_logic.cmake")
############/############################################################################################
#
# Source File Managment Macros
########################################################################################################
function(CHILDLIST result curdir)
  file(GLOB children RELATIVE ${curdir} ${curdir}/*)
  set(dirlist "")
  foreach(child ${children})
    if(IS_DIRECTORY ${curdir}/${child})
      list(APPEND dirlist ${curdir}/${child})
    endif()
  endforeach()
  set(${result} ${dirlist} PARENT_SCOPE)
endfunction()

function(EXELIST result curdir)
  file(GLOB children RELATIVE ${curdir} ${curdir}/*)
  set(dirlist "")
  foreach(child ${children})
    if(NOT IS_DIRECTORY ${curdir}/${child} AND ${child} MATCHES ".*${CMAKE_EXECUTABLE_SUFFIX}" )
      list(APPEND dirlist ${curdir}/${child})
	  set(${result} ${dirlist} PARENT_SCOPE)
	  return()
    endif()
  endforeach()
endfunction()


function(add_source_files var prefix regex source_group)
    message(STATUS "add_source_files( ${var} \"${prefix}\" ${regex} \"${source_group}\")")
    file(GLOB TEMP "${prefix}/${regex}")

    source_group("${source_group}" FILES ${TEMP})

    CHILDLIST( result ${prefix})
    
    foreach( dir IN LISTS result)
      add_source_files( ${var} "${prefix}/${dir}" ${regex} "${source_group}/${dir}")
    endforeach()

    set(${var} ${${var}} ${TEMP} PARENT_SCOPE)
endfunction()
########################################################################################################
#
# Find all bundles
#
########################################################################################################

string(TOUPPER ${CMAKE_INSTALL_CONFIG_NAME} CONFIG)
if( NOT UNIX )
  set( CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${CONFIG}} )
  set( CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_LIBRARY_OUTPUT_DIRECTORY_${CONFIG}} )
  set( CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY_${CONFIG}} )
endif()

set(CONFIG_SUFFIX ${CMAKE_${CONFIG}_POSTFIX})
foreach(_dir IN LISTS CMAKE_PREFIX_PATH)
    list(APPEND THIRD_PARTY  "${_dir}")
    list(APPEND THIRD_PARTY_LIB "${_dir}/lib")
    list(APPEND THIRD_PARTY_BIN "${_dir}/bin")
endforeach()


EXELIST(projects ${CMAKE_RUNTIME_OUTPUT_DIRECTORY} )

foreach( project IN LISTS projects)
	fixup_bundle(${project}
                  ""
                 "${THIRD_PARTY_LIB};${THIRD_PARTY_BIN}"
	  )
endforeach()
