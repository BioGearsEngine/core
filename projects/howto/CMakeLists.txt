cmake_minimum_required(VERSION 3.8.0)

# Policy adjustments
cmake_policy(VERSION 3.8.0 )

option(ARA_${ROOT_PROJECT_NAME}_BUILD_HOWTOS     "Select to build sdk how to examples." ON)

	set(OUTPUT_PREFIX ${CMAKE_BINARY_DIR}/outputs)
	if(UNIX)
	set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${OUTPUT_PREFIX}/howto/lib)
	set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${OUTPUT_PREFIX}/howto/lib)
	set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${OUTPUT_PREFIX}/howto/bin)
	else()
	  foreach(OUTPUTCONFIG ${CMAKE_CONFIGURATION_TYPES})
		  string(TOUPPER _${OUTPUTCONFIG} OUTPUTCONFIG_UPPER)
		  set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY${OUTPUTCONFIG_UPPER} "${OUTPUT_PREFIX}/${OUTPUTCONFIG}/lib")
		  set(CMAKE_PDB_OUTPUT_DIRECTORY_${OUTPUTCONFIG_UPPER} "${OUTPUT_PREFIX}/${OUTPUTCONFIG}/lib")
		  set(CMAKE_LIBRARY_OUTPUT_DIRECTORY${OUTPUTCONFIG_UPPER} "${OUTPUT_PREFIX}/${OUTPUTCONFIG}/bin")
		  set(CMAKE_RUNTIME_OUTPUT_DIRECTORY${OUTPUTCONFIG_UPPER} "${OUTPUT_PREFIX}/${OUTPUTCONFIG}/bin")
	  endforeach(OUTPUTCONFIG)
	endif()
  
	############################################################################
	# Step 1:
	#         Examples are almost never order dependent just add them as we find
	#         them
	############################################################################
	if(ARA_${ROOT_PROJECT_NAME}_BUILD_HOWTOS)
	  include ( ${PROJECT_SOURCE_DIR}/cmake/cmake-common_logic.cmake )

	  childlist( dir_list ${CMAKE_CURRENT_SOURCE_DIR} )
	  foreach( dir IN LISTS dir_list )
		if(NOT dir MATCHES "common-source")
		  add_subdirectory( ${dir} )
		endif()
	  endforeach()
	endif()

	message(STATUS "")

if (NOT (Java_FOUND AND ARA_${ROOT_PROJECT_NAME}_BUILD_JAVATOOLS) )
	message(WARNING "SDK examples require the use of gen-states which requires ARA_${ROOT_PROJECT_NAME}_BUILD_JAVATOOLS and a copy of the Java development kit.")
endif()