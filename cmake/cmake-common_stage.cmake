include(BundleUtilities)
include(cmake-common_cache.cmake)
include(${PROJECT_SOURCE_DIR}/cmake/cmake-common_logic.cmake)
########################################################################################################
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
set(THIRD_PARTY ${cmake-common_THIRD_PARTY})
set(THIRD_PARTY_LIB ${cmake-common_THIRD_PARTY_LIB})
set(THIRD_PARTY_BIN ${cmake-common_THIRD_PARTY_BIN})

CHILDLIST(projects ${PROJECT_SOURCE_DIR}/projects )

foreach( project IN LISTS projects)
  if (EXISTS ${project}/ext/cmake/bundle.cmake)
    message(STATUS "Bundling ${project}" )
    include(${project}/ext/cmake/bundle.cmake)
  endif()
  get_filename_component(project_name ${project} NAME )
  if ( project_name STREQUAL examples ) 
    CHILDLIST(examples ${project} )
    foreach( example IN LISTS examples)
      if(EXISTS ${example}/ext/cmake/bundle.cmake)
         message(STATUS "Bundling ${example}" )  
         include(${example}/ext/cmake/bundle.cmake)
      endif()
    endforeach()
  endif()
endforeach()
